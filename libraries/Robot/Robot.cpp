/*
  Robot.cpp - 
  Released into the public domain.
*/

#include <Robot.h>
#include <Sonar.h>

  AF_DCMotor back_left(1, MOTOR12_8KHZ);
  AF_DCMotor back_right(2, MOTOR12_8KHZ);
  AF_DCMotor front_right(3, MOTOR12_8KHZ);
  AF_DCMotor front_left(4, MOTOR12_8KHZ);
  const int MAX_SPEED = 255;
  int turn_angle = 90;
  int speed = MAX_SPEED;
  long max_distance = 0;
  boolean blocked = false;
  float oneDegree; // = 4600.0/360.0;
  Sonar sonar;


Robot::Robot(int theSpeed) {
  // oneDegree = 5733.0/360.0;
  oneDegree = 4600.0/360.0;
  setSpeed(theSpeed);
  // pulseIn(PING_PIN, HIGH);
}

void Robot::setSpeed(int theSpeed) {
  speed = theSpeed;
  back_left.setSpeed(speed); 
  back_right.setSpeed(speed);
  front_right.setSpeed(speed);
  front_left.setSpeed(speed);
}

void Robot::forward(long time) { 
  Serial.println("forward");
  back_left.run(FORWARD);
  back_right.run(FORWARD);
  front_right.run(FORWARD);
  front_left.run(FORWARD);
  delay(time);
}

void Robot::backward(long time) { 
  Serial.println("backward");
  back_left.run(BACKWARD);
  back_right.run(BACKWARD);
  front_right.run(BACKWARD);
  front_left.run(BACKWARD);
  delay(time);
}

// This function stops each motor (It is better to stop the motors before changing direction.)
void Robot::halt(long time) {
  Serial.println("halt");
  back_left.run(RELEASE);
  back_right.run(RELEASE);
  front_right.run(RELEASE);
  front_left.run(RELEASE);
  delay(time);
}

void Robot::turnLeft(int degrees) { 
  Serial.print("turn left " + String(degrees) + " degrees");
  int originalSpeed = speed;
  setSpeed(MAX_SPEED);
  front_right.run(BACKWARD);
  back_right.run(BACKWARD);
  front_left.run(FORWARD);
  back_left.run(FORWARD);
  delay(degrees * oneDegree);
  setSpeed(originalSpeed);
}

void Robot::turnRight(int degrees) {
  Serial.println("turn right " + String(degrees) + " degrees");
  int originalSpeed = speed;
  setSpeed(MAX_SPEED);
  front_right.run(FORWARD);
  back_right.run(FORWARD);
  front_left.run(BACKWARD);
  back_left.run(BACKWARD);
  delay(degrees * oneDegree);
  setSpeed(originalSpeed);
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



