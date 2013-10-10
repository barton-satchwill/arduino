
//----------------------------------------------------
//
//   gnd o\---------[sensor]-----o A0
//         \----------[led]------o 13 tell-tale
//          \---------[led]------o 12 for tuning
//
//----------------------------------------------------

//-------------------------
#define LEDinteruptA 4
#define LEDinteruptB 5
volatile int CLOCK_COUNTER = 0;
enum timer_action { 
  TRANSMIT, RECIEVE};
char* timer_actions[] = { 
  "TX", "RX"};
volatile timer_action timerAction = TRANSMIT;
volatile timer_action tempAction;
int blip = 0;
volatile int previousstate = 0;
volatile int flipcount = 0;
volatile int fliplimit = 6;
int baudrate = 1000; 
//-------------------------
#define LEDSENSOR A0
#define SAMPLESIZE 10
#define LEDrx 11
int threshold = 446;
long t;

void setup() {
  setupTimer();
  for (int i =3; i<=13;i++){ 
    pinMode(i,OUTPUT); 
    digitalWrite(i,LOW);
  }
  Serial.begin(9600);
  pinMode(LEDSENSOR, INPUT);
  pinMode(LEDrx, OUTPUT);
  Serial.println("=========== Reciever ===========");
  //    tune();
  synchronise();
}

void loop() {
}

void guessBaudRate() {
  int x;
  if (x != getSensorReading(LEDSENSOR)){
    long elapsed = millis()-t;
    t=millis();
    x=x^1;
    if (abs(baudrate - elapsed) < (baudrate*0.1)){
      baudrate = (baudrate*0.9) + (elapsed * 0.1);
      Serial.print(elapsed);
      Serial.print(" --> ");
    }
    Serial.println(baudrate);
    digitalWrite(13,x);
  }
}

void receive() {
  digitalWrite(13, LOW);
  digitalWrite(12, LOW);
  byte low = 0;
  //    byte data = Serial.read();
  for (byte mask = 00000001; mask>0; mask <<= 1){
    //      digitalWrite(13, (data & mask) > 0);
    //      digitalWrite(12, (data & mask) > 0);
    delayMicroseconds(100);
    // receive a single bit of transmitted data
    int bitValue = getSensorReading(LEDSENSOR);
    low += (bitValue * mask);
    digitalWrite(LEDrx, bitValue);
    //Serial.print(bitValue);
  }
  Serial.print((char)low);
  if (Serial.available()==0){
    Serial.println();
  }
}



int getSensorReading(int sensorPin){
  long bitValue=0;
  for (int i=0; i<SAMPLESIZE; i++){
    bitValue += (analogRead(sensorPin));
    delayMicroseconds(100);
  }
  bitValue = (bitValue/SAMPLESIZE);
  //Serial.println(bitValue);
  if (bitValue < threshold) {
    bitValue = 0;
  } 
  else {
    bitValue = 1;
  }
  return bitValue;
}


void tune() {
  Serial.print("tuning...");
  for (byte data = '\!'; data <= '\~'; data++){
    for (byte mask = 00000001; mask>0; mask <<= 1){
      digitalWrite(13, (data & mask) > 0);
      digitalWrite(12, (data & mask) > 0);
      delayMicroseconds(80);
      int val = getSensorReading(LEDSENSOR);
      if (((data & mask) > 0) > val){ 
        --threshold; 
      }
      if (((data & mask) > 0) < val){ 
        ++threshold; 
      }
    }
  }
  Serial.print("new threshold is ");
  Serial.println(threshold);
}






//it's not enough to look for an edge, you have to look for a transition
void synchronise(){
  while (flipcount<fliplimit){   
    volatile int x = getSensorReading(LEDSENSOR);
    if (previousstate != x ) {
      previousstate = x; 
      flipcount++; 
      digitalWrite(flipcount+2, HIGH);
    }
    if (flipcount == fliplimit){
      flipcount = 100;
      CLOCK_COUNTER = baudrate;
      digitalWrite(13, HIGH);
      digitalWrite(12, HIGH);
    }    
  }
}


//--------------------------------------------------------------------------

void setupTimer() {
  noInterrupts();
  // 8 bit timer
  TCCR2A = 0;              // set entire TCCR2A register to 0
  TCCR2B = 0;              // same for TCCR2B
  TCNT2  = 0;              //initialize counter value to 0
  // set compare match register for 8khz increments
  OCR2A = 249;             // = (16*10^6) / (1000 * 64) - 1 (must be <256)
  TCCR2A |= (1 << WGM21);  // turn on CTC mode
  TCCR2B |= (1 << CS12);   // | (1<<CS10);    // Set CS10 and CS12 bit for 64 prescaler
  TIMSK2 |= (1 << OCIE2A); // enable timer compare interrupt
  interrupts();
}  

ISR(TIMER2_COMPA_vect){
  CLOCK_COUNTER++;
  if (CLOCK_COUNTER >= baudrate) { 
    CLOCK_COUNTER = 0;
    digitalWrite(13, digitalRead(13) ^ 1);
    //    sample();
  }
  if (CLOCK_COUNTER%150 == 0) { 
    digitalWrite(11, digitalRead(11) ^ 1);
    //    sample();
  }
}

//--------------------------------------------------------------------------











