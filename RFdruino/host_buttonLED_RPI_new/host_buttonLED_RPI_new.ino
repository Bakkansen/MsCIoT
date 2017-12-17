#include <RFduinoGZLL.h>
#include "PLAB_PushButton.h"

device_t role = HOST;

int buttonState = 0;

const int rxPin = 0;
const int txPin = 1;

boolean dev0HasMessage = false;
boolean dev1HasMessage = false;
boolean dev2HasMessage = false;

boolean newData = false;

String dev0SendString = "test";
String dev1SendString = "";
String dev2SendString = "";

String readString = "";

const byte numChars = 32;
char receivedChars[numChars];

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  //Serial.println("Starting up host...");
  RFduinoGZLL.begin(role);
}

void loop() {
  recvWithStartEndMarkers();
  if (newData == true) {
    String str(receivedChars);
    handleSerialData(str);     
  }
}

void RFduinoGZLL_onReceive(device_t device, int rssi, char *data, int len) {
  // if we receive a poll, send toggle message if registered.
  if (len < 1) {
    handlePollRequests(device);
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
void handlePollRequests(device_t device) {
  if (device == DEVICE0 && dev0HasMessage == true) {
    // Serial.println("Sending GZLLmsg to DEV0");     
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

void recvWithStartEndMarkers() {
    static boolean recvInProgress = false;
    static byte ndx = 0;
    char startMarker = '<';
    char endMarker = '>';
    char rc;
 
 // if (Serial.available() > 0) {
    while (Serial.available() > 0 && newData == false) {
        rc = Serial.read();
        Serial.println("Char: " + rc);
        if (recvInProgress == true) {
            if (rc != endMarker) {
                receivedChars[ndx] = rc;
                ndx++;
                if (ndx >= numChars) {
                    ndx = numChars - 1;
                }
            }
            else {
                receivedChars[ndx] = '\0'; // terminate the string
                recvInProgress = false;
                ndx = 0;
                newData = true;
            }
        }

        else if (rc == startMarker) {
            recvInProgress = true;
        }
    }
}

// Handler for received serial data
void handleSerialData(String str) { 
  Serial.println("HandleSerialData!"); 
  newData = false;
  String devID = str.substring(0, 4);
  devID.toUpperCase();
  Serial.println("DEVID: " + devID + ". Msg: " + str.substring(5));
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

