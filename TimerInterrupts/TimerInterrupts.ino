
//timer interrupts
//by Amanda Ghassaei
//June 2012
//http://www.instructables.com/id/Arduino-Timer-Interrupts/

/*
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 *
*/

// timer setup for timer0, timer1, and timer2.
// For arduino uno or any board with ATMEL 328/168.. diecimila, duemilanove, lilypad, nano, mini...
//
// this code will enable all three arduino timer interrupts.
//   timer0 will interrupt at 2kHz
//   timer1 will interrupt at 1Hz
//   timer2 will interrupt at 8kHz

#include <Servo.h>



/*
 timer speed (Hz) = (Arduino clock speed (16MHz)) / prescaler
     prescaler can be either 1, 8, 64, 256 or 1024
     prescaler can be either 2^0, 2^3, 2^6, 2^7 or 2^8
 interrupt frequency (Hz) = (Arduino clock speed 16,000,000Hz) / (prescaler * (compare match register + 1))
 compare match register = [ 16,000,000Hz/ (prescaler * desired interrupt frequency) ] - 1
 
 CS12    CS11    CS10      Prescaler
 ------------------------------------------------------------------------------
  0       0       0       Timer off/no clock source
  0       0       1       1:1 prescaler/this is the same as not having prescaler
  0       1       0       1:8 prescaler
  0       1       1       1:64 prescaler
  1       0       0       1:256 prescaler
  1       0       1       1:1024 prescaler
  
timer0 : pins 5 and 6; used by delay() and millis()
timer1 : pins 9 and 10; used by Servo library
timer2 : pins 11 and 3; used by tone()
*/

#define SERVO 5
#define POT 2

//#define LED1 13
//#define LED3 8
//#define LED2 9
#define LED1 11
#define LED3 10
#define LED2 9

#define CLOCK_HZ = 16000000

volatile int TWO_HZ = 0;
volatile int EIGHT_HZ = 0;

Servo myservo;  // create servo object to control a servo 

void setup(){  
  myservo.attach(SERVO);  // attaches the servo on pin 9 to the servo object 

  //set pins as outputs
  pinMode(LED1, OUTPUT);
  pinMode(LED3, OUTPUT);
  pinMode(LED2, OUTPUT);

  noInterrupts();

// set timer0 interrupt at 2kHz
// 8 bit timer
  TCCR0A = 0;        // set entire TCCR0A register to 0
  TCCR0B = 0;        // same for TCCR0B
  TCNT0  = 0;        //initialize counter value to 0
  // set compare match register for 2khz increments
  OCR0A = 124;                // = (16*10^6) / (2000*64) - 1 (must be <256)
  TCCR0A |= (1 << WGM01);     // turn on CTC mode
  TCCR0B |= (1 << CS11) | (1 << CS10);     // Set CS11 and CS10 bits for 64 prescaler
  TIMSK0 |= (1 << OCIE0A);    // enable timer compare interrupt


//// set timer1 interrupt at 1Hz
//// 16 bit timer
//  TCCR1A = 0;// set entire TCCR1A register to 0
//  TCCR1B = 0;// same for TCCR1B
//  TCNT1  = 0;//initialize counter value to 0
//  // set compare match register for 1hz increments
//  OCR1A = 15624;            // = (16*10^6) / (1*1024) - 1 (must be <65536)
//  TCCR1B |= (1 << WGM12);   // turn on CTC mode
//  TCCR1B |= (1 << CS12) | (1 << CS10);    // Set CS12 and CS10 bits for 1024 prescaler
//  TIMSK1 |= (1 << OCIE1A);  // enable timer compare interrupt


//set timer2 interrupt at 8kHz
// 8 bit timer
  TCCR2A = 0;               // set entire TCCR2A register to 0
  TCCR2B = 0;               // same for TCCR2B
  TCNT2  = 0;               //initialize counter value to 0
  // set compare match register for 8khz increments
  OCR2A = 249;              // = (16*10^6) / (8*8000) - 1 (must be <256)
  TCCR2A |= (1 << WGM21);   // turn on CTC mode
  TCCR2B |= (1 << CS11);    // Set CS11 bit for 8 prescaler
  TIMSK2 |= (1 << OCIE2A);  // enable timer compare interrupt

  interrupts();
}



ISR(TIMER0_COMPA_vect){ 
//generates pulse wave of frequency 2kHz/2 = 1kHz 
  TWO_HZ++;
  if (TWO_HZ == 1000) {
    TWO_HZ = 0;
    digitalWrite(LED1, digitalRead(LED1) ^ 1);
  }
}

//ISR(TIMER1_COMPA_vect){
//// generates pulse wave of frequency 1Hz/2 = 0.5kHz 
//  digitalWrite(LED2, digitalRead(LED2) ^ 1);
//}
  
ISR(TIMER2_COMPA_vect){
// generates pulse wave of frequency 8kHz/2 = 4kHz 
  EIGHT_HZ++;
  if (EIGHT_HZ == 1000) {
    EIGHT_HZ = 0;
    digitalWrite(LED3, digitalRead(LED3) ^ 1);
  }
}


void loop(){
  //do other things here
  // servo
  int val = analogRead(POT);            // reads the value of the potentiometer (value between 0 and 1023) 
  val = map(val, 0, 1023, 0, 179);     // scale it to use it with the servo (value between 0 and 180) 
  myservo.write(val);                  // sets the servo position according to the scaled value 
  delay(15);                           // waits for the servo to get there 
}

