import time
import serial

try:
	ser = serial.Serial(
		port='/dev/ttyUSB0',
		baudrate = 115200,
		parity=serial.PARITY_NONE,
		stopbits=serial.STOPBITS_ONE,
		bytesize=serial.EIGHTBITS,
		timeout=1
	)

except:
	print("Failed to connect serial")
	raise SystemExit

try:
	while 1:
    	read_serial = ser.readline()
    	if (len(read_serial) > 1):
    		print read_serial

except (KeyboardInterrupt, SystemExit):
	print("Interrupt received")
	ser.close()

except (RuntimeError):
	print("RuntimeError")
	ser.close()
