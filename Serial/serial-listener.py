from serial import Serial

ser = serial.Serial('/dev/ttyAMA0', 9600)

while 1:
    read_serial = ser.readline()
    print read_serial
