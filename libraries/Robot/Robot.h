/*
  Robot.h - 
  Released into the public domain.
*/

#ifndef Robot_h
#define Robot_h

#include <Arduino.h>
#include <AFMotor.h>

class Robot
{
  public:
  	float oneDegree;

    Robot(int speed);
    void init();
	void setSpeed(int theSpeed);
	int getSpeed();
	void forward(long time);
	void backward(long time);
	void halt(long time);
	void turnLeft(int degrees);
	void turnRight(int degrees);
	void cylonScan(int scanAngle);
	float range();
	long brightness();
	long adjustSpeed();

  private:
  	int speed;
};

#endif
