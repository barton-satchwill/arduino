/*
  Robot.cpp -
  Released into the public domain.
*/

#include <Robot.h>
  #include <Logging.h>

#define SERVO 9
#define RANGE 0
#define ANGLE 1

// we need to map around pin 9, which is used for the servo
int LEDbar[] = {3,4,5,6,7,8,10,11,12,13};
Servo servo;  // create servo object to control a servo
int Robot::count = 0;


Robot::Robot(int speed) {
    id = count++;
    setSpeed(speed);
    servo.attach(SERVO);  // attaches the servo to the servo object
    servo.write(90);
}


void Robot::setSpeed(int speed) {
    this->speed = speed;
    motor.setSpeed(this->speed);
}


void Robot::setAdjustedSpeed(int speed) {
    this->speed = setSpeedForBrightness();
    this->speed = setSpeedForRange(this->speed, 18,8);
    motor.setSpeed(this->speed);
}


int Robot::getSpeed() {
    return motor.getSpeed();
}


String Robot::toString() {
    return
        String("------------------------------------\n") +
        "Robot " + String(id) + ", with " +
        "\n\t" + motor.toString() +
        "\n\t" + sonar.toString() +
        // "\n\t" + lightSensor.toString() +
        "\n------------------------------------";
}


void Robot::forward(long time) {
    motor.forward(time);
}


void Robot::backward(long time) {
    motor.backward(time);
}

// // This function stops all motors
// // (It is better to stop the motors before changing direction.)
void Robot::halt(long time) {
    motor.halt(time);
}


void Robot::turnLeft(int degrees) {
    motor.turnLeft(degrees);
}


void Robot::turnRight(int degrees) {
    motor.turnRight(degrees);
}


float Robot::range () {
    return sonar.range();
}


int Robot::setSpeedForRange(int speed, int triggerDistance, int minDistance) {
    // double slope = 255.0 / double(triggerDistance - minDistance);
    // double spd = min(speed, max(0,(range()-minDistance)) * slope);
    // return spd;
    return speed;
}


void Robot::rangeScan(int scanAngle) {
    // power pro: 0.165 msec/60° ==> 2.75 msec / degree
    // hitec: 5 msec / degree
    float scanRate = 5;
    // int scanAngle = 90;
    int scanStep = 10;
    int max[] = {0,0}; // range, angle
    int range = 0;

    servo.write(scanAngle/2);
    delay((scanAngle/2)*scanRate);

    // for (int i=0; i<10; i++){
    //   digitalWrite(LEDbar[i], LOW);
    // }

    for (int angle = 90-scanAngle/2; angle <= 90+(scanAngle/2); angle +=scanStep) {
        servo.write(angle);
        delay(scanRate*scanStep);
        range = this->range();

        // Serial.println(String(angle) + ", " + String(range) + "\"");
        Log.Debug("%d, %d\"" CR, angle, range);
        if(range > max[0]) {
            max[RANGE] = range;
            max[ANGLE] = angle;
        }
    }
    servo.write(90);
    delay((scanAngle/2)*scanRate);

    // Serial.println("max is at angle " + String(max[ANGLE]) + " range " + String (max[RANGE]) + "\"");

    if(max[ANGLE] <= 90) {
        turnLeft(90-max[ANGLE]);
    } else {
        turnRight(max[ANGLE]-90);
    }
}


void Robot::servoTest() {
    // power pro: 0.165 msec/60° ==> 2.75 msec / degree
    // hitec: 5 msec / degree
    float scanRate = 5;
    int scanAngle = 90;
    int scanStep = 10;
    int max[] = {0,0}; // range, angle
    int range = 0;

    servo.write(scanAngle/2);
    delay((scanAngle/2)*scanRate);

    // for (int i=0; i<10; i++){
    //   digitalWrite(LEDbar[i], LOW);
    // }

    for (int angle = scanAngle/2; angle <= (scanAngle/2) + scanAngle; angle +=scanStep) {
        servo.write(angle);
        int i = map(angle, 45, 135, 0, 9);
        range = this->range();
        // digitalWrite(LEDbar[i-1], LOW);
        // digitalWrite(LEDbar[i], HIGH);
        delay(scanRate*scanStep);

        if(range > max[0]) {
            max[RANGE] = range;
            max[ANGLE] = angle;
        }
    }
    // for (int i=0; i<scanStep; i++){
    //   digitalWrite(LEDbar[i], LOW);
    // }
    // digitalWrite(LEDbar[map(max[ANGLE],45,135,0,9)], HIGH);
    servo.write(90);
    delay((scanAngle/2)*scanRate);

    Serial.println("max is at angle " + String(max[ANGLE]) + " (index "+ String(map(max[1],45,135,0,9)) + ") range " + String (max[RANGE]));

    if(max[ANGLE] <= 90) {
        turnLeft(90-max[ANGLE]);
    } else {
        turnRight(max[ANGLE]-90);
    }
}

// ------- light sensor ----------

// long Robot::brightness() {
//   return lightSensor.read();
// }

int Robot::setSpeedForBrightness() {
    // double brightnessRange = lightSensor.maxBrightness() - lightSensor.minBrightness();
    // double scaleFactor = 255.0 / brightnessRange;
    // int speed = int((lightSensor.read() - lightSensor.minBrightness()) * scaleFactor);
    // Serial.println("brighness speed = " + String(speed) + "(" + String(int(  (lightSensor.read() - lightSensor.minBrightness()) )) + ")");
    return speed;
}

