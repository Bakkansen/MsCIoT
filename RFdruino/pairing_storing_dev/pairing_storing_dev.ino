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
byte pairPressInterval = 150; // 150 * 20 = 3000 (20 = debounce time)
byte pairPressCount = 0;
byte prevButtonState = HIGH;

// LED-blinking intervals
unsigned long prevBlink = 0;      // time since last TOGGLE_LED update
const long blinkInterval = 1000;  // interval at which to blink (ms)

// Different modes of operation for the host
#define OP_MODE 0     // Operating mode
#define PARING_MODE 1
#define SNIFF_MODE 2  // Check if there are other hosts operating on pairing address

int mode = OP_MODE;

void setMode(int m) {   
  if (mode == OP_MODE) {
    prevBlink = millis();
  }
  
  switch (m) {
    case 0:
      mode = OP_MODE;
      digitalWrite(PAIRING_LED, LOW);
      break;
    
    case 1:
      mode = PAIRING_MODE;
      break;

    case 2:
      mode = SNIFF_MODE;
      break;
      
    default:
      Serial.println("No valid state selected");
  }
  
}





// ------------------------
// Flash access
// ------------------------
// We use the last user accessible page (124-251) to avoid writing over it
// with sketches. 
#define FLASH_STORAGE 251


// double level of indirection required to get gcc
// to apply the stringizing operator correctly
#define str(x) xstr(x)
#define xstr(x) #x

// Data structured stored on the device
// Since the max transfer is <20 bytes over the radio,
// the lazy way is to make all the arrays 20 bytes long.
struct data_t {
  int role   // device1-8 = 0-7, host = 8
  int hba;   // host_base_address
  int dba;   // device_base_address  
  char valid[6];
}

void dump_data(struct data_t *p, bool force = false){
  if (String(p->valid) == "valid" || force) {
    Serial.print("Role = ");
    Serial.print(p->role);
    Serial.print("HPBA = ");
    Serial.print(p->hpba);
    Serial.print("DPBA = ");
    Serial.print(p->dpba);    
    if (force) {
      Serial.print("Valid = ");
      Serial.print(p->valid);
    }
  } else {
    Serial.println("No valid data in flash");
  }
}

// This is a pointer to the data stored in flash. This is read only
data_t *in_flash = (data_t*)ADDRESS_OF_PAGE(FLASH_STORAGE);

// This is our config struct. We will use this to store our config options in 
// memory for normal operations.
// When writing, the contents of this struct will overwrite the in_flash data.
// hba and dba are the default RFduino values
data_t config = {8, 0x0D0A0704, 0x0E0B0805, {""}}; // default data

// Copies char array src of length len to char array dst
void copyArray(char* src, char* dst, int len) {
  memcpy(dsp, src, sizeof(src[0])*len);
}


// ------------------------
// GZLL information
// ------------------------
device_t role = HOST;

// Default values set for the base addresses of the host and devices in pairing
// mode. 
#define HBPA 0x12345678
#define DBPA 0x98765432

void setGZLLRole(int r) {
  switch (r) {
    case 0:
      role = DEVICE1;
      break;

    case 1:
      role = DEVICE2;
      break;

    case 2:
      role = DEVICE3;
      break;

    case 3:
      role = DEVICE4;
      break;

    case 4:
      role = DEVICE5;
      break;

    case 5:
      role = DEVICE6;
      break;

    case 6:
      role = DEVICE7;
      break;

    case 7:
      role = DEVICE8;
      break;

    case 8:
      role = HOST;
      break;

    default:
      Serial.println("No valid role selected");
}

// ------------------------
// Button-press handling
// ------------------------

void longButtonPress() {
  
}

void shortButtonPress() {
  // Do normal button stuff
  // toggle led()
      
  // if mode == pairing || sniff => return to op_mode            
}


void setup() {  
  pinMode(PAIRING_LED, OUTPUT);
  pinMode(TOGGLE_LED, OUTPUT);
  pinMode(TOGGLE_BUTTON, INPUT);
  
  Serial.begin(9600);

  // Copy data from flash to config if there exists some data
  if (String(in_flash->valid) == "valid") {
    copyArray(in_flash->role, config.role, 20);
    copyArray(in_flash->hba, config.hba, 20);
    copyArray(in_flash->cba, config.cba, 20);
    copyArray(in_flash->valid, config.valid, 6);
  }

  setGZLLRole(config.role);
  RFduinoGZLL.begin(role);
  

}

void loop() {
  unsigned long currentMillis = millis();
  // LED code
  if (mode == PAIRING_MODE || mode == SNIFF_MODE) {
      if (currentMillis - prevBlink >= blinkInterval) {
        prevBlink = currentMillis;
      }
      digitalWrite(TOGGLE_LED, !digitalRead(TOGGLE_LED));
  }

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

void RFduinoGZLL_onReceive(device_t device, int rssi, char *data, int len) {
  
}

