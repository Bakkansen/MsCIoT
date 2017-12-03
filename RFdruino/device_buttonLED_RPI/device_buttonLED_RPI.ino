#include <RFduinoGZLL.h>
#include "PLAB_PushButton.h"

device_t role = DEVICE2; // DEV0 = RED, DEV1 = GREEN, DEV2 = YELLOW

const int ledPin = 6;
const int buttonPin = 2;
const int pollInterval = 200;

boolean ledOn = false;
long previousMillis = 0;

PLab_PushButton button(buttonPin); // Create a PushButton object


void setup() {
  // put your setup code here, to run once:
  pinMode(ledPin, OUTPUT);
  pinMode(buttonPin, INPUT);

  Serial.begin(9600);
  Serial.println("Starting up..");
  
  RFduinoGZLL.begin(role); 
}

void toggleLed() {
  ledOn = !ledOn;
  digitalWrite(ledPin, ledOn);  
}

void loop() {
  button.update();

  if (button.pressed()) {
    toggleLed();
    String msg = "LED:";
    if (digitalRead(ledPin) == true) {
      msg = msg + "ON";
    } else {
      msg = msg + "OFF";
    }
    Serial.println("Sent: " + msg);
    RFduinoGZLL.sendToHost(msg);
  }    
  // send null in order to let the host piggyback data
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis > pollInterval) {
     RFduinoGZLL.sendToHost(NULL, 0);
     previousMillis = currentMillis; 
  }
}

void RFduinoGZLL_onReceive(device_t device, int rssi, char *data, int len) {
  // ignore acknowledgement without payload
  if (len > 0 && device == HOST) { // HOST = 8    
    String str = data;
    String comp = str.substring(0, 4);
    if (comp.equals("LED")) {
      String state = str.substring(5);
      state.toUpperCase();
      if (state.equals("ON")){
        digitalWrite(ledPin, true);
      } else if (state.equals("OFF")) {
        digitalWrite(ledPin, false);
      }
    }
  }
}



