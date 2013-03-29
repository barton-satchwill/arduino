/*
  Robot.cpp -
  Released into the public domain.
*/

#include <Robot.h>
#include <Logging.h>

// diyode shield
#define RGB_RED 11
#define RGB_GREEN 10
#define RGB_BLUE 9
#define LED 6
#define SERVO 5

//motor control shield
// #define SERVO 9
#define RANGE 0
#define ANGLE 1

enum action {INIT, MOVE, PING, CHECK, DONE, F};
char* actions[] = {"INIT", "MOVE", "PING", "CHECK", "DONE", "F"};
volatile action sonarAction = F;
volatile int EIGHT_HZ = 3500;

volatile int angle;
int scanDirection = 10;
int maxRange;
int sonarRange;
long moveDelay = 30;
int scanRate = 5;
volatile long wait;
volatile long start;

Servo servo;  // create servo object to control a servo
int Robot::count = 0;




void setupTimer() {
  noInterrupts();
  // 8 bit timer
  TCCR2A = 0;               // set entire TCCR2A register to 0
  TCCR2B = 0;               // same for TCCR2B
  TCNT2  = 0;               //initialize counter value to 0
  // set compare match register for 4khz increments
  OCR2A = 249;              // = (16*10^6) / (1000 * 64) - 1 (must be <256)
  TCCR2A |= (1 << WGM21);   // turn on CTC mode
  TCCR2B |= (1 << CS12) | (1<<CS10);    // Set CS10 and CS12 bit for 64 prescaler
  TIMSK2 |= (1 << OCIE2A);  // enable timer compare interrupt
  interrupts();
}  

ISR(TIMER2_COMPA_vect){
  if (EIGHT_HZ++ == (4 * 1000)) {  // every 4 seconds
    Serial.println((millis()-start)/1000);
    EIGHT_HZ = 0;
    digitalWrite(LED, LOW); //digitalRead(LED) ^ 1);
    sonarAction = INIT;
  }
}




Robot::Robot(int speed) {
    id = count++;
    setSpeed(speed);
    servo.attach(SERVO);  // attaches the servo to the servo object
    servo.write(90);
    setupTimer();
    start = millis();
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


void Robot::scan() {
  int r;
  int t = (millis()-start)/1000;
  // Serial.println(actions[sonarAction]);
  switch (sonarAction) {
    case INIT: // move to inital position
      r = 0;
      sonarRange = 0;
      angle = 0;
      scanDirection = +10;
      moveServo(angle);
      sonarAction = MOVE;
      break;
      
    case MOVE: // move a step
      digitalWrite(RGB_BLUE, HIGH);
      angle = angle + scanDirection;
      angle = min(angle, 170);
      angle = max(angle, 10);
      moveServo(angle);
      sonarAction = PING;
      break;

    case PING: // take the range
      if (servoReady()){
        // r = range();
        Serial.print(" : range at angle " + String(angle) + " is " + String(r) + " at t = " + String(t) + "\n");
        sonarRange = max(sonarRange, r);
        sonarAction = CHECK;
        digitalWrite(RGB_BLUE, LOW);
      }
      break;

    case CHECK: // check for complete scan
      if (scanComplete()) {
        moveServo(90);
        sonarAction = DONE;        
      } else {
        sonarAction = MOVE;
      }
      break;

    case DONE: //scan complete
      break;
    case F:
      break;
    default:
      break;
  }
}

void Robot::moveServo(int angle){
  if (servoReady()) {
      wait = millis() + (scanRate * abs(servo.read() - angle));
      servo.write(angle);  
    }
}

boolean Robot::servoReady(){
  return millis() > wait;
}

boolean Robot::scanComplete() {
  return ((servo.read() >= 170 && scanDirection==+10) || (servo.read() <= 10 && scanDirection==-10));
}