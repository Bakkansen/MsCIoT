#include <RFduinoGZLL.h>
#include "PLAB_PushButton.h"

device_t role = HOST;

const int ledPin = 6;
const int buttonPin = 2;

// LED blink variables
bool isBlinking = false;
int ledState = LOW;
unsigned long blinkMillis = 0;
const long blinkInterval = 200;
int blinkCount = 0;
const int blinkAmount = 4;

// Operation mode of the RFduino
int mode = 0; // 0 = Start, 1 = GameMode, 2 = Completed

// PushButton setup
PLab_PushButton button(buttonPin);

// List of connected devices
int devices[8] = {0, 0, 0, 0, 0, 0, 0, 0};
device_t devList[8] = {DEVICE0, DEVICE1, DEVICE2, DEVICE3, DEVICE4, DEVICE5, DEVICE6, DEVICE7};

// Red Dot Variables
int dots = 0;
int timeouts = 0;
const int N = 10;
unsigned long startTime = 0;
unsigned long finishTime = 0;


void setup() {
  // put your setup code here, to run once:  
  pinMode(ledPin, OUTPUT);
  Serial.begin(9600);
  RFduinoGZLL.begin(role);
}

void loop(){
  button.update();
  if (mode == 0) {
    if (button.pressed()) {
      mode = 1;
      isBlinking = false;
      digitalWrite(ledPin, LOW);
      startTime = millis();
      Serial.println("Starting game mode");
      sendNewDot();
    }
    if (isBlinking) {
      unsigned long currentMillis = millis();
      if (currentMillis - blinkMillis >= blinkInterval) {
        blinkMillis = currentMillis;
      
        if (blinkCount < blinkAmount) {
          blinkCount += 1;
          ledState = !ledState;
          digitalWrite(ledPin, ledState);
        } else {
          isBlinking = false;
          blinkCount = 0;          
        }
      }
    }  
  } else if (mode == 1 || mode == 2) {
    if (button.pressed()) {
      resetGame();
    }    
  }
  
  
}

void RFduinoGZLL_onReceive(device_t device, int rssi, char *data, int len) {
  String str = data;    
  str = str.substring(0, 2);
  if (mode == 0) {
    if (str.equals("#c")) {
      connectDevice(device);
      Serial.print("Connecting device :");
      Serial.println(device);
    }
  } else if (mode == 1) {
    
    if (str.equals("#p")) {
      Serial.print("Received #p from: ");
      Serial.println(device);
      dots += 1;
      
      if (dots >= N) {
        finishGame();
      } else {
        sendNewDot();
      }
      
    } else if (str.equals("#t")) {
      Serial.print("Received #t from: ");
      Serial.println(device);
      dots += 1;
      timeouts += 1;
      
      if (dots >= N) {
        finishGame();
      } else {
        sendNewDot();
      }
    }
    
  }
}

void connectDevice(device_t device) {
  switch (device){
    case DEVICE0:
      devices[0] = 1;
      break;
    case DEVICE1:
      devices[1] = 1;
      break;
    case DEVICE2:
      devices[2] = 1;
      break;
    case DEVICE3:
      devices[3] = 1;
      break;
    case DEVICE4:
      devices[4] = 1;
      break;
    case DEVICE5:
      devices[5] = 1;
      break;
    case DEVICE6:
      devices[6] = 1;
      break;
    case DEVICE7:
      devices[7] = 1;
      break;
  }
}

void resetGame() {
  memset(devices, 0, sizeof(devices));
  dots = 0;
  timeouts = 0;
  startTime = 0;
  finishTime = 0;
  Serial.println("Game Reset, reconnect devices...");
  mode = 0;
}

void finishGame() {
  mode = 2;
  Serial.println("Game Finished!");
  /*
  finishTime = millis();
  float avgPressTime = (finishTime - startTime) / N;
  
  Serial.print("N: ");
  Serial.println(N);
  Serial.print("AvgPressTime: ");
  Serial.println(avgPressTime);
  Serial.print("Timeouts: ");
  Serial.println(timeouts);
  */
}

void sendNewDot() {
  long randNumber = random(8);
  if (devices[randNumber] == 1) {
    if (RFduinoGZLL.sendToDevice(devList[randNumber], "#d")) {
      Serial.print("Sent Dot to Device: ");
      Serial.println(randNumber);
      return;
    } else {
      Serial.print("Failed to send Dot to Deivce: ");
      Serial.println(randNumber);
    }
  } 
  sendNewDot();
}

void BlinkLed() {
  // if we're not in Start Mode, return.
  if (mode != 0) {
    return;
  }
  // If we're already blinking, add two additional blinks
  if (isBlinking) {
    blinkCount -= 4;
  // If we're not blinking, start blinking two times from now
  } else {    
    isBlinking = true;
    blinkCount = 0;
    blinkMillis = millis();  
    ledState = HIGH;
    digitalWrite(ledPin, ledState);
  }  

}

