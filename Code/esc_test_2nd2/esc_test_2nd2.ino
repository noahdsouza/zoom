int pot_val = 0;
int output_val = 0;
bool send = false;
char command = ' ';
void setup() {
  // put your setup code here, to run once:
  setPwmFrequency(3, 10000);
  Serial.begin(115200);
}

void loop() {
  // put your main code here, to run repeatedly:
  pot_val = analogRead(0);

  // Empty the command variable, check for new input and decide what to do with it.
  command = ' ';
  if (Serial.available() > 0) {
    command = Serial.read();
  }
  
  if (command == ' ') {
  }
  else if (command == 'd') {
    output_val = 255;
    send = true;
    Serial.println(2000);
  }
  else if (command == 'n') {
    output_val = 127;
    send = true;
    Serial.println(1500);
  }
  else if (command == 'p') {
    output_val = 0;
    send = true;
    Serial.println(1000);
  }
  
  //output_val = map(pot_val, 340, 420, 0, 255);
  if (send){
  analogWrite(3,output_val);
  }
  //Serial.println(127);
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
