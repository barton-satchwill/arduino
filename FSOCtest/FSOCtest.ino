
#include <SoftwareSerial.h>

//----------------------------------------------------
//
//    o------/\/\/\-----o-----[photocell]------o
//   gnd      10k       A0                     5v
//
//----------------------------------------------------
//
//    o-----[led]----/\/\/\------o
//   gnd              10k       A1  
//
//----------------------------------------------------

/************************************************************************************************************
 * http://www.instructables.com/id/Arduino-Timer-Interrupts/
 * timer speed (Hz) = (Arduino clock speed (16MHz)) / prescaler
 * prescaler can be either 1, 8, 64, 256 or 1024
 * prescaler can be either 2^0, 2^3, 2^6, 2^7 or 2^8
 * interrupt frequency (Hz) = (Arduino clock speed 16,000,000Hz) / (prescaler * (compare match register + 1))
 * compare match register = [ 16,000,000Hz/ (prescaler * desired interrupt frequency) ] - 1
 * 
 * CS12    CS11    CS10      Prescaler
 * ------------------------------------------------------------------------------
 * 0       0       0       Timer off/no clock source
 * 0       0       1       1:1 prescaler/this is the same as not having prescaler
 * 0       1       0       1:8 prescaler
 * 0       1       1       1:64 prescaler
 * 1       0       0       1:256 prescaler
 * 1       0       1       1:1024 prescaler
 ************************************************************************************************************/

#define LED1 13
#define LEDtx 12
#define LEDrx 11
#define LEDr 3
#define LEDw 4
#define rx 5
#define tx 6
#define no_such_pin 255
#define PHOTOCELL A0
#define LEDSENSOR A1
#define SAMPLESIZE 5
//-------------------------
volatile int EIGHT_HZ = 0;
enum timer_action { WRITE, SAMPLE};
char* timer_actions[] = {"WRITE", "SAMPLE"};
volatile timer_action timerAction = WRITE;
volatile timer_action tempAction;
//-------------------------
byte transmit = 13; //define our transmit pin
byte data = 170; //value to transmit, binary 10101010
byte mask = 1; //our bitmask
int bitDelay = 100;

long t;

SoftwareSerial mySerial(rx, tx); 

void setup(){  
  setupTimer();
  for (int i =3; i<=13;i++){ pinMode(i,OUTPUT); digitalWrite(i,LOW);}
  pinMode(A0, INPUT);
  pinMode(LEDSENSOR, INPUT);
  digitalWrite(LEDr, HIGH);
  digitalWrite(LEDw, LOW);
  Serial.begin(9600);
  mySerial.begin(9600);
  t = millis();
}

// extract bits from message
// transmit bits by flashing an LED at some rate
// sample the sensor at 10 x rate
// assemble bits back into some message
// compare data sent to data recieved

while bytes
  read a single byte
    for each bit
      by reading the timer
        flash a bit
        read the bit 10 times
        write the bit

void loop()
{ 
  if (timerAction == SAMPLE) {
//    if (tempAction != timerAction) { tempAction = timerAction; Serial.println(timer_actions[timerAction]);}
    int val = getSensorReading(LEDSENSOR);
//    Serial.println(val);
    digitalWrite(LEDrx,val);
//    delay(200);
  }

  
  if (timerAction == WRITE) {
    if (millis() - t > 200){t = millis(); digitalWrite(LEDtx, digitalRead(LEDtx)^1); digitalWrite(13, digitalRead(13)^1);}
//    if (tempAction != timerAction) {tempAction = timerAction;  Serial.println(timer_actions[timerAction]);}
//    if (Serial.available()){
//      byte val = Serial.read();
//      //printBits(val);
//      mask = 00000001;
//    }
//    if (mask > 0){
//      if (data & mask){ // if bitwise AND resolves to true
//        digitalWrite(transmit,HIGH); 
//        Serial.print("1");
//      } else {     //if bitwise AND resolves to false
//        digitalWrite(transmit,LOW); 
//        Serial.print("0");
//      }
//      mask <<= 1;
//      delay(bitDelay);
//    }
  }
}


int getSensorReading(int sensorPin){
  long val=0;
  for (int i=0; i<SAMPLESIZE; i++){
    val += (analogRead(sensorPin));
    delay(1);
  }
  val = (val/SAMPLESIZE);
//  Serial.print(val);
//  Serial.print(" --> ");
  if (val < 300) {
    val = 0;
  } 
  else {
    val = 1;
  }
  return val;
}


void printBits(byte data){
  Serial.println();
  Serial.print(data,BIN); Serial.print(" : "); Serial.println(data);
  // iterate through bit mask
  for (mask = 00000001; mask>0; mask <<= 1) { 
    if (data & mask){ // if bitwise AND resolves to true
      digitalWrite(transmit,HIGH); 
//      Serial.print("1");
    } else {     //if bitwise AND resolves to false
      digitalWrite(transmit,LOW); 
//      Serial.print("0");
    }
    //delayMicroseconds(bitDelay);
    delay(bitDelay);
  }
}


//--------------------------------------------------------------------------

void setupTimer() {
  noInterrupts();
  // 8 bit timer
  TCCR2A = 0;              // set entire TCCR2A register to 0
  TCCR2B = 0;              // same for TCCR2B
  TCNT2  = 0;              //initialize counter value to 0
  // set compare match register for 8khz increments
  OCR2A = 249;             // = (16*10^6) / (1000 * 64) - 1 (must be <256)
  TCCR2A |= (1 << WGM21);  // turn on CTC mode
  TCCR2B |= (1 << CS12);   // | (1<<CS10);    // Set CS10 and CS12 bit for 64 prescaler
  TIMSK2 |= (1 << OCIE2A); // enable timer compare interrupt
  interrupts();
}  


ISR(TIMER2_COMPA_vect){
  if (EIGHT_HZ++ == (100)) { 
    EIGHT_HZ = 0;
        digitalWrite(LEDr, digitalRead(LEDr) ^ 1);
        digitalWrite(LEDw, digitalRead(LEDw) ^ 1);
    if (timerAction == SAMPLE){
      timerAction = WRITE;
    } 
    else {
      timerAction = SAMPLE;
    } 
  }
}

//--------------------------------------------------------------------------





