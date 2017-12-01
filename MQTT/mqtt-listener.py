import paho.mqtt.client as mqtt
from string import split
from sqlalchemy import Column, String, Integer, Boolean, ForeignKey
from sqlalchemy import create_engine
from sqlalchemy.ext.automap import automap_base
from sqlalchemy.orm import Session
import time, socket


Base = automap_base()
engine = create_engine('mysql://terjero_adm:garme8834.@mysql.idi.ntnu.no/p_inneklima')
Base.prepare(engine, reflect=True)
Sensor = Base.classes.sensors
session = Session(engine)



# The callback for when the client receives a CONNACK response from the server.
def on_connect(client, userdata, flags, rc):
	print("Connected with result code "+str(rc))

	# Subscribing in on_connect() means that if we lose the connection and
	# reconnect then subscriptions will be renewed.
	client.subscribe("envmon/#")

# The callback for when a PUBLISH message is received from the server.
def on_message(client, userdata, msg):
	#print(msg.topic+" "+str(msg.payload))
	topics = split(msg.topic, '/')
	sensorId = topics[1]
	unit = topics[2]
	value = msg.payload
	sensors = session.query(Sensor)
	for sensor in sensors:
		if sensor.id == sensorId:
			if unit == 'temp': sensor.last_temp=valu
			if unit == 'rh': sensor.last_rh=value
			if unit == 'co2': sensor.last_co2=value
			sensor.time_stamp=time.time()
			print sensor.id + " " + unit + " " + value			
			session.commit()
			print "Data sent to database"
			send_data(sensor.id, unit, value)

    

def send_data(sensorId, unit, value):
	# Setter sammen data i riktig format, oppretter en socket og sender data til carbon/graphite
	if unit == 'temp': unitStr = '.t '
	if unit == 'rh': unitStr = '.rh '
	if unit == 'co2': unitStr ='.co2 '	
	msg = 'envmon.' + sensorId + unitStr + value + ' %d\n' % int(time.time())
	print msg	
	sock = socket.socket()
	sock.connect(('web-drift.idi.ntnu.no', 2003))
	sock.sendall(msg)
	sock.close()



client = mqtt.Client()
client.on_connect = on_connect
client.on_message = on_message

client.connect("mqtt.idi.ntnu.no", 1883, 60)

# Blocking call that processes network traffic, dispatches callbacks and
# handles reconnecting.
# Other loop*() functions are available that give a threaded interface and a
# manual interface.
client.loop_forever()
