#include <chrono>
#include <thread>


#include "SerialMotorCtrl.h"


int main(){

    MotorSerialConnection mtr_conn("/dev/ttyUSB0");

    MotorDataStruct motor_data = {};

    motor_data.motor_number = 0;
    motor_data.mode = MotorMode::PERCENT;

    while(true){
        for (size_t i = 0; i < 101; i++)
        {
            motor_data.outputValue = i/200.0;
            mtr_conn.send_command(motor_data);
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
        }

        for (size_t i = 0; i < 101; i++)
        {
            motor_data.outputValue = .5 - (i/200.0);
            mtr_conn.send_command(motor_data);
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
        }
        
    }
}