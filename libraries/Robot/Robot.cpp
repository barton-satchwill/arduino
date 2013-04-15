/*
  Robot.cpp -
  Released into the public domain.
*/

#include <Robot.h>
#include <Logging.h>

//motor control shield
#define SERVO 9
enum {RANGE, ANGLE};
enum action {INIT, MOVE, PING, CHECK, DONE, READY};
volatile action sonarAction = READY;
volatile int EIGHT_HZ = 0;

volatile int servoAngle;
float rangeForward;
int scanAngle;
int servoLimit;
int servoDirection = 10;
int maxRange;
// int sonarRange;
// long moveDelay = 30;
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

int turnAngle = 90;

long scanFrequency = 1*1000;



void setupTimer() {
  noInterrupts();
  // 8 bit timer
  TCCR2A = 0;                         // set entire TCCR2A register to 0
  TCCR2B = 0;                         // same for TCCR2B
  TCNT2  = 0;                         //initialize counter value to 0
                                      // set compare match register for 8khz increments
  OCR2A = 249;                        // = (16*10^6) / (1000 * 64) - 1 (must be <256)
  TCCR2A |= (1 << WGM21);             // turn on CTC mode
  TCCR2B |= (1 << CS12) ; //| (1<<CS10);  // Set CS12 bit for 256 prescaler
  TIMSK2 |= (1 << OCIE2A);            // enable timer compare interrupt
  interrupts();
}  

ISR(TIMER2_COMPA_vect){
  if (EIGHT_HZ++ >= scanFrequency) {  
    EIGHT_HZ = 0;
    if (sonarAction == READY) {
      sonarAction = INIT;
    }
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


float Robot::rangeAhead () {
  if (servo.read() == 90) {
    rangeForward = sonar.range();
  }
    return rangeForward;
}


float Robot::range () {
    int sonarRange = sonar.range();
    // Serial.println("range  is " + String(sonarRange) + "\" at servoAngle " + String(servo.read()));
    return sonarRange;
}


void Robot::initiateRangeScan() {
  Serial.println("force scan start");
  sonarAction = INIT;
  EIGHT_HZ = 0;
}


void Robot::rangeScan(int scanAngle) {
  int sonarRange;

  switch (sonarAction) {
    case INIT: // move to inital position
      Serial.println("scan start...");
      sonarRange = 0;
      sonarRange = 0;
      max[ANGLE] = 90;
      max[RANGE] = 0;
      servoLimit = (scanAngle/2);
      servoAngle = 90-servoLimit; 
      servoDirection = +scanStep;
      moveServo(servoAngle);
      sonar.setRate(Sonar::FAST);
      sonarAction = PING; 
      break;
      
    case MOVE: // move a step
      servoAngle = servoAngle + servoDirection;
      servoAngle = min(servoAngle, 175);
      servoAngle = max(servoAngle, 5);
      moveServo(servoAngle);
      sonarAction = PING;
      break;

    case PING: // take the range
      if (servoReady()){
        sonarRange = range();
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
      turnAngle = max[ANGLE];
      Serial.println("angle to maximum distance is " + String(max[ANGLE]));
      sonarAction = READY;
      sonar.setRate(Sonar::SLOW);
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
  Serial.println("---done?---");
  Serial.print(servo.read());
  Serial.print(" >= ");
  Serial.print(90+servoLimit);
  Serial.print(" > ");
  Serial.println(servoDirection);
  Serial.print(servo.read());
  Serial.print(" <= ");
  Serial.print(90-servoLimit);
  Serial.print(" < ");
  Serial.println(servoDirection);
  Serial.println("--------------");

  return (
    (servo.read() >= (90+servoLimit)) && (servoDirection > 0) || 
    (servo.read() <= (90-servoLimit)) && (servoDirection < 0));
}


int Robot::getTurnAngle() {
  return turnAngle;
}


void Robot::clearTurnAngle() {
  turnAngle = 90;
}