#include <RFduinoGZLL.h>
#include "PLAB_PushButton.h"
// ------------------------
// Hardware variables
// ------------------------
#define PAIRING_LED 5
#define TOGGLE_LED 6
#define TOGGLE_BUTTON 2

PLab_PushButton toggleButton(TOGGLE_BUTTON);

unsigned long prevButtonPress = 0;
// interval at which the button must be pressed to initiate pairing mode (ms)
byte pairPressInterval = 75; // 75 * 20 = 1500 (20 = debounce time)
byte pairPressCount = 0;
byte prevButtonState = HIGH;

// LED-blinking intervals
unsigned long prevBlink = 0;      // time since last TOGGLE_LED update
const long blinkInterval = 1000;  // interval at which to blink (ms)


// ------------------------
// GZLL information
// ------------------------
device_t role = HOST;

void RFduinoGZLL_onReceive(device_t device, int rssi, char *data, int len) {
  String str = data;
  Serial.println("Received: " + str);
}

// ------------------------
// Button-press handling
// ------------------------

void setup() {  
  pinMode(PAIRING_LED, OUTPUT);
  pinMode(TOGGLE_LED, OUTPUT);
  pinMode(TOGGLE_BUTTON, INPUT);
  
  Serial.begin(9600);
  Serial.println("Host starting up...");
  RFduinoGZLL.begin(role);
  

}

void loop() {
  unsigned long currentMillis = millis();
  // LED code

  // Button code
  byte currentButtonState = toggleButton.isDown();  
  if (button.pressed()) {
    pairPressCount = 0;
  } else if (button.released()) {    
    if (pairPressCount >= pairPressInterval) {
      longButtonPress();
    } else {      
      shortButtonPress();
    }
  } else if (currentButtonState == LOW) {
    pairPressCount++;
  }
  prevButtonState = currentButtonState;
  
  
}
