
//----------------------------------------------------
//
//   gnd o\---------[laser]-----o 13
//         \---------[led]------o 12
//
//----------------------------------------------------

#define LEDlaser 12
#define LEDtx 13
volatile int CLOCK_COUNTER = 0;
volatile boolean send_bit = false;
int baudrate = 100; 
int bitcount = 0;


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
  if (Serial.available()){
    char c = Serial.read();
    if (c == '!') {
      configure();
    } else {
      transmit_byte(c);
    }
  }
}


void transmit_byte(byte data) {
  bitcount = 0;
  while (bitcount < 8){
    if(send_bit){
      digitalWrite(LEDlaser, bitRead(data, bitcount++));
      Serial.print(digitalRead(LEDlaser));
      send_bit = false;
      //----------- debugging -----------
      if (bitcount == 8) { 
        Serial.print("-->[");
        Serial.write(data);
        Serial.print("]-->");
        Serial.print(data, BIN);
        Serial.println(); 
      }
      //---------------------------------
    }
  }
}

void preamble(){
  Serial.println("sending preamble");
  byte preambleByte = 85; // 01010101
  transmit_byte(preambleByte);
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

  if (CLOCK_COUNTER == baudrate) { 
    CLOCK_COUNTER = 0;
    digitalWrite(13, digitalRead(13)^1);
    send_bit = true;
  }
}
//--------------------------------------------------------------------------

void configure(){
  status();
  while(!Serial.available()){
    ; // do nothing
  }
  while(Serial.available()){
    byte s = Serial.read();
    switch(s){
    case 'b':
      baudrate = Serial.parseInt();
      Serial.print("baud rate = "); 
      Serial.println(baudrate);   
      break;
    case 't':
      Serial.println("transmit");
      break;
    default:
      Serial.println("default");
      break;
    }
  }
}

void status(){
  Serial.println("------------------------------------------");
  Serial.print("send_bit = ");  
  Serial.println(send_bit);   
  Serial.print("baud rate = "); 
  Serial.println(baudrate);   
  Serial.println("------------------------------------------");
}




