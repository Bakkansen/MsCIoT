#include <RFduinoGZLL.h>
#include "PLAB_PushButton.h"

device_t role = HOST;

const int ledPin = 6;
const int buttonPin = 2;

PLab_PushButton button(buttonPin);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  RFduinoGZLL.begin(role);
}

void loop(){
  button.update();
  if (button.pressed()) {
    if (RFduinoGZLL.sendToDevice(DEVICE0, "#test")){
      Serial.println("Queued Msg");
    } else {
      Serial.println("Failed to Queue Msg");
    }
  }
}

void RFduinoGZLL_onReceive(device_t device, int rssi, char *data, int len) {
  if (device = DEVICE0) {
   Serial.println("Received msg from Device"); 
  }  
}

