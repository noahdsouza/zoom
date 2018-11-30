const int white = A2;
const int yellow = A3;
const int red = A4;
const int black = A5;
const int motor1 = 9;

int readIndex = 0; // used for rolling average
const int samples = 10; // smaples to average over
int readIndex2 = 0; // used for rolling average
const int samples2 = 100; // samples to average over

float speed = 0; // a speed value from 0-1
int output = 0; // the value written to the motors
int PWRange = 600; // range of PWM signal
int PWMin = 1400; // minimum PWM value

int readingsW[samples]; int totalW = 0; int avgW = 0; float varW = 1;
int readingsW2[samples2]; long totalW2 = 0L; int avgW2 = -1;
int readingsY[samples]; int totalY = 0; int avgY = 0; float varY = 1;
int readingsY2[samples2]; long totalY2 = 0L; int avgY2 = -1;
int readingsR[samples]; int totalR = 0; int avgR = 0; float varR = 1;
int readingsR2[samples2]; long totalR2 = 0L; int avgR2 = -1;
int readingsB[samples]; int totalB = 0; int avgB = 0; float varB = 1;
int readingsB2[samples2]; long totalB2 = 0L; int avgB2 = -1;

void setup() {
  Serial.begin(9600);
  // initialize the buffer for calculating the rolling average
  for (int i = 0; i < samples; i++) {
    readingsW[i] = 0;
    readingsY[i] = 0;
    readingsR[i] = 0;
    readingsB[i] = 0;
  }
  for (int j = 0; j < samples2; j++) {
    readingsW2[j] = 0;
    readingsY2[j] = 0;
    readingsR2[j] = 0;
    readingsB2[j] = 0;
  }
}

void loop() {
  // basically, these calculate the rolling averages for each sensor
  totalW -= readingsW[readIndex];
  readingsW[readIndex] = analogRead(white);
  totalW += readingsW[readIndex];
  avgW = totalW / samples;

  totalW2 -= readingsW2[readIndex2];
  readingsW2[readIndex2] = analogRead(white);
  totalW2 += readingsW2[readIndex2];
  avgW2 = totalW2 / samples2;

  totalY -= readingsY[readIndex];
  readingsY[readIndex] = analogRead(yellow);
  totalY += readingsY[readIndex];
  avgY = totalY / samples;

  totalY2 -= readingsY2[readIndex2];
  readingsY2[readIndex2] = analogRead(yellow);
  totalY2 += readingsY2[readIndex2];
  avgY2 = totalY2 / samples2;

  totalR -= readingsR[readIndex];
  readingsR[readIndex] = analogRead(red);
  totalR += readingsR[readIndex];
  avgR = totalR / samples;

  totalR2 -= readingsR2[readIndex2];
  readingsR2[readIndex2] = analogRead(red);
  totalR2 += readingsR2[readIndex2];
  avgR2 = totalR2 / samples2;

  totalB -= readingsB[readIndex];
  readingsB[readIndex] = analogRead(black);
  totalB += readingsB[readIndex];
  avgB = totalB / samples;

  totalB2 -= readingsB2[readIndex2];
  readingsB2[readIndex2] = analogRead(black);
  totalB2 += readingsB2[readIndex2];
  avgB2 = totalB2 / samples2;

  readIndex = (readIndex + 1) % samples;
  readIndex2 = (readIndex2 + 1) % samples2;

  // Calculate the variance from baseline of each sensor
  varW = (float) avgW / (float) avgW2;
  varY = (float) avgY / (float) avgY2;
  varR = (float) avgR / (float) avgR2;
  varB = (float) avgB / (float) avgB2;
  
  speed = calcSpeed(varW, varY, varR, varB, speed);

  // map it to the range of the pwm output:
  output = speed * PWRange + PWMin;
  analogWrite(motor1, output);

  // print the results to the Serial Monitor every few cycles:
  if (readIndex == 0) {
    Serial.print("Speed = ");
    Serial.println(speed);
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
