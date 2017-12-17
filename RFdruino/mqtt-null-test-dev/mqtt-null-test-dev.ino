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
device_t role = DEVICE0;


// ------------------------
// Button-press handling
// ------------------------

void longButtonPress() {
  Serial.println("LongButtonPress");
  Serial.println(pairPressCount);
  bool status = RFduinoGZLL.sendToHost(NULL, 0);
  if (status) {
    Serial.println("Sent NULL");
  } else {
    Serial.println("Failed to send NULL");
  }
  pairPressCount = 0;

}

void shortButtonPress() {
  int i = pairPressCount;
  String s = "ShortButtonPress: " + i;
  Serial.println(pairPressCount);
  bool status = RFduinoGZLL.sendToHost("HEI");
  if (status) {
    Serial.println("Sent Hei");
  } else {
    Serial.println("Failed to send HEI");
  }
  pairPressCount = 0;
}


void setup() {  
  pinMode(PAIRING_LED, OUTPUT);
  pinMode(TOGGLE_LED, OUTPUT);
  pinMode(TOGGLE_BUTTON, INPUT);
  
  Serial.begin(9600);

  Serial.println("DEVICE0 starting up...");

  RFduinoGZLL.begin(role);
  

}

void loop() {
  toggleButton.update();
  unsigned long currentMillis = millis();
  // LED code

  // Button code
  byte currentButtonState = toggleButton.isDown();  
  if (toggleButton.pressed()) {
    pairPressCount = 0;
  } else if (toggleButton.released()) {    
    if (pairPressCount >= pairPressInterval) {
      longButtonPress();
    } else {      
      shortButtonPress();
    }
  } else if (currentButtonState == HIGH) {
    pairPressCount++;
    Serial.println(pairPressCount);
  }
  prevButtonState = currentButtonState;
  
  
}
