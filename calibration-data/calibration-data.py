"""outputs sensor readings into graph"""

import serial
import matplotlib.pyplot as plt

# set the name of the serial port.  Determine the name as follows:
arduinoComPort = "/dev/cu.usbmodem14201"

# set the baud rate so that it is the same as the sending program
baudRate = 9600

# open the serial port
serialPort = serial.Serial(arduinoComPort, baudRate, timeout=1)

# initialize
plt.axis([0, 10, 0, 1])
x = []
y = []

# main loop to read data from the Arduino
for i in range(100):
    # ask for a line of data from the serial port, the ".decode()" converts the data from an "array of bytes", to a string
    lineOfData = serialPort.readline().decode()

    # check if data was received
    if len(lineOfData) > 0:
        # data was received, convert it into respective variables
        sensorValue = int(lineOfData)
        x.append(i)
        y.append(sensorValue)
        plt.plot(x, y, '-bo')
        plt.pause(0.05)

plt.show()
