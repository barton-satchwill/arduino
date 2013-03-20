/*
  Sonar.h -
  Released into the public domain.
*/

#ifndef Sonar_h
#define Sonar_h

#include <Arduino.h>
// #include <AFMotor.h>

class Sonar
{
public:
    Sonar();

    float ping();
    float range();
    String toString();

private:
    int id;
    static int count;

    float microsecondsToInches(long microseconds);
};

#endif
