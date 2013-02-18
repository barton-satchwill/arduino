/*
  Sonar.cpp - 
  Released into the public domain.
*/

#include <Sonar.h>

const int PING_PIN = 19;


Sonar::Sonar() {}  

// This is the code that runs the PING))) Sensor
float Sonar::ping () {
 long duration;
 pinMode(PING_PIN, OUTPUT);

 // clear
 digitalWrite(PING_PIN, LOW);
 delayMicroseconds(2);

 // ping
 digitalWrite(PING_PIN, HIGH);
 delayMicroseconds(5);
 digitalWrite(PING_PIN, LOW);

 // listen
 pinMode(PING_PIN, INPUT);
 duration = pulseIn(PING_PIN, HIGH);

 return duration;
}

float Sonar::range () {
 long duration = ping();

 float inches = microsecondsToInches(duration);
 Serial.print("range: ");
 Serial.print(inches);
 Serial.println("\"");
 return inches;
}

float Sonar::microsecondsToInches(long microseconds){
 return microseconds / 74.0 / 2.0;
}



