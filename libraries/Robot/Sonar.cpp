/*
  Sonar.cpp -
  Released into the public domain.
*/

#include <Sonar.h>

#define PING_PIN 19
int Sonar::count = 0;


Sonar::Sonar() {
    id = count++;
}

String Sonar::toString() {
    return "Sonar " + String(id);
}

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
    return random(2,20);
    // long duration = ping();

    // float inches = microsecondsToInches(duration);
    // // Serial.println("range: " + String((int)inches) + "\"");
    // return inches;
}

float Sonar::microsecondsToInches(long microseconds) {
    return microseconds / 74.0 / 2.0;
}



