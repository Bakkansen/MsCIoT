#include "PLAB_PushButton.h"

const int ledPin = 6;
const int buttonPin = 2;

bool ledOn = false;

PLab_PushButton button(buttonPin);

void setup() {
  pinMode(ledPin, OUTPUT);
  Serial.begin(115200);
}

void processIncomingByte(const byte inByte) {
  switch (inByte) {
    case 'B':
      ledOn = HIGH;
      digitalWrite(ledPin, ledOn);
      break;
    case 'C':
      ledOn = LOW;
      digitalWrite(ledPin, ledOn);
      break;
    default:
      break;
  }
}

void loop() {  
  button.update();
  if (button.pressed()) {
    ledOn = !ledOn;
    digitalWrite(ledPin, ledOn);
    if (ledOn) {
      Serial.println("B");
    } else {
      Serial.println("C");
    }
  }
  while (Serial.available() > 0) {
    processIncomingByte(Serial.read());
  }


}

