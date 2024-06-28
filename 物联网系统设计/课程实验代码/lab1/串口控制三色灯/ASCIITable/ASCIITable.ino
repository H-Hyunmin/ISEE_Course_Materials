#include <ArduinoJson.h>

const int redPin = 8;
const int greenPin = 7;
const int bluePin = 9;

void setup() {
  // initialize serial:
  Serial.begin(115200);
  // make the pins outputs:
  pinMode(redPin, OUTPUT);
  pinMode(greenPin, OUTPUT);
  pinMode(bluePin, OUTPUT);
}

void loop() {
  // if there's any serial available, read it:
  while (Serial.available() > 0) {

    // look for the next valid integer in the incoming serial stream:
    int red = Serial.parseInt();
    // do it again:
    int green = Serial.parseInt();
    // do it again:
    int blue = Serial.parseInt();

    Serial.println(red);
    Serial.println(green);
    Serial.println(blue);
    
    analogWrite(redPin, red);
    analogWrite(greenPin, green);
    analogWrite(bluePin, blue);
    }
  }