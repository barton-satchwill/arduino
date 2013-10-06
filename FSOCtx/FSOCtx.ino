
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
  Serial.println("=========== Transmitter ===========");
} 

void loop() { 
  for (byte data = '\!'; data <= '\~'; data++){
    Serial.print("-->");  
    Serial.print(" dec: ");  Serial.print(data, DEC);  
    //Serial.print(", hex: ");  Serial.print(data, HEX); 
    //Serial.print(", oct: ");  Serial.print(data, OCT);
    Serial.print(", bin: ");  Serial.print(data, BIN);   
    Serial.print("-->");  

    for (byte mask = 00000001; mask>0; mask <<= 1) {
      int val = ((data & mask) > 0);
      digitalWrite(LEDlaser, val);
      digitalWrite(LEDtx, val);
      Serial.print(val);
      delay(bitDelay);
    }
    Serial.println();
  } 
}




