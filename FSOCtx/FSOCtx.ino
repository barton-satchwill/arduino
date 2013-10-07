
//----------------------------------------------------
//
//   gnd o\---------[laser]-----o 13
//         \---------[led]------o 12
//
//----------------------------------------------------

#define LEDlaser 12
#define LEDtx 13
int bitDelay = 50;


void setup() { 
  Serial.begin(9600); 
  pinMode(LEDlaser, OUTPUT);
  pinMode(LEDtx, OUTPUT);
  digitalWrite(LEDlaser, LOW);
  digitalWrite(LEDtx, LOW);
  Serial.println("=========== Transmitter ===========");
} 

void loop() { 
  preamble();
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
  




