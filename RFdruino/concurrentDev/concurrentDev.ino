#include <RFduinoGZLL.h>

int messagesReceived = 0;
int messagesSent = 10;
const int sendInterval = 100;
unsigned long prevMillis = 0;


void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  RFduinoGZLL.begin(DEVICE0);
}

void loop(){  
    unsigned long currentMillis = millis(); 
    if (currentMillis - prevMillis >= sendInterval) {
      prevMillis = currentMillis;
      RFduinoGZLL.sendToHost(NULL, 0);
      RFduinoGZLL.sendToHost(NULL, 0);
    }  
}

void RFduinoGZLL_onReceive(device_t device, int rssi, char *data, int len) {
  if (len > 0) {
    messagesReceived += 1;
    String str = data;   
    String p = "ID: HOST  M: " + messagesReceived;;
    p = p + "  Data: ";
    p = p + str;
    if (device = HOST) {
      Serial.println(data[0]);  
    }
    
  } else {
    byte val = data[0];
    Serial.println(val);  
  }
}

