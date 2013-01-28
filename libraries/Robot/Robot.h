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
	void setSpeed(int speed);
	void forward(long time);
	void backward(long time);
	void halt(long time);
	void turnLeft(int degrees);
	void turnRight(int degrees);
	void cylonScan();
	float range();
	// long microsecondsToInches(long microseconds);

  private:
};

#endif
