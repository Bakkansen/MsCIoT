#include <RFduinoGZLL.h>
#include "PLAB_PushButton.h"

device_t role = HOST;

const int redButtonPin = 2;
const int greenButtonPin = 3;
const int yellowButtonPin = 4;
int buttonState = 0;

const int rxPin = 0;
const int txPin = 1;

int toggleDev0 = 0; // DEVICE0 RED
int toggleDev1 = 0; // DEVICE1 GREEN
int toggleDev2 = 0; // DEVICE2 YELLOW

String readString = "";

PLab_PushButton redButton(redButtonPin); // Create a PushButton object
PLab_PushButton greenButton(greenButtonPin); // Create a PushButton object
PLab_PushButton yellowButton(yellowButtonPin); // Create a PushButton object

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  //Serial.println("Starting up host...");
  RFduinoGZLL.begin(role);
}

void loop() {
  // put your main code here, to run repeatedly: 
  if (Serial.available())  {
    char c = Serial.read();   // gets one byte from serial buffer
    if (c == '\n') {           // serial always ends on 'X'
      handleSerialData(readString);
      readString="";          // clears variable for new input      
    } else if (c != '\r') {     
      readString += c;        // makes the string readString
    }
  }  
  redButton.update();  
  if (redButton.pressed()){        
    toggleDev0 = 1;
  }
  greenButton.update();
  if (greenButton.pressed()){        
    toggleDev1 = 1;
  }
  yellowButton.update();
  if (yellowButton.pressed()){        
    toggleDev2 = 1;
  }
}

void RFduinoGZLL_onReceive(device_t device, int rssi, char *data, int len) {
  // if we receive a poll, send toggle message if registered.
  if (len < 1) {
    handlePollRequests(device, data);        
  // If we receive anything else than a NULL msg, handle it
  } else {
    // SETT OPP SLIK AT SERIAL SENDER MELDINGER TILBAKE TIL ARDUNIO
    String str = data;    
    str = str.substring(0, 10);
    if (str.equals("#toggleLed")) {
      if (device == DEVICE0) {
        Serial.println("#toggleRedLed");
      } else if (device == DEVICE1) {
        Serial.println("#toggleGreenLed");
      } else if (device == DEVICE2) {
        Serial.println("#toggleYellowLed");
      }
    }    
  }  
}

// Piggy-backs toggle led message if toggle led for device is registered.
void handlePollRequests(device_t device, char *data) {
  if (device == DEVICE0 && toggleDev0 == 1) {
    //Serial.println("Sending msg to DEV0");     
    RFduinoGZLL.sendToDevice(DEVICE0, "#toggleLed");
    toggleDev0 = 0;
  } else if (device == DEVICE1 && toggleDev1 == 1) {
    //Serial.println("Sending msg to DEV1");     
    RFduinoGZLL.sendToDevice(DEVICE1, "#toggleLed");
    toggleDev1 = 0;
  } else if (device == DEVICE2 && toggleDev2 == 1) {
    //Serial.println("Sending msg to DEV2");     
    RFduinoGZLL.sendToDevice(DEVICE2, "#toggleLed");
    toggleDev2 = 0;
  }
}

// Handler for received serial data
void handleSerialData(String str) {
  Serial.println("'" +str + "'");
  if (str.equals("#toggleRedLed")) {
    toggleDev0 = 1;
    // Serial.println("r");
  } else if (str.equals("#toggleGreenLed")) {
    toggleDev1 = 1;      
  } else if (str.equals("#toggleYellowLed")) {
    toggleDev2 = 1;
  }
}

