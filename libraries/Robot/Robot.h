/*
  Robot.h -
  Released into the public domain.
*/

#ifndef Robot_h
#define Robot_h

#include <Arduino.h>
#include <AFMotor.h>
#include <Motor.h>
#include <Sonar.h>
#include <lightSensor.h>
#include <Servo.h>


class Robot
{
public:
    Robot(int speed);
    void init();
    void setSpeed(int theSpeed);
    void setAdjustedSpeed(int theSpeed);
    int getSpeed();
    String toString();
    void forward(long time);
    void backward(long time);
    void halt(long time);
    void turnLeft(int degrees);
    void turnRight(int degrees);
    void rangeScan(int scanAngle);
    void scan();
    float range();
    long brightness();
    int setSpeedForBrightness();
    int setSpeedForRange(int speed, int triggerDistance, int minDistance);
    void servoTest();


private:
    int id;
    static int count;
    int speed;
    Motor motor;
    Sonar sonar;
    // LightSensor lightSensor;
};

#endif
