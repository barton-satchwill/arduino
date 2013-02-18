/*
 Original code by Nick Brenn
 Modified by Marc de Vinck
 Make Projects Arduino-based 4WD robot
 http://makeprojects.com/Project/Build-your-own-Arduino-Controlled-Robot-/577/1
*/

#include <Robot.h>
#include <AFMotor.h>

Robot *pRobot;

void setup() {
  Serial.begin(9600); 
  delay(1000);
  pRobot = new Robot(120);
  Serial.println(pRobot->toString());
  Serial.println("here we go...");
}

void loop() { 
	int i = 1;	
	while (pRobot->range() > 8){
		// take a look around every 20 * 200 milliseconds
		if ((i++ % 40) == 0){
			pRobot->cylonScan(45);
		}
		pRobot->setAdjustedSpeed(120);
		pRobot->forward(100);
	}
	while (pRobot->range() < 10){
		pRobot->backward(100);
	}
	pRobot->cylonScan(90);
}
