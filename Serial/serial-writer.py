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
    text = input("prompt ").upper()
    if (text.equals('B') || text.equals('C')):
      ser.write(text)
    else:
      print("Invalid input, try again");

except (KeyboardInterrupt, SystemExit):
  print("Interrupt received")
  ser.close()

except (RuntimeError):
  print("RuntimeError")
  ser.close()



  

