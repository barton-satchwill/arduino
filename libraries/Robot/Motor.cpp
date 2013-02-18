/*
  Motor.cpp - 
  Released into the public domain.
*/

#include <Motor.h>
#include <Sonar.h>

int Motor::count = 0;
const int MAX_SPEED = 255;
const int MIN_SPEED = 75;
const int HALT_TIME = 3000;
const float oneDegree = 2338.0/360.0;


AF_DCMotor back_left(1, MOTOR12_8KHZ);
AF_DCMotor back_right(2, MOTOR12_8KHZ);
AF_DCMotor front_right(3, MOTOR12_8KHZ);
AF_DCMotor front_left(4, MOTOR12_8KHZ);

Motor::Motor() {
  id = Motor::count++;
  motorOn();
}

// turn on the motors
void Motor::motorOn(){
  setSpeed(200);
  halt();
  direction = AHEAD;  
}

String Motor::toString() {
  return "Motor " + String(id) + ", speed " + String(this->speed);
}

void Motor::setSpeed(int theSpeed) {
  speed = min(theSpeed, MAX_SPEED);
  speed = max(speed, MIN_SPEED);
  back_left.setSpeed(speed); 
  back_right.setSpeed(speed);
  front_right.setSpeed(speed);
  front_left.setSpeed(speed);
}

int Motor::getSpeed() {
  return this->speed;
}

void Motor::forward(long time) { 
  // Serial.println("forward");
  if (direction != AHEAD) {
    halt();
  }
  direction = AHEAD;
  back_left.run(FORWARD);
  back_right.run(FORWARD);
  front_right.run(FORWARD);
  front_left.run(FORWARD);
  delay(time);
}

void Motor::backward(long time) { 
  // Serial.println("backward");
  if (direction != REVERSE) {
    halt();
  }
  direction = REVERSE;
  back_left.run(BACKWARD);
  back_right.run(BACKWARD);
  front_right.run(BACKWARD);
  front_left.run(BACKWARD);
  delay(time);
}

// This function stops each motor (It is better to stop the motors before changing direction.)
// TODO: either use the 'time' parameter, or don't!
void Motor::halt() {
  halt(HALT_TIME);
}

void Motor::halt(long time) {
  if (direction != STOPPED){
    direction = STOPPED;
    // Serial.println("halt");
    back_left.run(RELEASE);
    back_right.run(RELEASE);
    front_right.run(RELEASE);
    front_left.run(RELEASE);
    delay(time);
  }
}

void Motor::turnRight(int degrees) { 
  if (direction != RIGHT_TURN) {
    halt();
  }
  direction = RIGHT_TURN;
  long turnTime = degrees * oneDegree;
  // Serial.println("turn right " + String(degrees) + " degrees in " + String(turnTime) + " milliseconds");
  int originalSpeed = speed;
  setSpeed(MAX_SPEED);
  front_right.run(BACKWARD);
  back_right.run(BACKWARD);
  front_left.run(FORWARD);
  back_left.run(FORWARD);
  delay(degrees * oneDegree);
  setSpeed(originalSpeed);
}

void Motor::turnLeft(int degrees) {
  if (direction != LEFT_TURN) {
    halt();
  }
  direction = LEFT_TURN;
  long turnTime = degrees * oneDegree;
  // Serial.println("turn left " + String(degrees) + " degrees in " + String(turnTime) + " milliseconds");
  int originalSpeed = speed;
  setSpeed(MAX_SPEED);
  front_right.run(FORWARD);
  back_right.run(FORWARD);
  front_left.run(BACKWARD);
  back_left.run(BACKWARD);
  delay(degrees * oneDegree);
  setSpeed(originalSpeed);
}

// void Motor::setSpeed(int speed) {
//   double slope = 255.000/4000.0000;
//   double start = millis();

//   while (speed > 120) {
//     speed = -slope*(millis()-start) + 255;
//       theMotor->setSpeed(speed);
//   }
// }