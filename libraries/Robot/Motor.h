/*
  Motor.h - 
  Released into the public domain.
*/

#ifndef Motor_h
#define Motor_h

#include <Arduino.h>
#include <AFMotor.h>

class Motor
{
  public:
  	float oneDegree;

    Motor();
	void setSpeed(int speed);
	void forward(long time);
	void backward(long time);
	void halt(long time);
	void turnLeft(int degrees);
	void turnRight(int degrees);

  private:
};

#endif
