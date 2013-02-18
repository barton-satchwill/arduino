/*
  Robot.cpp - 
  Released into the public domain.
*/

#include <Robot.h>

int Robot::count = 0;
int turn_angle = 90;
long max_distance = 0;
boolean blocked = false;
double r = 0.0;
long lastping = 0;



void printDouble(double val, byte precision){
  // prints val with number of decimal places determine by precision
  // precision is a number from 0 to 6 indicating the desired decimial places
  // example: printDouble( 3.1415, 2); // prints 3.14 (two decimal places)
   
  Serial.print (int(val));  //prints the int part
  if( precision > 0) {
    Serial.print("."); // print the decimal point
    unsigned long frac;
    unsigned long mult = 1;
    byte padding = precision -1;
    while(precision--)
    mult *=10;
     
    if(val >= 0)
      frac = (val - int(val)) * mult;
    else
      frac = (int(val)- val ) * mult;
    unsigned long frac1 = frac;
    while( frac1 /= 10 )
      padding--;
    while(padding--)
      Serial.print("0");
    Serial.println(frac,DEC) ;
  }
}




Robot::Robot(int speed) {
  // Serial.println("robots!");
  id = count++;
  setSpeed(speed);
}

void Robot::setSpeed(int speed) {
  this->speed = speed;
  motor.setSpeed(this->speed);
}

void Robot::setAdjustedSpeed(int speed) {
  this->speed = setSpeedForBrightness();
  this->speed = setSpeedForRange(this->speed, 18,8);
  motor.setSpeed(this->speed);
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
  // "\n\t" + lightSensor.toString() + 
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
  long distance_ahead = range();

  turnRight(scanAngle);
  long distance_right = range();
  
  turnLeft(2*scanAngle);
  long distance_left = range();

  // Serial.println(String(". . . . . . . .\n") +
  //   "ahead: " + String(distance_ahead) + 
  //   ",  right: " + String(distance_right) + 
  //   ", left: " + String(distance_left) );

  if (distance_ahead > max(distance_right, distance_left)) {
    turnRight(scanAngle);
  }
  else if (distance_right > max(distance_ahead, distance_left)) {
    turnRight(2*scanAngle);
  }
  else if (distance_left >= max(distance_ahead, distance_right)) {
  }
}  

float Robot::range () { 
  // if(millis() - lastping > 100) {
    // lastping = millis();
    r = sonar.range();
  // }
  // else {Serial.print("!");}
  printDouble(r,1);
  return r; 
}

long Robot::brightness() {
  return lightSensor.read();
}

int Robot::setSpeedForBrightness(){
  double brightnessRange = lightSensor.maxBrightness() - lightSensor.minBrightness();
  double scaleFactor = 255.0 / brightnessRange; 
  int speed = int((lightSensor.read() - lightSensor.minBrightness()) * scaleFactor);
  // Serial.println("brighness speed = " + String(speed) + "(" + String(int(  (lightSensor.read() - lightSensor.minBrightness()) )) + ")");
  return speed;
}


int Robot::setSpeedForRange(int speed, int triggerDistance, int minDistance) {
  double slope = 255.0 / double(triggerDistance - minDistance);
  double spd = min(speed, max(0,(range()-minDistance)) * slope);
  return spd;
}