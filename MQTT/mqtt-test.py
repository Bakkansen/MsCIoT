import paho.mqtt.client as mqtt
string topic = "masterTest/unity"

def on_connect(client, userdata, flags, rc):
    print("Connected with result code " + str(rc))
    client.subscribe(topic)

def on_message(client, userdata, msg):
    print("Received data from " + msg.topic + ": " + msg.payload)

client = mqtt.Client()
client.on_connect = on_connect
client.on_message = on_message

client.connect("mqtt.idi.ntnu.no", 1883, 60)

client.loop_forever()
