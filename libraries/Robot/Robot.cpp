/*
  Robot.cpp -
  Released into the public domain.
*/

#include <Robot.h>
#include <Logging.h>

//motor control shield
// #define SERVO 9
enum {RANGE, ANGLE};
enum action {INIT, MOVE, PING, CHECK, DONE, F};
volatile action sonarAction = DONE;
volatile int EIGHT_HZ = 3500;

volatile int servoAngle;
int scanAngle;
int servoLimit;
int servoDirection = 10;
int maxRange;
// int sonarRange;
long moveDelay = 30;
volatile long wait;
Servo servo;  // create servo object to control a servo
int Robot::count = 0;

// power pro: 0.165 msec/60° ==> 2.75 msec / degree
// hitec: 5 msec / degree
int scanRate = 5;
// int scanAngle = 90;
int scanStep = 10;
int max[] = {0,0}; // range, angle
int range = 0;

int turnAngle = 0;

//Debug and development stuff
#define SERVO 5
int report = 0;




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
    EIGHT_HZ = 0;
    sonarAction = INIT;
  }
}




Robot::Robot(int speed) {
    id = count++;
    setSpeed(speed);
    servo.attach(SERVO);  // attaches the servo to the servo object
    servo.write(90);
    setupTimer();
}


void Robot::setSpeed(int speed) {
    this->speed = speed;
    motor.setSpeed(this->speed);
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





void Robot::rangeScan(int scanAngle) {
  int sonarRange;
  switch (sonarAction) {
    case INIT: // move to inital position
      sonarRange = 0;
      sonarRange = 0;
      max[ANGLE] = 0;
      max[RANGE] = 0;
      report = 0;
      servoLimit = (scanAngle/2);
      servoAngle = 90-servoLimit; 
      servoDirection = +scanStep;
      moveServo(servoAngle);
      sonarAction = PING; 
      break;
      
    case MOVE: // move a step
      servoAngle = servoAngle + servoDirection;
      servoAngle = min(servoAngle, 170);
      servoAngle = max(servoAngle, 10);
      moveServo(servoAngle);
      sonarAction = PING;
      break;

    case PING: // take the range
      if (servoReady()){
        sonarRange = range();
        Serial.println("range  is " + String(sonarRange) + "\" at servoAngle " + String(servoAngle));
        // maxRange = max(maxRange, sonarRange);
        if(sonarRange > max[RANGE]) {
            max[RANGE] = sonarRange;
            max[ANGLE] = servoAngle;
        }
        sonarAction = CHECK;
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
      if (report == 0){
        turnAngle = max[ANGLE];
        Serial.println("angle to maximum distance is " + String(max[ANGLE]));
        report=1;
      }
      break;

    default:
      break;
  }
}

void Robot::moveServo(int servoAngle){
  if (servoReady()) {
      wait = millis() + (scanRate * abs(servo.read() - servoAngle));
      servo.write(servoAngle);  
    }
}

boolean Robot::servoReady(){
  return millis() > wait;
}

boolean Robot::scanComplete() {
  return (
    (servo.read() >= (90+servoLimit)) && (servoDirection >0) || 
    (servo.read() <= (90-servoLimit)) && (servoDirection <0));
}

int Robot::getTurnAngle() {
  return turnAngle;
}