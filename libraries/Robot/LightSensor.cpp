/*
  LightSensor.cpp - 
  Released into the public domain.
*/

#include <LightSensor.h>

LightSensor::LightSensor() {
}


long LightSensor::read() {
	return analogRead(PHOTOCELL);
}

long LightSensor::maxBrightness() {
	return maxread;
}

long LightSensor::minBrightness() {
	return minread;
}

// calibration loop to determine a rasonable range of light levels (minread to maxread)
// and map that to frequencies between minfreq and maxfreq
void LightSensor::calibrate() {
	maxread = 0;
	minread = 1000;

	Serial.print("Calibrating...");
	for (int i = 0; i< 500; i++) {  	// calibration loop runs for 5 seconds
		val = analogRead(PHOTOCELL);	// read photocell 

		// if (val > maxread) {        	// as the values climb, store the largest
		//     maxread = val;
		//   }
		//   if (val < minread) {        // as the values drop, store the smallest
		//     minread = val;
		//   }  

		maxread = max(maxread, val);
		minread = min(minread, val);
		delay(10);                  // reasonable delay
	} 
	Serial.println("done: " + String(minread) + " to " + String(maxread));
}  

