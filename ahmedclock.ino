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
