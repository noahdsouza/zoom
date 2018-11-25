const int white = A2;
const int yellow = A3;
const int red = A4;
const int black = A5;
const int motor1 = 9;

int readIndex = 0; // used for rolling average
const int samples = 20; // smaples to average over

float speed = 0; // a speed value from 0-1
int output = 0; // the value written to the motors
int PWRange = 600; // range of PWM signal
int PWMin = 1400; // minimum PWM value

int readingsW[samples]; int totalW = 0; int avgW = 0; int baseW = -1; float varW = 1;
int readingsY[samples]; int totalY = 0; int avgY = 0; int baseY = -1; float varY = 1;
int readingsR[samples]; int totalR = 0; int avgR = 0; int baseR = -1; float varR = 1;
int readingsB[samples]; int totalB = 0; int avgB = 0; int baseB = -1; float varB = 1;

void setup() {
  Serial.begin(9600);
  // initialize the buffer for calculating the rolling average
  for (int i = 0; i < samples; i++) {
    readingsW[i] = 0;
    readingsY[i] = 0;
    readingsR[i] = 0;
    readingsB[i] = 0;
  }
}

void loop() {
  // basically, these calculate the rolling averages for each sensor
  totalW -= readingsW[readIndex];
  readingsW[readIndex] = analogRead(white);
  totalW += readingsW[readIndex];
  avgW = totalW / samples;

  totalY -= readingsY[readIndex];
  readingsY[readIndex] = analogRead(yellow);
  totalY += readingsY[readIndex];
  avgY = totalY / samples;

  totalR -= readingsR[readIndex];
  readingsR[readIndex] = analogRead(red);
  totalR += readingsR[readIndex];
  avgR = totalR / samples;

  totalB -= readingsB[readIndex];
  readingsB[readIndex] = analogRead(black);
  totalB += readingsB[readIndex];
  avgB = totalB / samples;

  readIndex = (readIndex + 1) % samples;

  // Set baseline values if they haven't been set already (effecively calibrates on power up)
  if (baseW == -1) {
    baseW = avgW;
    baseY = avgY;
    baseR = avgR;
    baseB = avgB;
  }

  // Calculate the variance from baseline of each sensor
  varW = avgW / baseW;
  varY = avgY / baseY;
  varR = avgR / baseR;
  varB = avgB / baseB;

  speed = calcSpeed(varW, varY, varR, varB, speed);

  // map it to the range of the pwm output:
  output = speed * PWRange + PWMin;
  analogWrite(motor1, output);

  // print the results to the Serial Monitor every few cycles:
  if (readIndex == 0) {
    Serial.print("White = ");
    Serial.println(avgW);
    Serial.print("Yellow = ");
    Serial.println(avgY);
    Serial.print("Red = ");
    Serial.println(avgR);
    Serial.print("Black = ");
    Serial.println(avgB);
  }

  delay(100);
}

float calcSpeed(float w, float y, float r, float b, float sp) {
  float LRDiff = abs(w-y) + abs(r-b); // check for significat turn
  float FWDiff = (w+y) / (r+b); // check for forward lean

  if (LRDiff < 0.4) { // arbitrary tilt threshold
     sp += (FWDiff - 1) * 0.2; // arbitrary acceleration multiplier
    if (sp > 1) {
      sp = 1;
    }
  }

  return sp;
}
