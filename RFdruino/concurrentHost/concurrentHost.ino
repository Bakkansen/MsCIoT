#include <RFduinoGZLL.h>

int messagesReceived = 0;

int messagesSent = 0;
const int sendInterval = 500;
unsigned long prevMillis = 0;
byte buf[1];

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  RFduinoGZLL.begin(HOST);
}

void loop(){
    unsigned long currentMillis = millis(); 
    if (currentMillis - prevMillis >= sendInterval) {
      prevMillis = currentMillis;
      buf[1] = messagesSent;
      Serial.println(buf[1]);
      if (RFduinoGZLL.sendToDevice(DEVICE0, (char *)buf), sizeof(buf)) {
        messagesSent += 1; 
      }      
  }
}

void RFduinoGZLL_onReceive(device_t device, int rssi, char *data, int len) {
  if (len > 0) {
    messagesReceived += 1;
    String str = data;
    String p = "ID: " + device;
    p = p + "M: " + messagesReceived;
    p = p + " Data: " + str;
    // Serial.println(p);
    // RFduinoGZLL.sendToDevice(device, str);
  } else {
    // Serial.println("Received msg with len = 0");
  }
}

