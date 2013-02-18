/*
MotorUnit.h - 
Released into the public domain.
*/

#ifndef MotorUnit_h
#define MotorUnit_h

#include <Arduino.h>
#include <AFMotor.h>

class MotorUnit
{
public:
  MotorUnit(uint8_t motorNumber, uint8_t frequency);
  void setSpeed(int speed);
  void forward();
  void backward();
  void halt();
  static const int stoppedInterval;

private:
  enum {halted = 0, prograde = 1, retrograde = 2};
  AF_DCMotor* theMotor;
  int motion;
  int motorNumber;
  int id;    
  static int counter;    
  boolean wait();
};

#endif
