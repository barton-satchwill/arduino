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
    void setSpeed(int theSpeed);
    String toString();
    void forward(long time);
    void backward(long time);
    void halt(long time);
    void turnLeft(int degrees);
    void turnRight(int degrees);
    void rangeScan(int scanAngle);
    float range();
    float rangeAhead();
    int getTurnAngle();
    void clearTurnAngle();
    void initiateRangeScan();


private:
    void moveServo(int angle);
    boolean servoReady();
    boolean scanComplete();
    int id;
    static int count;
    int speed;
    Motor motor;
    Sonar sonar;
    // LightSensor lightSensor;
};

#endif
