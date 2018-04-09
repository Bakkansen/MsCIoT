#include <RFduinoGZLL.h>

device_t role = HOST;

void setup() { 
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

void processIncomingByte(const byte inByte) {
  switch (inByte) {
    case 'B':
      RFduinoGZLL.sendToDevice(DEVICE0, "#b");
      break;
    case 'C':
      RFduinoGZLL.sendToDevice(DEVICE0, "#c");
      break;
    default:
      break;
  }
}

void loop() {  
  while (Serial.available() > 0) {
    processIncomingByte(Serial.read());
  }


}

