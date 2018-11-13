// Attempt to control ESC and motor from scratch

#include <Servo.h>
Servo ESC1;

int motor = 9;

void setup() {
  // put your setup code here, to run once:
  pinMode(motor, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  analogWrite(motor,2000);
}
