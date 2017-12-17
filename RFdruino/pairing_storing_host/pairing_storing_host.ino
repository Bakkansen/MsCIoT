#include <RFduinoGZLL.h>
#include "PLAB_PushButton.h"
// ------------------------
// Hardware variables
// ------------------------
#define PAIRING_LED 5
#define PAIRING_BUTTON 2

PLab_PushButton pairingButton(PAIRING_BUTTON);

// LED-blinking intervals
unsigned long prevBlink = 0;      // time since last PAIRING_LED update
const long blinkInterval = 1000;  // interval at which to blink (ms)


// ------------------------
// Flash access and storage
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
// Modes of operation
// ------------------------

#define OP_MODE 0     // Operating mode
#define PARING_MODE 1
#define SNIFF_MODE 2  // Check if there are other hosts operating on pairing address

int mode = OP_MODE; // Initialize in OP_MODE

// SNIFF_MODE variables
unsigned long sniffStart = 0;     // time when SNIFF_MODE started
const long sniffTime = 2000;       // amount of time to stay in SNIFF_MODE
unsigned long prevEcho = 0;      // time since last ECHO sent in SNIFF_MODE
const long echoInterval = 200;   // interval at which to send ECHO msg when in SNIFF_MODE

void setMode(int m) {   
  int prevMode = mode;
    
  switch (m) {
    case 0:
      mode = OP_MODE;
      digitalWrite(PAIRING_LED, LOW);
      break;
    
    case 1:
      mode = PAIRING_MODE;
      if (prevMode == OP_MODE) {
        prevBlink = millis();    
        checkAddressAvailability(HBPA, DBPA);
      }
      break;

    case 2:
      mode = SNIFF_MODE;
      sniffStart = millis();
      break;
      
    default:
      Serial.println("No valid state selected");
  }  
}

void togglePairingMode() {
  if (mode == PAIRING_MODE || mode == SNIFF_MODE) {
    setMode(OP_MODE);
  } else if (mode == OP_MODE) {
    setMode(PAIRING_MODE);
  }
} 

// ------------------------
// GZLL 
// ------------------------


// Default values set for the base addresses of the host and devices in pairing
// mode. 
#define HBPA 0x12345678
#define DBPA 0x98765432
#define PAIRING_MSG_ID 101;

device_t role = HOST;
unsigned long connectedDevices[8] = {0, 0, 0, 0, 0, 0, 0, 0};
unsigned long lastConnectionCheck = 0;      // Time since last connection check
const long connectionCheckInterval = 5000;  // Interval at which a connection check is performed

unsigned long hostAddresses[10] = {0x22345678, 0x32345678, 0x42345678, 0x52345678, 0x62345678, 0x72345678, 0x82345678, 0x912345678, 0x13345678, 0x14345678};
unsigned long devAddresses[10] = {0x98765433, 0x98765434, 0x98765435, 0x98765436, 0x98765437, 0x98765438, 0x98765439, 0x98765430, 0x98765442, 0x98765452};

struct gzllInfoPckt_t {
  int pairingID;
  int hostBaseAddress;
  int devBaseAddress;
  int role;
}


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


void checkAddressAvailability(int hostAddr, int devAddr) {  
  restartGZLL(hostAddr, devAddr, DEVICE0);  
  setMode(SNIFF_MODE);
}

void restartGZLL(int hostAddr, int devAddr, device_t r) {
  RFduinoGZLL.end();
  RFduinoGZLL.hostBaseAddress = hostAddr;
  RFduinoGZLL.deviceBaseAddress = devAddr;
  role = r;
  RFduinoGZLL.begin(role);
}

void handlePollRequests(device_t device) {
  
}

bool receivedMsg(devic_et device) {
  switch (device) {
      case DEVICE0:
        connectedDevices[0] = millis();
        break;
      
      case DEVICE1:
          connectedDevices[1] = millis();
        break;
      
      case DEVICE2:
          connectedDevices[2] = millis();
        break;
      
      case DEVICE3:
          connectedDevices[3] = millis();
        break;
      
      case DEVICE4:
          connectedDevices[4] = millis();
        break;
      
      case DEVICE5:
          connectedDevices[5] = millis();
        break;
      
      case DEVICE6:
          connectedDevices[6] = millis();
        break;
      
      case DEVICE7:
          connectedDevices[7] = millis();
        break;
      
      default:
        // do something
  }
}

// Returns the first available spot for a new device to connect
int getFirstAvailableSpot() {
  for(int i=0; i<8; i++){
      if (connectedDevices[i] == 0) {
        return i;
      }
  }
  return -1;
}

// Checks if its more than 5s since we last received a msg from a device, 
// if so we free up the spot for new devices to connect
void checkConnectedDevices() {
    long currentMillis = millis();
    for(int i=0; i<8; i++){
      long devTime = connectedDevices[i];
      if (devTime != 0 && (currentMillis - devTime > connectionCheckInterval)) {
        connectedDevices[i] = 0;
      }
  }
}

void RFduinoGZLL_onReceive(device_t device, int rssi, char *data, int len) {
  String str = data;
  receivedMsg(device);
  // SNIFF_MODE handling
  if (mode == SNIFF_MODE) {    
    if (device == HOST && str == "ECHO") {
      // Received indication that another host is in pairing mode, return to OP_MODE
      restartGZLL(config.hba, config.cba, HOST);
      setMode(OP_MODE);
    }

  } else if (mode == OP_MODE) {
  // OP_MODE handling
    if (len < 1) {
      handlePollRequests(device);
    }

    if (device != HOST && str == "ECHO") {
      RFduinoGZLL.sendToDevice(device, "ECHO");
    }

  
  } else if (mode == PAIRING_MODE) {
  // PAIRING_MODE handling
    if (str == "PAIR") {
      // Role = Get availabe spot (-1 indicates no available spots)
      // if (role != -1): Send json-obj with hba, dba and role
      int role = getFirstAvailableSpot();
      if (role > -1) {
        gzllInfoPckt_t pckt {PAIRING_MSG_ID, config.hba, config.cba, role };
        bool sendStatus = RFduinoGZLL.sendToDevice(device, (char *)&pckt, sizeof(pckt));
        // sendToDevice retuns true if the package was placed in the TX_FIFO, false if the TX_FIFO was full
        if (!sendStatus) {
          Serial.print("Failed to send pairing package to: " + device);
        }
      }
    }
  }

  
}


// ------------------------
// Setup and Loop
// ------------------------

void setup() {  
  pinMode(PAIRING_LED, OUTPUT);
  pinMode(PAIRING_BUTTON, INPUT);
  
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
  setMode(SNIFF_MODE);

}

void loop() {
  unsigned long currentMillis = millis();
  toggleButton.update();
  // LED code
  if (mode == PAIRING_MODE || mode == SNIFF_MODE) {
      if (currentMillis - prevBlink >= blinkInterval) {
        prevBlink = currentMillis;
        digitalWrite(PAIRING_LED, !digitalRead(PAIRING_LED));
      }
      
  }

  // Button code  
  if (button.pressed()) {
    togglePairingMode();
  }
  
  // SNIFF_MODE actions
  if (mode == SNIFF_MODE){
    if (currentMillis - sniffStart >= sniffTime) {
      restartGZLL(HBPA, DBPA, HOST);
      mode = OP_MODE;
    } else if (currentMillis - prevEcho >= echoInterval) {
        RFduino.sendToHost("ECHO");
        prevEcho = currentMillis;
      }     
  }
  
  
}

