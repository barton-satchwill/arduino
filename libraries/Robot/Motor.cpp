/*
  Motor.cpp - 
  Released into the public domain.
*/

#include <Motor.h>
#include <Sonar.h>

  AF_DCMotor back_left(1, MOTOR12_8KHZ);
  AF_DCMotor back_right(2, MOTOR12_8KHZ);
  AF_DCMotor front_right(3, MOTOR12_8KHZ);
  AF_DCMotor front_left(4, MOTOR12_8KHZ);
  const int MAX_SPEED = 255;
  int speed = MAX_SPEED;
  float oneDegree; // = 4600.0/360.0;


Motor::Motor() {
  oneDegree = 4600.0/360.0;
}

void Motor::setSpeed(int theSpeed) {
  speed = theSpeed;
  back_left.setSpeed(speed); 
  back_right.setSpeed(speed);
  front_right.setSpeed(speed);
  front_left.setSpeed(speed);
}

void Motor::forward(long time) { 
  Serial.println("forward");
  back_left.run(FORWARD);
  back_right.run(FORWARD);
  front_right.run(FORWARD);
  front_left.run(FORWARD);
  delay(time);
}

void Motor::backward(long time) { 
  Serial.println("backward");
  back_left.run(BACKWARD);
  back_right.run(BACKWARD);
  front_right.run(BACKWARD);
  front_left.run(BACKWARD);
  delay(time);
}

// This function stops each motor (It is better to stop the motors before changing direction.)
void Motor::halt(long time) {
  Serial.println("halt");
  back_left.run(RELEASE);
  back_right.run(RELEASE);
  front_right.run(RELEASE);
  front_left.run(RELEASE);
  delay(time);
}

void Motor::turnLeft(int degrees) { 
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

void Motor::turnRight(int degrees) {
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

