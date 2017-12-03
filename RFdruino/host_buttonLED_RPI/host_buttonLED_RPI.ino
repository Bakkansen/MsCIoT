#include <RFduinoGZLL.h>
#include "PLAB_PushButton.h"

device_t role = HOST;

int buttonState = 0;

const int rxPin = 0;
const int txPin = 1;

bool dev0HasMessage = false;
bool dev1HasMessage = false;
bool dev2HasMessage = false;

String dev0SendString = "";
String dev1SendString = "";
String dev2SendString = "";

String readString = "";

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
}

void RFduinoGZLL_onReceive(device_t device, int rssi, char *data, int len) {
  // if we receive a poll, send toggle message if registered.
  if (len < 1) {
    handlePollRequests(device, data);        
  // If we receive anything else than a NULL msg, handle it
  } else {
    // SETT OPP SLIK AT SERIAL SENDER MELDINGER TILBAKE TIL ARDUNIO
    String str = data;    
    if (device == DEVICE0) {
      Serial.println("DEV0:" + str);
    } else if (device == DEVICE1) {
      Serial.println("DEV1:" + str);
    } else if (device == DEVICE2) {
      Serial.println("DEV2:" + str);
    }
  }  
}

// Piggy-backs toggle led message if toggle led for device is registered.
void handlePollRequests(device_t device, char *data) {
  if (device == DEVICE0 && dev0HasMessage == true) {
    //Serial.println("Sending msg to DEV0");     
    RFduinoGZLL.sendToDevice(DEVICE0, dev0SendString);
    dev0HasMessage = false;
    dev0SendString = "";
  } else if (device == DEVICE1 && dev1HasMessage == true) {
    //Serial.println("Sending msg to DEV1");     
    RFduinoGZLL.sendToDevice(DEVICE1, dev1SendString);
    dev1HasMessage = false;
    dev1SendString = "";
  } else if (device == DEVICE2 && dev2HasMessage == true) {
    //Serial.println("Sending msg to DEV2");     
    RFduinoGZLL.sendToDevice(DEVICE2, dev2SendString);
    dev2HasMessage = false;
    dev2SendString = "";
  }
}

// Handler for received serial data
void handleSerialData(String str) {
  String devID = str.substring(0, 4);
  devID.toUpperCase();
  // Serial.println("Recvd: " + devID + ":" + str.substring(5));
  if (devID = "DEV0"){
    dev0HasMessage = true;
    dev0SendString = str.substring(5);  
  }
  if (devID = "DEV1"){
    dev1HasMessage = true;
    dev1SendString = str.substring(5);  
  }
  if (devID = "DEV2"){
    dev2HasMessage = true;
    dev2SendString = str.substring(5);  
  }
}

