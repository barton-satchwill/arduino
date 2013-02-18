const int beepPin = 12;
const int outPin = 13; 
int mail = LOW; // Is there new mail?
int val; // Value read from the serial port
int state = HIGH;

void setup()
{
    pinMode(outPin, OUTPUT); // sets the digital pin as output
    pinMode(beepPin, OUTPUT); // sets the digital pin as output
    Serial.begin(9600);
    Serial.flush();
    Serial.println("starting...");
}

void loop()
{
    // Read from serial port
    if (Serial.available()) {
        val = Serial.read();
        Serial.println(val);
        if (val == 'M') {
          if (mail != HIGH) {
            tone(beepPin, 400, 500);
          }
          mail = HIGH;
        }
        else if (val == 'N') { 
          mail = LOW;
        }
    }

    // Set the status of the output pin
    digitalWrite(outPin, mail);
}
