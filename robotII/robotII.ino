/*
Original code by Nick Brenn
 Modified by Marc de Vinck
 Make Projects Arduino-based 4WD robot
 http://makeprojects.com/Project/Build-your-own-Arduino-Controlled-Robot-/577/1
 */

#include <AFMotor.h>
#include <Robot.h>
//#include <Sonar.h>

Robot robot(100);

void setup() {
  Serial.begin(9600); // set up Serial library at 9600 bps
  Serial.println("here we go...");
}

void loop() { // This is the main program that will run over and over
//  test();
  run();
}

void test() {
//  cylonScan();
  robot.turnLeft(30);
  robot.halt(500);
}


void run()
{
  while (robot.range() > 8){ 
    robot.forward(200); 
  }

  while (robot.range() < 10){ 
    robot.backward(200); 
  }

  robot.cylonScan();
}


