#include <RFduinoGZLL.h>

device_t role = HOST;

typedef enum { NONE, DEV, MSG } states;
states state = NONE;


const char startChar = '<';
const char stopChar = '>';
const char delimiter = ':';

unsigned int index_pos = 0;

const unsigned int MAX_DEV_INPUT = 5;
const unsigned int MAX_MSG_INPUT = 30;

char dev[MAX_DEV_INPUT];      // Array to keep the device info received
char msg[MAX_MSG_INPUT];    // Array to keep the component name



void setup() { 
  state = NONE;
  Serial.begin(115200);
  RFduinoGZLL.begin(role);
}

void RFduinoGZLL_onReceive(device_t device, int rssi, char *data, int len) {
  String str = data;
  str = str.substring(0, 2);
  if (str.equals("#b")) {
    Serial.println('B');
  } else if (str.equals("#c")) {
    Serial.println('C');
  }
}

void handleDelimiter() {
  index_pos = 0;
  if (state == DEV) {
    state = MSG;
  }
}

void handleIncomingData(const byte inByte) {
  switch (state) {
    case DEV:
      if (index_pos < (MAX_DEV_INPUT - 1)) {
        dev[index_pos++] = inByte;
      }
      break;

    case MSG:
      if (index_pos < (MAX_COMP_INPUT - 1)) {
        msg[index_pos++] = inByte;
      }
      break;

}

void handleStopCharReceived() {
  String device(dev);
  device_t receiver = getDevicetFromString(device);
  
  if (receiver != HOST) {
    RFduinoGZLL.sendToDevice(receiver, msg);
  }
  state = NONE;
}

device_t getDevicetFromString(string device) {
  device_t receiver = HOST;
  switch (device)  {
    case "DEV0":
    receiver = DEVICE0;
    break;

    case "DEV1":
    receiver = DEVICE1;
    break;

    case "DEV2":
    receiver = DEVICE2;
    break;

    case "DEV3":
    receiver = DEVICE3;
    break;

    case "DEV4":
    receiver = DEVICE4;
    break;

    case "DEV5":
    receiver = DEVICE5;
    break;

    case "DEV6":
    receiver = DEVICE6;
    break;

    case "DEV7":
    receiver = DEVICE7;
    break;
  }
  return receiver;
}

void processIncomingByte(const byte inByte) {
  
  switch (inByte) {
  
    case startChar:
      state = DEV;    
      break;
    
    case stopChar:
      handleStopCharReceived();
      break;

    case delimiter:
      handleDelimiter();
      
    default:
      handleIncomingData(inByte);
      break;
  }
}

void loop() {  
  while (Serial.available() > 0) {
    processIncomingByte(Serial.read());
  }


}

