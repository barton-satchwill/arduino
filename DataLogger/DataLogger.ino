#include <SD.h>
#include <Wire.h>
#include "RTClib.h"

// A simple data logger for the Arduino analog pins

// how many milliseconds between grabbing data and logging it. 1000 ms is once a second
#define LOG_INTERVAL  1000 // mills between entries (reduce to take more/faster data)

// how many milliseconds before writing the logged data permanently to disk
// set it to the LOG_INTERVAL to write each time (safest)
// set it to 10*LOG_INTERVAL to write all data every 10 datareads, you could lose up to 
// the last 10 reads if power is lost but it uses less power and is much faster!
#define SYNC_INTERVAL 10*LOG_INTERVAL // mills between calls to flush() - to write data to the card
uint32_t syncTime = 0; // time of last sync()

#define ECHO_TO_SERIAL   0 // echo data to serial port
#define WAIT_TO_START    0 // Wait for serial input in setup()

// the digital pins that connect to the LEDs
#define redLEDpin 2
#define greenLEDpin 3

// The analog pins that connect to the sensors
#define PHOTOCELL 1         //photocell analog in pin 1
#define photocellPin 1      // analog 1
#define ThermistorPIN 0     // analog 0
#define tempPin 0           // analog 0
#define BANDGAPREF 14       // special indicator that we want to measure the bandgap

//#define aref_voltage 3.3         // we tie 3.3V to ARef and measure it with a multimeter!
#define aref_voltage 4.91        // we tie 3.3V to ARef and measure it with a multimeter!
#define bandgap_voltage 1.1      // this is not super guaranteed but its not -too- off

RTC_DS1307 RTC; // define the Real Time Clock object

// for the data logging shield, we use digital pin 10 for the SD cs line
const int chipSelect = 10;

// the logging file
File logfile;

// ==============================================================================


void setup(void)
{
  Serial.begin(9600);
  Serial.println();

  // use debugging LEDs
  pinMode(redLEDpin, OUTPUT);
  pinMode(greenLEDpin, OUTPUT);

#if WAIT_TO_START
  Serial.println("Type any character to start");
  while (!Serial.available());
#endif //WAIT_TO_START

  setupSDcard();
  setupLoggingFile();
  setupRealTimeClock();
  calibrate();

  log(logfile, "millis,stamp,datetime,light,temp,vcc\n");
//  log(logfile, "millis, stamp, datetime, temp, vcc\n");

  // If you want to set the aref to something other than 5v
  //analogReference(EXTERNAL);
}


void loop(void)
{
  // delay for the amount of time we want between readings
  delay((LOG_INTERVAL -1) - (millis() % LOG_INTERVAL));
  digitalWrite(greenLEDpin, HIGH);
  // log milliseconds since starting
  log(logfile, String(millis()) + ", ");
  printDateTime(RTC.now());
  readPhotocell();
//  readTemperature();
  readThermistor();
  readVoltage();
  digitalWrite(greenLEDpin, LOW);
  writeDataLog(logfile);
}

// ------------------------ set up ---------------------------------
void setupSDcard(){
  // initialize the SD card
  Serial.print("Initializing SD card...");
  // make sure that the default chip select pin is set to
  // output, even if you don't use it:
  pinMode(10, OUTPUT);

  // see if the card is present and can be initialized:
  if (!SD.begin(chipSelect)) {
    error("Card failed, or not present");
  }
  Serial.println("card initialized.");

}  

void setupLoggingFile(){
  // create a new file
  char filename[] = "LOGGER00.CSV";
  for (uint8_t i = 0; i < 100; i++) {
    filename[6] = i/10 + '0';
    filename[7] = i%10 + '0';
    if (! SD.exists(filename)) {
      // only open a new file if it doesn't exist
      logfile = SD.open(filename, FILE_WRITE); 
      break;  // leave the loop!
    }
  }
  if (! logfile) {
    error("couldnt create file");
  }
  Serial.print("Logging to: ");
  Serial.println(filename);
}


void setupRealTimeClock(){
  Wire.begin();  
  if (!RTC.begin()) {
    log(logfile, "RTC failed");
  }
}

// --------------------- reading sensors ---------------------------
void readThermistor(){
  float temp;
  temp=Thermistor(analogRead(ThermistorPIN));       // read ADC and  convert it to Celsius
//  Serial.print("Celsius: "); 
//  Serial.print(temp,1);                             // display Celsius
  //temp = (temp * 9.0)/ 5.0 + 32.0;                  // converts to  Fahrenheit
  //Serial.print(", Fahrenheit: "); 
  //Serial.print(temp,1);                             // display  Fahrenheit
//  Serial.println("");                                   
  log(logfile, ", " + printDouble(temp,100));
}


void readPhotocell(){
  analogRead(photocellPin);
  delay(10); 
  int photocellReading = analogRead(photocellPin);  
  photocellReading = map(photocellReading, 40, 960, 0, 30);
  log(logfile, ", " + String(photocellReading));
}


void readTemperature(){
  analogRead(tempPin); 
  delay(10);
  int tempReading = analogRead(tempPin);    
  // converting that reading to voltage, for 3.3v arduino use 3.3, for 5.0, use 5.0
  float voltage = tempReading * aref_voltage / 1024;  
  float temperatureC = (voltage - 0.5) * 100 ;
  float temperatureF = (temperatureC * 9 / 5) + 32;
  log(logfile, ", " + printDouble(temperatureF,100)); 
}


void readVoltage(){
  // Log the estimated 'VCC' voltage by measuring the internal 1.1v ref
  analogRead(BANDGAPREF); 
  delay(10);
  int refReading = analogRead(BANDGAPREF); 
  float supplyvoltage = (bandgap_voltage * 1024) / refReading; 
  log(logfile, ", " + printDouble(supplyvoltage, 100) + "\n");
}


void writeDataLog(File logfile){
  digitalWrite(greenLEDpin, LOW);
  // Now we write data to disk! Don't sync too often - requires 2048 bytes of I/O to SD card
  // which uses a bunch of power and takes time
  if ((millis() - syncTime) < SYNC_INTERVAL) return;
  syncTime = millis();

  // blink LED to show we are syncing data to the card & updating FAT!
  digitalWrite(redLEDpin, HIGH);
  logfile.flush();
  digitalWrite(redLEDpin, LOW);
}  


void error(char *str)
{
  Serial.print("error: ");
  Serial.println(str);
  // red LED indicates error
  digitalWrite(redLEDpin, HIGH);
  while(1);
}








//void setup() {
//  Serial.begin(9600);
//}
//
//void loop() {
//  Serial.print("thermistor pin: ");
//  Serial.println(analogRead(ThermistorPIN));
//  float temp;
//  temp=Thermistor(analogRead(ThermistorPIN));       // read ADC and  convert it to Celsius
//  Serial.print("Celsius: "); 
//  Serial.print(temp,1);                             // display Celsius
//  //temp = (temp * 9.0)/ 5.0 + 32.0;                  // converts to  Fahrenheit
//  //Serial.print(", Fahrenheit: "); 
//  //Serial.print(temp,1);                             // display  Fahrenheit
//  Serial.println("");                                   
//  delay(1000);                                      // Delay a bit... 
//}


