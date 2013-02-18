/*
  Robot.cpp - 
  Released into the public domain.
*/

#include <Robot.h>

int Robot::count = 0;
int turn_angle = 90;
long max_distance = 0;
boolean blocked = false;

Robot::Robot(int speed) {
  Serial.println("robots!");
  id = count++;
  setSpeed(speed);
}

void Robot::setSpeed(int speed) {
  // this->speed = adjustSpeed();
  motor.setSpeed(speed);
}

int Robot::getSpeed() {
  return motor.getSpeed();
}

String Robot::toString() {
  return 
  String("------------------------------------\n") + 
  "Robot " + String(id) + ", with " + 
  "\n\t" + motor.toString() + 
  "\n\t" + sonar.toString() + 
  "\n\t" + lightSensor.toString() + 
  "\n------------------------------------"; 
}

void Robot::forward(long time) { 
  motor.forward(time);
}

void Robot::backward(long time) { 
  motor.backward(time);
}

// // This function stops all motors 
// // (It is better to stop the motors before changing direction.)
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
  // Serial.print("ahead ");
  // delay(4000);
  long distance_ahead = range();

  turnRight(scanAngle);
  // Serial.print("right ");
  // delay(4000);
  long distance_right = range();
  
  turnLeft(2*scanAngle);
  // Serial.print("left ");
  // delay(4000)  ;
  long distance_left = range();


  Serial.println(String(". . . . . . . .\n") +
    "ahead: " + String(distance_ahead) + 
    ",  right: " + String(distance_right) + 
    ", left: " + String(distance_left) );

  if (distance_ahead > max(distance_right, distance_left)) {
    Serial.println("keep going ahead");    
    turnRight(scanAngle);
  }
  else if (distance_right > max(distance_ahead, distance_left)) {
    Serial.println("take the right-hand path");    
    turnRight(2*scanAngle);
  }
  else if (distance_left >= max(distance_ahead, distance_right)) {
    Serial.println("take the left-hand path");
  }
}  

float Robot::range () { 
  return sonar.range(); 
}

// long Robot::brightness() {
//   return lightSensor.read();
// }

// long Robot::adjustSpeed(){
//   double brightnessRange = lightSensor.maxBrightness() - lightSensor.minBrightness();
//   double scaleFactor = 255.0 / brightnessRange; 
//   long speed = int((lightSensor.read() - lightSensor.minBrightness()) * scaleFactor);
//   // Serial.println("speed = " + String(speed) + "(" + String(int(  (lightSensor.read() - lightSensor.minBrightness()) )) + ")");
//   return speed;
// }