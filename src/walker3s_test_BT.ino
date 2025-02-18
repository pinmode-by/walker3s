// Walker 3s test
// button control
// Arduino nano, 2*TA6586, Servo, 2 LED, 1 button
// 18.02.2025


#include <Servo.h>
#include <FastLED.h>

#define COLOR_ORDER GRB
#define CHIPSET WS2812
#define NUM_LEDS 8
#define BRIGHTNESS 200
#define FRAMES_PER_SECOND 60

Servo myservo;
CRGB leds[NUM_LEDS];
const byte NUM_LINE = 4;
int line[NUM_LINE];

const float kV = 0.008;

const byte LED1 = 9;
const byte LED2 = 10;
const byte BUTTON = 2;
const byte ML1 = 3;
const byte ML2 = 5;
const byte MR1 = 6;
const byte MR2 = 11;
const byte SERVO = 12;
const byte SLED_PIN = 8;
const byte VMETER = A7;
byte linePin[NUM_LINE] = { A0, A1, A2, A3 };

bool state = false;

void readLine() {  //read line sensors
  for (int i = 0; i < NUM_LINE; i++) {
    line[i] = analogRead(linePin[i]);
  }
}

void printLine() {  // read and print line sensors
  readLine();
  for (int i = 0; i < NUM_LINE; i++) {
    Serial.print(line[i]);
    Serial.print("  ");
  }
  Serial.println();
}

float getVoltage() {  // returns voltage of power supply
  return kV * analogRead(VMETER);
}

void drive(int left, int right) {  // motor control
  left = constrain(left, -255, 255);
  right = constrain(right, -255, 255);

  if (left > 0) {
    digitalWrite(ML1, 1);
    analogWrite(ML2, 255 - left);
  } else {
    digitalWrite(ML2, 1);
    analogWrite(ML1, 255 + left);
  }

  if (right > 0) {
    digitalWrite(MR1, 1);
    analogWrite(MR2, 255 - right);
  } else {
    digitalWrite(MR2, 1);
    analogWrite(MR1, 255 + right);
  }
}  // end of drive()

void fillBlue() {  // fill smart led with blue color
  for (int j = 0; j < NUM_LEDS; j++) {
    leds[j] = CRGB::Blue;
  }
  FastLED.show();  // display this frame
}

void setup() {
  Serial.begin(9600);
  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
  pinMode(ML1, OUTPUT);
  pinMode(ML2, OUTPUT);
  pinMode(MR1, OUTPUT);
  pinMode(MR2, OUTPUT);
  pinMode(BUTTON, INPUT_PULLUP);
  myservo.attach(SERVO);
  FastLED.addLeds<CHIPSET, SLED_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
  FastLED.setBrightness(BRIGHTNESS);
}  // end of setup()

void loop() {
  if (Serial.available()) {
    char ch = Serial.read();
    if (ch == 'i') {
      //get info
      Serial.print("Voltage=");
      Serial.println(getVoltage());
      printLine();
    } else if (ch == 'r') {
      //read sensors
      printLine();
      drive(0, 0);
    } else if (ch == 'L') {
      //left motor
      digitalWrite(LED1, 1);
      digitalWrite(LED2, 0);
      drive(100, 0);
    } else if (ch == 'R') {
      //right motor
      digitalWrite(LED1, 0);
      digitalWrite(LED2, 1);
      drive(0, 100);
    } else if (ch == 's') {
      //servo test
      drive(0, 0);
      for (int i = 0; i <= 180; i = i + 10) {
        myservo.write(i);
        delay(50);
      }
      delay(300);
      for (int i = 180; i >= 0; i = i - 10) {
        myservo.write(i);
        delay(50);
      }

    } else {
      //do nothing
      //drive(0, 0);
      //digitalWrite(LED1, 0);
      //digitalWrite(LED2, 0);
    }
  }
}
