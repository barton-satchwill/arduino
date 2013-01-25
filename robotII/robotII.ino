/*
Original code by Nick Brenn
Modified by Marc de Vinck
Make Projects Arduino-based 4WD robot
http://makeprojects.com/Project/Build-your-own-Arduino-Controlled-Robot-/577/1
*/
#include <AFMotor.h>

AF_DCMotor back_left(1, MOTOR12_8KHZ);
AF_DCMotor back_right(2, MOTOR12_8KHZ);
AF_DCMotor front_right(3, MOTOR12_8KHZ);
AF_DCMotor front_left(4, MOTOR12_8KHZ);
const int MAX_SPEED = 255;
const int PING_PIN = 19;
int turn_time = 1500;
int speed = 150;
long max_distance = 0;

void setup() {
  Serial.begin(9600); // set up Serial library at 9600 bps
  setSpeed(speed);
  Serial.println("here we go...");
}

void loop() { // This is the main program that will run over and over
//  test();
  run();
}

void test() {
  cylonScan();
}

void run()
{
  long duration = pulseIn(PING_PIN, HIGH);
  long inches = ping(); // Set the inches variable to the float returned by the ping() function.
  
  while (inches > 8){ // While the robot is 8 inches away from an object.
    inches = ping();
    forward(200); // Move the robot forward.
  }
  
  while (inches < 10){ // Until the robot is 10 inches away from the object, go backward.
    inches = ping();
    backward(200); // Move the robot backward.
  }
//  turnRight(900); // Once the robot is done moving backward, turn the robot right.
  cylonScan();
}


void cylonScan() 
{
  // take an initial measurement, assume it is the greatest distance
  int distance_ahead = ping();
  turnRight(turn_time);
  int distance_right = ping();
  turnLeft(2*turn_time);
  int distance_left = ping();
//  if (distance_left >= max(distance_ahead, distance_right)){}
  if (distance_ahead >= max(distance_left, distance_right)){turnRight(turn_time);}
  if (distance_right >= max(distance_ahead, distance_left)){turnRight(2*turn_time);}
  halt();
}  


void setSpeed(int speed)
{
  back_left.setSpeed(speed); // Set the speed for the motors (255 is the maximum)
  back_right.setSpeed(speed);
  front_right.setSpeed(speed);
  front_left.setSpeed(speed);
}

void forward(long time){ // This function moves the wheels forward
  Serial.println("forward");
  back_left.run(FORWARD);
  back_right.run(FORWARD);
  front_right.run(FORWARD);
  front_left.run(FORWARD);
  delay(time);
}

void backward(long time) { // This function moves the wheels backward
  Serial.println("backward");
  back_left.run(BACKWARD);
  back_right.run(BACKWARD);
  front_right.run(BACKWARD);
  front_left.run(BACKWARD);
  delay(time);
}

void halt() // This function stops each motor (It is better to stop the motors before changing direction.)
{
  Serial.println("halt");
  back_left.run(RELEASE);
  back_right.run(RELEASE);
  front_right.run(RELEASE);
  front_left.run(RELEASE);
  delay(500);
}

void turnLeft(long time){ // This function turns the robot right.
  Serial.println("turn left");
  setSpeed(MAX_SPEED);
  front_right.run(BACKWARD);
  back_right.run(BACKWARD);
  front_left.run(FORWARD);
  back_left.run(FORWARD);
  delay(time);
  setSpeed(speed);
}

void turnRight(long time){ // This function turns the robot right.
  Serial.println("turn right");
  setSpeed(MAX_SPEED);
  front_right.run(FORWARD);
  back_right.run(FORWARD);
  front_left.run(BACKWARD);
  back_left.run(BACKWARD);
  delay(time);
  setSpeed(speed);
}

float ping () // This is the code that runs the PING ))) Sensor
{
  long duration, inches;
  pinMode(PING_PIN, OUTPUT);
  digitalWrite(PING_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(PING_PIN, HIGH);
  delayMicroseconds(5);
  digitalWrite(PING_PIN, LOW);
  pinMode(PING_PIN, INPUT);
  duration = pulseIn(PING_PIN, HIGH);
  long microsecondsToInches(long microseconds);
  inches = microsecondsToInches(duration);
  Serial.println(String(inches) + "\"");
  return inches;
}

long microsecondsToInches(long microseconds){
//  Serial.print(microseconds);
//  Serial.println(" microseconds");
  return microseconds / 74 / 2;
}


//void loop() {
//  back_left.run(FORWARD);
//  delay(500);
//  back_left.run(RELEASE);
//
//  back_right.run(FORWARD);
//  delay(500);
//  back_right.run(RELEASE);
//
//  front_right.run(FORWARD);
//  delay(500);
//  front_right.run(RELEASE);
//
//  front_left.run(FORWARD);
//  delay(500);
//  front_left.run(RELEASE);

//  forward(1000);
//  turnRight(1000);
//}


