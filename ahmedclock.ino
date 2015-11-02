// ahmedclock
// drives one of the secondary windings on an AC-powered alarm clock
// with 12VAC by switching a 12V battery polarity back and forth at 60Hz
// using an Extruder Controller V2.2 board from an original makerbot.
//
// for a 2015 halloween costume appearing as Ahmed Mohammed
// by http://github.com/jerkey
#define DIR1    7
#define DIR2    8
#define ENABLE1 5
#define ENABLE2 6
#define HALFPERIOD 8 // milliseconds to wait

void setup() {
  pinMode(DIR1   ,OUTPUT);
  pinMode(DIR2   ,OUTPUT);
  pinMode(ENABLE1,OUTPUT);
  pinMode(ENABLE2,OUTPUT);
  digitalWrite(ENABLE1,HIGH); // turn on output 1
}

void loop() {
  digitalWrite(DIR1,HIGH);
  delay(HALFPERIOD);
  digitalWrite(DIR1,LOW);
  delay(HALFPERIOD);
}
