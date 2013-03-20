/*
LightSensor.cpp -
Released into the public domain.
*/

#include <LightSensor.h>

int LightSensor::count = 0;
const int LED_PIN = 13;


LightSensor::LightSensor() {
    id = count++;
    pinMode(PHOTOCELL, OUTPUT);
    pinMode(LED_PIN, OUTPUT);
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


void blink() {
    if (digitalRead(LED_PIN) == LOW)
        digitalWrite(LED_PIN, HIGH);
    else
        digitalWrite(LED_PIN, LOW);
}

// calibration loop to determine a reasonable range of light levels (minread to maxread)
// and map that to frequencies between minfreq and maxfreq
void LightSensor::calibrate() {
    maxread = 0;
    minread = 1000;

    Serial.print("Calibrating " + toString() + "...");
    digitalWrite(LED_PIN, HIGH);
    for (int i = 0; i< 500; i++) {  	// calibration loop runs for 5 seconds
        if ((i % 25) == 0) {
            blink();
        }
        val = analogRead(PHOTOCELL);	// read photocell
        maxread = max(maxread, val);
        minread = min(minread, val);
        delay(10);                  // reasonable delay
    }
    Serial.println("done: " + String(minread) + " to " + String(maxread));
    digitalWrite(LED_PIN, LOW);
}

String LightSensor::toString() {
    return "LightSensor " + String(id);
}

