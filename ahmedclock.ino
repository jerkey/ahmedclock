// ahmedclock: branch dronearm
// drives two DC motors on a drone, one which tightens a worm clamp onto a pole
// the other which loosens to disengage the drone from the thing it just attached
// to a pole.  for disaster.radio
// using an Extruder Controller V2.2 board from an original makerbot.
//
// for a 2018 demonstration of drone mounting of a disaster.radio node
// by http://github.com/jerkey
#define DIR1    7
#define DIR2    8
#define ENABLE1 5
#define ENABLE2 6

// the atmega328 only has a single Serial port, and the default SBUS code
// doesn't run on it. You can get it to work by not using the timer, but that
// means you can't use delay in the loop() function anymore.

#include <SBUS.h>
SBUS sbus(Serial);

void setup() {
  sbus.begin(false); // we won't be able to use the delay() function
  pinMode(DIR1   ,OUTPUT);
  pinMode(DIR2   ,OUTPUT);
  pinMode(ENABLE1,OUTPUT);
  pinMode(ENABLE2,OUTPUT);
  digitalWrite(DIR1,HIGH);
  digitalWrite(DIR2,HIGH);
}

void loop() {
  sbus.process();
  int value = sbus.getChannel(1);
  analogWrite(ENABLE1, value / 8);
  value = sbus.getChannel(2);
  analogWrite(ENABLE2, value / 8);
}
