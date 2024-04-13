from enum import Enum
import struct
import serial
import time

s = serial.Serial(
    port='/dev/ttyACM0',
    baudrate=230400
)

def read_msg():
    msg = b''

    while x!= b'x\13':
        x = s.read(1)

    print("got XON")

while True:
    print("Listening for message")
    read_msg()