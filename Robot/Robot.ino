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

Robot *pRobot;

void setup() {
  Serial.begin(9600);
  pRobot = new Robot(200);
}

void loop() {
  drive();
  int range = pRobot->rangeScan(60);
  Serial.println("turn angle is "+ String(range));
}

void drive() {
//  Serial.print("range is " );
//  Serial.println(pRobot->range());
}



