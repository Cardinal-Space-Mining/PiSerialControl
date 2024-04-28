from enum import Enum
import struct
import serial
import time

s = serial.Serial(
    port='/dev/ttyS0',
    baudrate=115200
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

    print("writing")
    s.write(b'\x13')

    # xon message, reads till a xon
    while x != b'\x13':
        x = s.read(1)

    s.write(msg)

    # listening for xoff
    while x != b'\x11':
        x = s.read(1)

def send_msg_func(func_num):
    parts = [struct.pack('<i', func_num)]

    msg = b''
    for part in parts:
        msg = msg + part

    x = b''
    print("wrting");
    s.write(b'\x13')
    while x != b'\x13':
        x = s.read(1)
    print("got xon")
    s.write(msg)
    print("wrote msg")
    while x != b'\x11':
        x = s.read()
    print("got xoff")

# template message
def set(outputValue: float, motornum1, motornum2):
    send_msg(0, motornum1, float(outputValue))
    send_msg(0, motornum2, float(outputValue)) 

def setActuator(outputValue: float, motor_num):
    send_msg(0, motor_num, float(outputValue))

# test spins treads
def move_treds():
    while True:
        for i in range(101):
            set(.15, 0, 1)
            time.sleep(0.01)
        for i in range(101):
            set(.15, 0, 1)
            time.sleep(0.01)

def start_stop_offload():
    while True:
        print("offloading")
        send_msg_func(3)
        time.sleep(4)
        print("stop offload")
        send_msg_func(4)
        time.sleep(4)

def actuator():
    while True:
        setActuator(.2, 5)
        time.sleep(10)

def cycles():
    while True:
        send_msg_func(1)
        time.sleep(25)
        send_msg_func(2)
        time.sleep(4)
        send_msg_func(3)
        time.sleep(4)
        send_msg_func(4)
        time.sleep(4)

# start_stop_offload()
# send_msg_func(4)
#send_msg_func(3)
# send_msg_func(1)
cycles()
