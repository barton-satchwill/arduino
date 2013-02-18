/*
  Motor.cpp - 
  Released into the public domain.
*/

#include <Motor.h>
#include <Sonar.h>
#include <MotorUnit.h>

MotorUnit back_left(1, MOTOR12_8KHZ);
MotorUnit back_right(2, MOTOR12_8KHZ);
MotorUnit front_right(3, MOTOR12_8KHZ);
MotorUnit front_left(4, MOTOR12_8KHZ);
const int MAX_SPEED = 255;
const int MIN_SPEED = 70;
int xspeed = MAX_SPEED;
float oneDegree; // = 4600.0/360.0;

Motor::Motor() {
  oneDegree = 2338.0/360.0;
}

void Motor::setSpeed(int theSpeed) {
  xspeed = min(theSpeed, MAX_SPEED);
  xspeed = max(xspeed, MIN_SPEED);
  back_left.setSpeed(xspeed); 
  back_right.setSpeed(xspeed);
  front_right.setSpeed(xspeed);
  front_left.setSpeed(xspeed);
}

void Motor::forward(long time) { 
  Serial.println("forward");
  back_left.forward();
  back_right.forward();
  front_right.forward();
  front_left.forward();
  // delay(time);
}

void Motor::backward(long time) { 
  Serial.println("backward");
  back_left.backward();
  back_right.backward();
  front_right.backward();
  front_left.backward();
  // delay(time);
}

// This function stops each motor (It is better to stop the motors before changing direction.)
void Motor::halt(long time) {
  Serial.println("halt");
  back_left.halt();
  back_right.halt();
  front_right.halt();
  front_left.halt();
  delay(MotorUnit::stoppedInterval);
}

void Motor::turnRight(int degrees) { 
  halt(1);
  long turnTime = degrees * oneDegree;
  Serial.println("turn right " + String(degrees) + " degrees in " + String(turnTime) + " milliseconds");
  int originalSpeed = speed;
  setSpeed(MAX_SPEED);
  front_right.backward();
  back_right.backward();
  front_left.forward();
  back_left.forward();
  delay(degrees * oneDegree);
  setSpeed(originalSpeed);
  halt(1);
}

void Motor::turnLeft(int degrees) {
  long turnTime = degrees * oneDegree;
  halt(1);
  Serial.println("turn left " + String(degrees) + " degrees in " + String(turnTime) + " milliseconds");
  int originalSpeed = speed;
  setSpeed(MAX_SPEED);
  front_right.forward();
  back_right.forward();
  front_left.backward();
  back_left.backward();
  delay(degrees * oneDegree);
  setSpeed(originalSpeed);
  halt(1);
}

