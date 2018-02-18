#include <RFduinoGZLL.h>
#include "PLAB_PushButton.h"

device_t role = DEVICE0;

const int ledPin = 6;
const int buttonPin = 2;
String str = "";

PLab_PushButton button(buttonPin);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  RFduinoGZLL.begin(role);
}

void loop(){  
  button.update();
  if (button.pressed()){
    RFduinoGZLL.sendToHost(NULL, 0);
  }
}

void RFduinoGZLL_onReceive(device_t device, int rssi, char *data, int len) {  
  str = data;
  Serial.println(str);
  str = "";
}

