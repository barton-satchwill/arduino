/*
  Sonar.h -
  Released into the public domain.
*/

#ifndef Sonar_h
#define Sonar_h

#include <Arduino.h>

class Sonar
{
public:
    enum Rate {FAST = 30, SLOW  = 600};

    Sonar();

    float ping();
    float range();
    String toString();
    void setRate(Rate rate);


private:
    int id;
    static int count;
    long pingTime;
    long duration;

    float microsecondsToInches(long microseconds);
};

#endif
