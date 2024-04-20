from enum import Enum
import struct
import serial
import time

s = serial.Serial(
    port='/dev/ttyS0',
    baudrate=9600
)

# root function to generate messages
def send_msg(func_num, motor_num, outputValue):
    # parts is the final message, struct.pack turns message into bytes, pack enum (i) and then append motor number in following
    parts = [struct.pack('<i', func_num)]        # function number
    parts.append(struct.pack('<i', motor_num))   # motor number
    parts.append(struct.pack('<d', outputValue)) # percent value rn

    msg = b'' # msg is empty byte literal then read msg byes into it
    for part in parts:
        msg = msg + part
    # random buffer variable
    x = b''

    s.write(b'\x13')

    # xon message, reads till a xon
    while x != b'\x13':
        x = s.read(1)

    s.write(msg)

    print()
    # listening for xoff
    while x != b'\x11':
        x = s.read(1)
	    
# template message
def set(outputValue: float):
	send_msg(0, 0, float(outputValue))

# testing spins motors up and down quickly
while True:
    for i in range(101):
        set(float(i/200))
        time.sleep(0.01)
    for i in range(101):
        set(.5 - (i/200))
        time.sleep(0.01)
