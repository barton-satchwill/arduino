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

    Motor();
    void setSpeed(int speed);
    int getSpeed();
    void forward(long time);
    void backward(long time);
    void halt();
    void halt(long time);
    void turnLeft(int degrees);
    void turnRight(int degrees);

    String toString();

private:
    enum directionType {AHEAD, REVERSE, LEFT_TURN, RIGHT_TURN, STOPPED};
    void motorOn();
    static int count;
    int id;
    directionType direction;
    int speed;
};
#endif

