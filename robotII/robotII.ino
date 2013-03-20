/*
 Original code by Nick Brenn
 Modified by Marc de Vinck
 Make Projects Arduino-based 4WD robot
 http://makeprojects.com/Project/Build-your-own-Arduino-Controlled-Robot-/577/1
 */

#include <AFMotor.h>
#include <Robot.h>
#include <Sonar.h>
#include <Servo.h>
#include <Logging.h>

/* TODO
--------------

*/

#define DRIVE_CYCLE 20U
#define CHECK_RANGE_CYCLE 2000U

unsigned long drive_millis=0;
unsigned long checkRange_millis=0;

volatile boolean drive = true;
volatile boolean look = false;
volatile boolean backup = false;

Robot *pRobot;

void setup() {
    //  setupLEDbar();
    //  pinMode(6,OUTPUT);
    //  digitalWrite(6,LOW);
    Log.Init(LOG_LEVEL_DEBUG, 9600);
    Log.Info(CR"Here we go!"CR);
    // Serial.begin(9600);
    pRobot = new Robot(200);
    char buffer[500];
    String s = pRobot->toString();
    s.toCharArray(buffer,500);
    Log.Debug(buffer);
    Serial.println(pRobot->toString());
    Serial.println("here we go...");
}

void loop() {
//  test();
    if(cycleCheck(drive_millis, DRIVE_CYCLE)) {
        drive();
    }
    if(cycleCheck(checkRange_millis, CHECK_RANGE_CYCLE)) {
        checkRange();
    }
}

void drive() {
    Serial.println("drive");
    pRobot->forward(DRIVE_CYCLE);
    while(pRobot->range() < 10) {
        Serial.println("back up!");
        pRobot->backward(DRIVE_CYCLE);
        backup = true;
    }
    if(backup){
        Serial.println("finished backing up");
        backup = false;
        pRobot->halt();
        pRobot->rangeScan(170);
        // re-set the scan timer
        checkRange_millis = millis();
    }
}

void checkRange() {
    if (!backup) {
        Serial.println("looking...");
        int speed = pRobot->getSpeed();
        pRobot->setSpeed(20);
        pRobot->rangeScan(60);
        pRobot->setSpeed(speed);
        // re-set the scan timer
        checkRange_millis = millis();
    }
}


boolean cycleCheck(unsigned long& lastMillis, unsigned int cycle) {
    unsigned long currentMillis = millis();
    if( currentMillis > (lastMillis + cycle) ) {
        lastMillis = millis();
        return true;
    } else {
        return false;
    }
}

