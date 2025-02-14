import serial
import numpy as np
import struct
import time

s = serial.Serial('/dev/ttyUSB0', 115200)

def read_serial():
    # print(s.read())
    print(s.read_until(b'\n'))

def twist_to_serial(val1, val2):
    l_x = np.float32(val1) 
    a_z = np.float32(val2)
    # print(l_x, a_z)
    s.write(struct.pack('ff', l_x, a_z))

cooldown = 10

while True:
    read_serial()

    if cooldown > 0:
        cooldown -= 10
    else:
        twist_to_serial(0.0, -1.0)
        cooldown = 10
