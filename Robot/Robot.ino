/*
  Released into the public domain.
 */

#include <AFMotor.h>
#include <Robot.h>
#include <Sonar.h>
#include <Servo.h>
#include <Logging.h>

/* TODO
-----------------------
 - ACTION_A runs continuously, even through a range scan.  This is good!
 - probably want to interrupt everything while backing up (move scan to ACTION_A?)
 - how do we know when to turn?
   - check for angle of greatest range? <-- try this one first of all  
     - scan
     - set turn angle
     - get turn angle
     - turn
     - clear turn angle
- do we need a normal ping rate and a high speed ping rate?
 - start a scan immediately after backing up
   - which means we need a way to force a scan to start...
 - how do we distinguish the straight-ahead range from the not-straight-ahead range?
   - do we need to?  yes!
   - redefine Robot::range() to only give distance straight ahead?
   - check to see if not scanning?
-----------------------
- need to stop after backing up, while checking for new range
- after backing up, check a wider range
 - a little fix: favour the ranges closest to straight ahead.
 r = range / (1+(abs(90-angle)/100))
 parabola: y =axx + bx + c
 - for heaven's sake, rename EIGHT_HZ
 
 - expose the scan frequency
*/

Robot *pRobot;
long lastPing;
enum action {ACTION_A, ACTION_B, ACTION_C, ACTION_D};
char* actionString[] = {"ACTION_A","              ACTION_B","ACTION_C","ACTION_D"};
volatile action robotAction = ACTION_A;
int r;
int turnangle = 0;
char* debugAction = actionString[ACTION_B];

void setup() {
  Serial.begin(9600);
  pRobot = new Robot(200);
    lastPing = millis();
    r = pRobot->rangeAhead();
}

void loop() {
  drive();
}

void drive() {
//  if (millis() > lastPing + 2050) 
  {
    lastPing = millis();
    r = pRobot->rangeAhead();
  }

  if (debugAction != actionString[robotAction]){
    debugAction = actionString[robotAction];
    Serial.println(debugAction);
  }

  switch (robotAction) {
    case ACTION_A: // drive forward
      if (r < 8 ) {
        robotAction = ACTION_B;
      } else if (pRobot->getTurnAngle() != 90) {
        robotAction = ACTION_C;
      } else {
        pRobot->rangeScan(90);
        pRobot->forward(1);
      }
      break;
  
    case ACTION_B: // backup!
      if (r < 10) {
        pRobot->backward(1);
      } else {
        pRobot->initiateRangeScan();
        pRobot->setSpeed(0);
        pRobot->clearTurnAngle();
        robotAction = ACTION_D;
      }
      break;

    case ACTION_C: // turn?
      turnangle = pRobot->getTurnAngle();
      if (turnangle < 90) {
        pRobot->turnLeft(90-turnangle);
      } else {
        pRobot->turnRight(turnangle-90);
      }
      pRobot->clearTurnAngle();
      robotAction = ACTION_A;
      break;
  
    case ACTION_D: // find out what where you are and what to do
      pRobot->rangeScan(170);
      if (pRobot->getTurnAngle() != 90){
        pRobot->setSpeed(200);
        robotAction = ACTION_C;
      }
      break;
  }
}


