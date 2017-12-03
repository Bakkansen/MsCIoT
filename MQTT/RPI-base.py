import paho.mqtt.client as mqtt
import paho.mqtt.publish as publish
import time
import serial

#MQTT Information
#Topic stucture: Pettjo/Type/Environment/RPI/Dev ex: Pettjo/Action/House/RPI1/Dev0
mqttBrokerAddress = "mqtt.idi.ntnu.no"
listenTopic = "Pettjo/#"


#Devlist
# Dev0 = Green
# Dev1 = Red
# Dev2 = Yellow
greenMsg = "#toggleGreenLed"
redMsg = "#toggleRedLed"
yellowMsg = "#toggleYellowLed"

devList = { "DEV0": [greenMsg, "LED", "Off"], "DEV1": [redMsg, "LED", "Off"], "DEV2": [yellowMsg, "LED", "Off"]}


#RPI Information
rpiID = "RPI1"
Environment = "House"


#Serial Information
ser = serial.Serial(
	port='/dev/ttyAMA0',
	baudrate = 9600,
	parity=serial.PARITY_NONE,
	stopbits=serial.STOPBITS_ONE,
	bytesize=serial.EIGHTBITS,
	timeout=1
	)


def on_connect(client, userdata, flags, rc):
    print("Connected with result code " + str(rc))
    client.subscribe(listenTopic)


def on_message(client, userdata, msg):
    print("Received data from " + msg.topic + ": " + msg.payload)
    HandleMQTTMessage(msg)


def HandleMQTTMessage(msg):
	topicSplit = msg.topic.split('/')
	msgType = "#"
	msgEnvironment = "#"
	msgRPI = "#"
	msgDev = "#"
	if (len(topicSplit) > 1):
		msgType = topicSplit[1]
	if (len(topicSplit) > 2):
		msgEnvironment = topicSplit[2]
	if (len(topicSplit) > 3):
		msgRPI = topicSplit[3]
	if (len(topicSplit) > 4):
		msgDev = topicSplit[4].upper()

	if (msgRPI != rpiID and msgRPI != "#"):
		return

	if (msgType == "Action"):
		if (msgDev == "#"):
			for dev in devList.keys():
				ActionMsgRcvd(dev, msg.payload)
		elif (msgDev in devList.keys()):
			ActionMsgRcvd(msgDev, msg.payload)


def ActionMsgRcvd(dev, msg):
	# MQTT msg format: "Comp:CompState"
	print("Recvd MQTTmsg: [" + dev + "] : [" + msg + "]")
	parsedMsg = msg.split(':')
	if (parsedMsg[0].upper() in devList[dev]):
		sendString = dev + ":" + parsedMsg[0] + ":" + parsedMsg[1]
		SendSerialMsg(sendString)


def HandleSerialMessage(msg):
	print("Received Serial message: [" + msg + "]")
	parsedMsg = msg.split(':')
	dev = parsedMsg[0].upper()
	comp = parsedMsg[1]
	compState = parsedMsg[2]
	if dev in devList:
		try:
			index = devList[dev].index(comp)
			if (devList[dev][index + 1] != compState):
				devList[dev][index + 1] = compState
				PublishMQTTMsg("Action", comp + ":" + compState, dev)
		except ValueError:
			pass



def SendSerialMsg(serialMsg):
	print("Sent serial message: [" + serialMsg + "]")
	ser.write(serialMsg.encode())


def PublishMQTTMsg(msgType, payload, dev):
	if (dev in devList.keys()):
		topic = "Pettjo/" + msgType + "/" + Environment + "/" + rpiID + "/" + dev		
		print("Published mqtt message: [" + topic + "]: " + "[" + payload + "]")
		publish.single(topic, payload, hostname="mqttBrokerAddress")


client = mqtt.Client()
client.on_connect = on_connect
client.on_message = on_message

client.connect(mqttBrokerAddress, 1883, 60)

client.loop_forever()
print("MQTT client connected!")

try:
	while True:
		read_serial = ser.readline()
		HandleSerialMessage(read_serial)
except (KeyboardInterrupt, SystemExit):
	raise
