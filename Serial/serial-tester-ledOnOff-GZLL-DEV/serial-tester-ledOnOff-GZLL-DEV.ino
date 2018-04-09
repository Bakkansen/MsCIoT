#include <RFduinoGZLL.h>
#include "PLAB_PushButton.h"

device_t role = DEVICE0;

const int ledPin = 6;
const int buttonPin = 2;

bool ledOn = false;

// PushButton setup
PLab_PushButton button(buttonPin);

void setup() {  
  pinMode(ledPin, OUTPUT);
  Serial.begin(115200);
  RFduinoGZLL.begin(role);
}


void loop(){
  button.update();

  if (button.pressed()) {
    ledOn = !ledOn;
    digitalWrite(ledPin, ledOn);
    if (ledOn) {
      RFduinoGZLL.sendToHost("#b");
    } else {
      RFduinoGZLL.sendToHost("#c");
    }
  }
  RFduinoGZLL.sendToHost(NULL, 0);
  delay(200);
  
}

void RFduinoGZLL_onReceive(device_t device, int rssi, char *data, int len) {
  String str = data;
  str = str.substring(0, 2);
  if (str.equals("#b")) {
    ledOn = HIGH;
    digitalWrite(ledPin, ledOn);
  }
  if (str.equals("#c")) {
    ledOn = LOW;
    digitalWrite(ledPin, ledOn);
  }
}

