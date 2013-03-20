/*
  LightSensor.h -
  Released into the public domain.
*/

//
// 5v-----[photocell]-----+
//                        |
// A0---------------------+
//                        |
// gnd------/\/\/\/-------+
//            10k


#ifndef LightSensor_h
#define LightSensor_h

#define PHOTOCELL 0 //photocell analog in pin 0

#include <Arduino.h>

class LightSensor
{
public:
    LightSensor();
    void calibrate();
    long read();
    long maxBrightness();
    long minBrightness();
    String toString();

private:
    long minread;	// minimum value from calibration phase
    long maxread;	//maximum value from calibration phase
    long val;		//stores raw value from photocell
    int id;
    static int count;
};

#endif
