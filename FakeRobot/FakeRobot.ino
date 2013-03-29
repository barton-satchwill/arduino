
#include <Servo.h>

#define LED 6
#define SERVO 5

enum action {A, B, C, D, E, F};
char* actions[] = {"A", "B", "C", "D", "E", "F"};
volatile action sonarAction = F;
volatile int EIGHT_HZ = 0;

int angle;
int scanDirection = 10;
Servo servo;  
int maxRange;
int sonarRange;
long moveDelay = 300;
volatile long wait;


void setup(){
  Serial.begin(9600);
  servo.attach(SERVO);  
  pinMode(LED, OUTPUT);
  setupTimer();
}

void loop(){
  drive();
  delay(100);
  scan();
}

void scan() {
  int r;
  Serial.println(actions[sonarAction]);
  switch (sonarAction) {
    case A: // move to inital position
      r = 0;
      sonarRange = 0;
      if (angle <90) angle = 10;
      if (angle >90) angle = 170;
      sonarAction = C;
      break;
      
    case B: // move a step
      if (angle >= 170) scanDirection = -10;
      else
        if (angle <= 10) scanDirection = +10;

      angle = angle + scanDirection;
      angle = min(angle, 170);
      angle = max(angle, 10);
      if (angle > 170 || angle < 10) {
        sonarAction = D;
      } else {        
        servo.write(angle);
        wait = millis() + moveDelay;
        sonarAction = C;
      }
      break;

    case C: // take the range
      if (millis() > wait){
        r = range();
        Serial.print(" : range at angle " + String(angle) + " is " + String(r) + "\n");
        sonarRange = max(sonarRange, r);
        sonarAction = B;
      }
      break;

    case D: // check for complete scan
      if (servo.read() >= 170 || servo.read() <= 10) {
        sonarAction = E;
      } else {
        sonarAction = B;
      }
      break;

    case E: //scan complete
      break;
    case F:
      break;
    default:
      ;
  }
}

void drive() {
//  Serial.println("drive...");
  if (sonarAction == D) {
    Serial.println("---------- all done ----------");
    delay(5000);
//    sonarAction = F;
//    if (maxRange != sonarRange){
//      maxRange = sonarRange;
//      Serial.println("--------> turning!"); 
//      delay(1000);
  //  }
  }
}

int range() {
  return random(5,50);
}

void setupTimer() {
  noInterrupts();
  // 8 bit timer
  TCCR2A = 0;               // set entire TCCR2A register to 0
  TCCR2B = 0;               // same for TCCR2B
  TCNT2  = 0;               //initialize counter value to 0
  // set compare match register for 8khz increments
  OCR2A = 249;              // = (16*10^6) / (1000 * 64) - 1 (must be <256)
  TCCR2A |= (1 << WGM21);   // turn on CTC mode
  TCCR2B |= (1 << CS12) | (1<<CS10);    // Set CS10 and CS12 bit for 64 prescaler
  TIMSK2 |= (1 << OCIE2A);  // enable timer compare interrupt
  interrupts();
}  


ISR(TIMER2_COMPA_vect){
  if (EIGHT_HZ++ == (4 * 1000)) {
    EIGHT_HZ = 0;
    digitalWrite(LED, digitalRead(LED) ^ 1);
    sonarAction = A;
  }
}


