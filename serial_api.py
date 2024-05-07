import serial
import struct
import serial

class SerialManager:
    __slots__ = ("serial_hdl")

    __RIO_BAUDRATE__: int = 115200

    MOTOR_CTRL: int = 0

    __MOTOR_IDX_TRACK_RIGHT__ = 0

    __MOTOR_IDX_TRACK_LEFT__ = 1

    START_MINING_FUNC_NUM = 1

    STOP_MINING_FUNC_NUM = 2

    START_OFFLOAD_FUNC_NUM = 3

    STOP_OFFLOAD_FUNC_NUM = 4

    MSG_OK = 0

    def __init__(self, port: str, baudrate = __RIO_BAUDRATE__) -> None:
        self.serial_hdl =  serial.Serial(port=port, baudrate=baudrate)

    def _send_msg_func(self, func_num: int) -> int:
        parts = [struct.pack('<i', func_num)]

        msg = b''
        for part in parts:
            msg = msg + part

        x = b''

        self.serial_hdl.write(b'\x13')
        while x != b'\x13':
            x = self.serial_hdl.read(1)

        self.serial_hdl.write(msg)

        while x != b'\x11':
            x = self.serial_hdl.read()

        return SerialManager.MSG_OK

    # root function to generate messages
    def _send_msg(self, func_num, motor_num, outputValue) -> int:
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
        self.serial_hdl.write(b'\x13')

        # xon message, reads till a xon
        while x != b'\x13':
            x = self.serial_hdl.read(1)

        self.serial_hdl.write(msg)

        # listening for xoff
        while x != b'\x11':
            x = self.serial_hdl.read(1)

        return SerialManager.MSG_OK

    def __del__(self):
        self.serial_hdl.close()