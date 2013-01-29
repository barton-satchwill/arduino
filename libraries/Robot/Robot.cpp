/*
  Robot.cpp - 
  Released into the public domain.
*/

#include <Robot.h>
#include <Motor.h>
#include <Sonar.h>

  int turn_angle = 90;
  long max_distance = 0;
  boolean blocked = false;
  Sonar sonar;
  Motor motor;


Robot::Robot(int theSpeed) {
  setSpeed(theSpeed);
}

void Robot::setSpeed(int theSpeed) {
  motor.setSpeed(theSpeed);
}

void Robot::forward(long time) { 
  motor.forward(time);
}

void Robot::backward(long time) { 
  motor.backward(time);
}

// This function stops each motor (It is better to stop the motors before changing direction.)
void Robot::halt(long time) {
  motor.halt(time);
}

void Robot::turnLeft(int degrees) { 
  motor.turnLeft(degrees);
}

void Robot::turnRight(int degrees) {
  motor.turnRight(degrees);
}

void Robot::cylonScan() 
{
  int motorPause = 200;
  // take an initial measurement, assume it is the greatest distance
  long distance_ahead = range();
  halt(motorPause);
  turnRight(turn_angle);

  long distance_right = range();
  halt(motorPause);
  turnLeft(2*turn_angle);
  long distance_left = range();
  //  if (distance_left >= max(distance_ahead, distance_right)){}
  //  if (distance_ahead >= max(distance_left, distance_right)){turnRight(turn_angle);}
  if (distance_right >= max(distance_ahead, distance_left)){
    halt(motorPause);
    turnRight(2*turn_angle);
  }
  halt(motorPause);
}  

float Robot::range () { 
  return sonar.range(); 
}

