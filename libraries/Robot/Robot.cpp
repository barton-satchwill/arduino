/*
  Robot.cpp - 
  Released into the public domain.
*/

#include <Robot.h>
#include <Motor.h>
#include <Sonar.h>
#include <LightSensor.h>  

  int turn_angle = 90;
  long max_distance = 0;
  boolean blocked = false;
  Sonar sonar;
  Motor motor;
  LightSensor lightSensor;


Robot::Robot(int theSpeed) {
  setSpeed(theSpeed);
  init();
  // lightSensor.calibrate();
}

void Robot::init(){
  Serial.println("initialising the robot...");
  lightSensor.calibrate();
  Serial.println("initialising the robot...done!");
}

void Robot::setSpeed(int speed) {
  this->speed = adjustSpeed();
  motor.setSpeed(this->speed);
}

int Robot::getSpeed() {
  return this->speed;
}

void Robot::forward(long time) { 
  motor.forward(time);
}

void Robot::backward(long time) { 
  motor.backward(time);
}

// This function stops all motors 
// (It is better to stop the motors before changing direction.)
void Robot::halt(long time) {
  motor.halt(time);
}

void Robot::turnLeft(int degrees) { 
  motor.turnLeft(degrees);
}

void Robot::turnRight(int degrees) {
  motor.turnRight(degrees);
}

void Robot::cylonScan(int scanAngle)  {
  long distance_ahead = range();

  turnRight(scanAngle);
  long distance_right = range();
  
  turnLeft(2*scanAngle);
  long distance_left = range();

  if (distance_ahead >= max(distance_right, distance_left)){
    turnRight(scanAngle);
  }

  if (distance_right >= max(distance_ahead, distance_left)){
    turnRight(2*scanAngle);
  }
}  

float Robot::range () { 
  return sonar.range(); 
}

long Robot::brightness() {
  return lightSensor.read();
}

long Robot::adjustSpeed(){
  double brightnessRange = lightSensor.maxBrightness() - lightSensor.minBrightness();
  double scaleFactor = 255.0 / brightnessRange; 
  long speed = int((lightSensor.read() - lightSensor.minBrightness()) * scaleFactor);
  Serial.println("speed = " + String(speed) + "(" + String(int(  (lightSensor.read() - lightSensor.minBrightness()) )) + ")");
  return speed;
}