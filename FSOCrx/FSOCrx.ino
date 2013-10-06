
//----------------------------------------------------
//
//   gnd o\---------[sensor]-----o A0
//         \----------[led]------o 13 tell-tale
//          \---------[led]------o 12 for tuning
//
//----------------------------------------------------

#define LEDSENSOR A0
#define SAMPLESIZE 10
#define LEDrx 11
int threshold = 200;

void setup() {
  Serial.begin(9600);
  pinMode(LEDSENSOR, INPUT);
  pinMode(LEDrx, OUTPUT);
  Serial.println("=========== Reciever ===========");
  pinMode(12, OUTPUT);
  digitalWrite(12,HIGH);
  pinMode(13, OUTPUT);
  tune();
}

void loop() {
  digitalWrite(13, LOW);
  digitalWrite(12, LOW);
//  if (Serial.available()){
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
  } else {
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
      int bitValue = getSensorReading(LEDSENSOR);
      if (((data & mask) > 0) > bitValue){ (--threshold); }
      if (((data & mask) > 0) < bitValue){ (++threshold); }
    }
  }
  Serial.print("new threshold is ");
  Serial.println(threshold);
}





