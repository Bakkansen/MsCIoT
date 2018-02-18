#include <RFduinoGZLL.h>
#include "PLAB_PushButton.h"

device_t role = DEVICE2; // DEV0 = RED, DEV1 = GREEN, DEV2 = YELLOW

const int ledPin = 6;
const int buttonPin = 2;
const int pollInterval = 200;

boolean ledOn = false;
long previousMillis = 0;

PLab_PushButton button(buttonPin); // Create a PushButton object

// 
int devices[8] = {0, 0, 0, 0, 0, 0, 0, 0}

void setup() {
  // put your setup code here, to run once:
  pinMode(ledPin, OUTPUT);
  pinMode(buttonPin, INPUT);

  Serial.begin(9600);
  Serial.println("Starting up..");
  
  RFduinoGZLL.begin(role); 
}

void loop() {
  button.update();

  if (button.pressed()) {
    RFduinoGZLL.sendToHost("#toggleLed");
    toggleLed();
  }    
  // send null in order to let the host piggyback data
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis > pollInterval) {
     RFduinoGZLL.sendToHost(NULL, 0);
     previousMillis = currentMillis; 
  }
}

void RFduinoGZLL_onReceive(device_t device, int rssi, char *data, int len) {
  if (mode == 0) {
    String str = data;    
    str = str.substring(0, 2);
    if (str.equals("#c")) {
      
    }
  }
}

void toggleLed() {
  ledOn = !ledOn;
  digitalWrite(ledPin, ledOn);  
}

