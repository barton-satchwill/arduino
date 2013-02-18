/*
LightSensor.cpp - 
Released into the public domain.
*/

#include <LightSensor.h>

int LightSensor::count = 0;


LightSensor::LightSensor() {
	id = count++;
	Serial.println("light sensors!");
	pinMode(PHOTOCELL, OUTPUT);
	calibrate();
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

// calibration loop to determine a reasonable range of light levels (minread to maxread)
// and map that to frequencies between minfreq and maxfreq
void LightSensor::calibrate() {
	maxread = 0;
	minread = 1000;

	Serial.print("Calibrating " + toString() + "...");
	for (int i = 0; i< 500; i++) {  	// calibration loop runs for 5 seconds
		val = analogRead(PHOTOCELL);	// read photocell 
		maxread = max(maxread, val);
		minread = min(minread, val);
		delay(10);                  // reasonable delay
	} 
	Serial.println("done: " + String(minread) + " to " + String(maxread));
}  

String LightSensor::toString() {
	return "LightSensor " + String(id);
} 

