/*
  LightSensor.h - 
  Released into the public domain.
*/

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
    


// long val = 0;        //stores raw value from photocell
// long maxread = 0;    
// long minread = 1000;  
// int i = 0;            // loop dummy variable
// double factor = 0;    // scaling factor for calibration

};

#endif
