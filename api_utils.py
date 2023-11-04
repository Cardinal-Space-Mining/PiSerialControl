from enum import Enum
import struct
import serial
import time

s = serial.Serial(
    port='/dev/ttyUSB0',
    baudrate=230400
)

class ControlMode(Enum):
    PercentOutput = 0
    Position = 1
    Velocity = 2
    Current = 3
    Follower = 5
    MotionProfile = 6
    MotionMagic = 7
    MotionProfileArc = 10
    MusicTone = 13
    Disabled = 15

# root function to generate messages
def send_msg(func_num, motor_num, *argv):
    parts = [struct.pack('i', func_num)]
    parts.append(struct.pack('i', motor_num))
    for arg in argv:
        if isinstance(arg, float):
            parts.append(struct.pack('d', arg))
        if isinstance(arg, int):
            parts.append(struct.pack('i', arg))
        if isinstance(arg, ControlMode):
            parts.append(struct.pack('i', arg.value))
    msg = b''
    for part in parts:
        msg = msg + part
    x = b''
    while x != b'\x13':
        x = s.read(1)
    s.write(msg)
    while x != b'\x11':
        x = s.read(1)

def set(mode: ControlMode, outputValue: float):
	send_msg(0, 0, ControlMode(mode), float(outputValue))

while True:
    for i in range(101):
        set(ControlMode.PercentOutput, float(i/200))
        time.sleep(0.01)
    for i in range(101):
        set(ControlMode.PercentOutput, .5 - (i/200))
        time.sleep(0.01)