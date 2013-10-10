
//----------------------------------------------------
//
//   gnd o\---------[laser]-----o 13
//         \---------[led]------o 12
//
//----------------------------------------------------

//-------------------------
#define LEDinteruptA 4
#define LEDinteruptB 5
volatile int CLOCK_COUNTER = 0;
enum timer_action {TRANSMIT, RECIEVE};
char* timer_actions[] = {"TX", "RX"};
volatile timer_action timerAction = TRANSMIT;
volatile timer_action tempAction;
int baudrate = 1000; 
//-------------------------
#define LEDlaser 12
#define LEDtx 13
int bitDelay = 50;


void setup() { 
  Serial.begin(9600); 
  setupTimer();
  pinMode(LEDlaser, OUTPUT);
  pinMode(LEDtx, OUTPUT);
  digitalWrite(LEDlaser, LOW);
  digitalWrite(LEDtx, LOW);
  Serial.println("=========== Transmitter ===========");
} 

void loop() { 
//  preamble();
//  for (byte data = '\!'; data <= '\~'; data++){
//    transmit(data);
//  }
}

void transmit(byte data) { 
//----------- debugging -----------
  Serial.print("[");  
  Serial.write(data);
  Serial.print("]-->");  
  Serial.print(data, BIN);   
  Serial.print("-->");  
//---------------------------------
  for (byte mask = 00000001; mask>0; mask <<= 1) {
    int val = ((data & mask) > 0);
    digitalWrite(LEDlaser, val);
    digitalWrite(LEDtx, val);
    Serial.print(val);
    delay(bitDelay);
  }
  Serial.println();
}


void preamble(){
  Serial.println("sending preamble");
  for (int i=0; i<10; i++){
    int val = (digitalRead(LEDlaser) ^1);
    digitalWrite(LEDlaser, val);
    digitalWrite(LEDtx, val);
    Serial.print(val);
    delay(500);
  }
  Serial.println();
  transmit('\X');
  digitalWrite(LEDlaser, LOW);
  digitalWrite(LEDtx, LOW);
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
  if (CLOCK_COUNTER++ == baudrate) { 
    CLOCK_COUNTER = 0;
    digitalWrite(LEDlaser, digitalRead(LEDlaser) ^ 1);
    digitalWrite(LEDtx, digitalRead(LEDtx) ^ 1);
  }
}

//--------------------------------------------------------------------------

