/* 
Released to the public domain
*/

#include <Servo.h>

// Diyode CodeShield Base Script
#define ENCODER_A 14
#define ENCODER_B 15
#define ENCODER_PORT PINC
#define SWITCH 13
#define BUTTON 12
#define RGB_RED 11
#define RGB_GREEN 10
#define RGB_BLUE 9
#define LED 6
#define SERVO 9 //5
#define PIEZO 3
#define RELAY 2
#define POT 2
#define HALL 3
#define THERMISTOR 4
#define PHOTOCELL 5


#define CYCLE1 500U
#define CYCLE2 1000U
#define PULSE 50U
#define PULSEON 3500U
#define PULSEOFF 1000U


// 0.10 sec/60°

unsigned long led1LastMillis = 0;
unsigned long led2LastMillis = 0;
unsigned long pulseLastMillis = 0;
unsigned long pulseOnLastMillis = 0;
unsigned long pulseOffLastMillis = 0;
boolean led1State = false;
boolean led2State = false;
boolean doPulse = false;
int i = 0;
int mod = 10;
unsigned int cycle1 = CYCLE1;
int servoAngle = 0;
int step = 1;
Servo myservo; 

void setup()
{
  myservo.attach(SERVO);
//  servoCheck();
  Serial.begin(9600);
  pinMode(RGB_GREEN, OUTPUT);
  pinMode(LED, OUTPUT);
}

void loop()
{
  servoTest();
  
//  if(cycleCheck(led1LastMillis, cycle1)) {
//    if (++i % mod == 0) {
//      if (cycle1 == PULSE) {
//        cycle1 = CYCLE1;
//        mod=10;
//      } else {
//        cycle1 = PULSE;
//        mod=CYCLE1/PULSE;
//      }
//    }
//    digitalWrite(RGB_GREEN, led1State);
//    led1State = !led1State;
//  }
//
//  if(cycleCheck(led2LastMillis, CYCLE2)) {
//    digitalWrite(LED, led2State);
//    led2State = !led2State;
////  servoScan();
//    
//  }
}


boolean cycleCheck(unsigned long& lastMillis, unsigned int cycle) 
{
//  Serial.println(cycle);
  unsigned long currentMillis = millis();
//  if(currentMillis - *lastMillis >= cycle) {
  if( currentMillis > (lastMillis + cycle) ) {
    lastMillis = currentMillis;
    return true;
  } else {
    return false;
  }
}

boolean x(unsigned long& time, unsigned int cycle) {
  unsigned long currentTime = millis();
  if (time <= currentTime) {
    time += cycle;
    return true;
  }
  return false;
}

void servoCheck() {
  myservo.write(0);
  delay(5000);
  myservo.write(90);
  delay(5000);
  myservo.write(180);
  delay(5000);
  myservo.write(90);
  delay(5000);
//  myservo.detach();
}    

void servoSweep() {
  if (servoAngle == 45) { step = 1; }
  if (servoAngle == 135){ step = -1;} 
  servoAngle+=step;
  myservo.write(servoAngle);
}  

void servoScan() {
  int DELAY=400;
  myservo.write(45);
  delay(DELAY);
  for (int angle = 45; angle < 135; angle++){
    myservo.write(angle);
    delay(5);
  }
  myservo.write(90);
  delay(DELAY);
}


void servoTest2(){
  // power pro: 0.165 msec/60° ==> 2.75 msec / degree
  // hitec: 5 msec / degree
  int scanAngle = 90;
  int scanTime = 2.75*scanAngle;
  myservo.write(scanAngle/2);
  delay((scanAngle/2)*2.75);
  myservo.write((scanAngle/2) + scanAngle);
  delay(scanTime);  
  myservo.write(90);
  delay(3000);
}  

void servoTest(){
  // power pro: 0.165 msec/60° ==> 2.75 msec / degree
  // hitec: 5 msec / degree
  float scanRate = 2.75;
  int scanAngle = 90;
  int scanTime = scanRate*scanAngle;
  myservo.write(scanAngle/2);
  delay((scanAngle/2)*2.75);
  delay(5000);
  for (int angle = scanAngle/2; angle < (scanAngle/2) + scanAngle; angle +=10) {
    myservo.write(angle);
    delay(scanRate*10);
  }
//  delay(5000);
   myservo.write(90);
  delay(5000);
}
