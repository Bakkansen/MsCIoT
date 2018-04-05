#include <RFduinoGZLL.h>
#include "PLAB_PushButton.h"

device_t role = DEVICE0;

const int ledPin = 6;
const int buttonPin = 2;

// PushButton setup
PLab_PushButton button(buttonPin);

// Game Variables
bool hasTheRedDot = false;
unsigned long timeout = 0;
const long timeoutInterval = 5000;

// Operating mode
int mode = 0; // 0 = Connection mode, 1 = Game mode

void setup() {
  // put your setup code here, to run once:  
  pinMode(ledPin, OUTPUT);
  Serial.begin(9600);
  RFduinoGZLL.begin(role);
}


void loop(){
  button.update();
  // Connection mode code
  if (mode == 0) {
    if (button.pressed()) {
      RFduinoGZLL.sendToHost("#c");
      mode = 1;
    }

  // Game mode code
  } else if (mode == 1) {
    
    if (hasTheRedDot) {      
      unsigned long currentMillis = millis();
      if (currentMillis - timeout >= timeoutInterval) {
        if (hasTheRedDot) {
          RFduinoGZLL.sendToHost("#t");  
        }
        hasTheRedDot = false;
        digitalWrite(ledPin, LOW);
        
        
        
      } else if (button.pressed()){
        hasTheRedDot = false;
        digitalWrite(ledPin, LOW);
        RFduinoGZLL.sendToHost("#p");
        
      }
    } else {
      digitalWrite(ledPin, LOW);
      RFduinoGZLL.sendToHost(NULL, 0);
      delay(100);
    }
  }
}

void RFduinoGZLL_onReceive(device_t device, int rssi, char *data, int len) {
  String str = data;
  str = str.substring(0, 2);
  if (str.equals("#d")) {
    hasTheRedDot = true;
    digitalWrite(ledPin, HIGH);
    timeout = millis();
  }
  if (str.equals("#g")) {
    hasTheRedDot = false;
    mode = 0;    
  }
}

