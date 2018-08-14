// ahmedclock: branch dronearm
// drives two DC motors on a drone, one which tightens a worm clamp onto a pole
// the other which loosens to disengage the drone from the thing it just attached
// to a pole.  for disaster.radio
// using an Extruder Controller V2.2 board from an original makerbot.
//
// for a 2018 demonstration of drone mounting of a disaster.radio node
// by http://github.com/jerkey
#define TORQUE_AMPS     2.0 // how many amps at which we think the screw is tight
#define MOTOR1PWM       192
#define MOTOR2PWM       192
#define MODE1MINTIME    2000 // how many milliseconds minimum for mode 1
#define MODE1TIMEOUT    7000 // how many milliseconds to do mode 1 before error timeout
#define MODE2TIMEOUT    5000 // how many milliseconds to do mode 2
#define PRINTRATE       500 // how often to printInfo()
#define THERMISTOR      A3 // has a 4.7KΩ pullup resistor to Vcc
#define REVERSE_BUTTON  3 // a button to reverse the worm-clamp motor
#define DIR1    7
#define DIR2    8
#define ENABLE1 5
#define ENABLE1_IS_PWM (TCCR0A & 0b00110000) // page 106 of pdf ~/sketchbook/ATmega328_168.pdf
#define ENABLE2 6
#define ENABLE2_IS_PWM (TCCR0A & 0b11000000) // page 106 of pdf ~/sketchbook/ATmega328_168.pdf
#define ISENSE1COEFF 204.37 // 6.5v 8.1Ω ADC=164
#define ISENSE1 A6
//#define ISENSE2COEFF 204.37
//#define ISENSE2 A7

byte mode = 0; // 0: do nothing, 1: motor 1 until amps, 2: motor 2
unsigned long mode1time = 0;
unsigned long mode2time = 0;
unsigned long printtime= 0;
float isense1;
int thermistor;

void setup() {
  Serial.begin(57600);
  pinMode(DIR1   ,OUTPUT);
  pinMode(DIR2   ,OUTPUT);
  pinMode(ENABLE1,OUTPUT);
  pinMode(ENABLE2,OUTPUT);
  digitalWrite(REVERSE_BUTTON,HIGH); // enable pull-up resistor on this input button
  digitalWrite(DIR2,LOW); // loosen screw to disengage copter
  //setPwmFrequency(ENABLE1,8); // sets divisor for both ENABLE1 and ENABLE2
}

void loop() {
  if (mode==0) {
    digitalWrite(DIR1,HIGH); // loosen
    if (! digitalRead(REVERSE_BUTTON)) { // button is pressed
      if (ENABLE1_IS_PWM==0) {
        Serial.print("REVERSING");
        analogWrite(ENABLE1,MOTOR1PWM); // reverse motor if button is closed
      }
    } else {
      digitalWrite(ENABLE1,LOW);
    }
  }

  if (mode==1) {
    digitalWrite(DIR1,LOW); // tighten
    if (ENABLE1_IS_PWM==0) analogWrite(ENABLE1,MOTOR1PWM);
    if (millis() - mode1time > MODE1TIMEOUT) { // we took too long, restart the sequence
      digitalWrite(ENABLE1,LOW);
      mode = 0;
    }
  }

  if ((mode == 2) && (millis() - mode2time < MODE2TIMEOUT)) {
    if (! ENABLE2_IS_PWM) analogWrite(ENABLE2, MOTOR2PWM);
  } else {
    digitalWrite(ENABLE2, LOW);
    if (mode == 2) mode = 0; // we're done
  }

  isense1 = analogRead(ISENSE1)/ISENSE1COEFF;
  thermistor = analogRead(THERMISTOR);
  printInfo();
  if ((thermistor < 512) && (mode == 0)) {
    Serial.println("tripwire!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");
    mode = 1; // detect tripwire
    mode1time = millis(); // start the timer for mode1
  }
  if ((mode==1) && (isense1 > TORQUE_AMPS) && (millis() - mode1time > MODE1MINTIME)) {
    Serial.println("TIGHT!   XXXXXX XXXXXXXXXXXX     X          XXXXXXXXX       XXXXXXX     XXXXXXXXXXX   XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX");
    digitalWrite(ENABLE1,LOW); // turn off screwmotor
    Serial.print(isense1,2);
    mode = 2;
    mode2time = millis(); // start the timer for mode2
  }
}

void printInfo() {
  if (millis() - printtime > PRINTRATE) {
    printtime = millis();
    Serial.print(mode);
    Serial.print("\t");
    Serial.print(isense1,2);
    Serial.print("\t");
    Serial.println(thermistor);
  }
}

void setPwmFrequency(int pin, int divisor) {
  byte mode;
  if(pin == 5 || pin == 6 || pin == 9 || pin == 10) {
    switch(divisor) {
      case 1: mode = 0x01; break;
      case 8: mode = 0x02; break;
      case 64: mode = 0x03; break;
      case 256: mode = 0x04; break;
      case 1024: mode = 0x05; break;
      default: return;
    }
    if(pin == 5 || pin == 6) {
      TCCR0B = TCCR0B & 0b11111000 | mode;
    } else {
      TCCR1B = TCCR1B & 0b11111000 | mode;
    }
  } else if(pin == 3 || pin == 11) {
    switch(divisor) {
      case 1: mode = 0x01; break;
      case 8: mode = 0x02; break;
      case 32: mode = 0x03; break;
      case 64: mode = 0x04; break;
      case 128: mode = 0x05; break;
      case 256: mode = 0x06; break;
      case 1024: mode = 0x07; break;
      default: return;
    }
    TCCR2B = TCCR2B & 0b11111000 | mode;
  }
}
