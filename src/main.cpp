#include <chrono>
#include <thread>


#include "SerialMotorCtrl.h"


int main(){

    MotorSerialConnection mtr_conn("/dev/ttyUSB0");

    while(true){
        for (size_t i = 0; i < 101; i++)
        {
            mtr_conn.set_percent(1, i/200);
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
        }

        for (size_t i = 0; i < 101; i++)
        {
            mtr_conn.set_percent(1, .5 - (i/200.0));
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
        }
        
    }
}