#include <chrono>
#include <thread>
#include <iostream>


#include "../include/SerialMotorCtrl.h"


int main(){

    MotorSerialConnection mtr_conn("/dev/ttyS0");
    std::cout << "starting \n";
    // while(true){
    //     for (size_t i = 0; i < 101; i++)
    //     {
    //         mtr_conn.set_percent(0, i/200);
    //         std::this_thread::sleep_for(std::chrono::milliseconds(10));
    //     }

    //     for (size_t i = 0; i < 101; i++)
    //     {
    //         mtr_conn.set_percent(0, .5 - (i/200.0));
    //         std::this_thread::sleep_for(std::chrono::milliseconds(10));
    //     }
    //     std::cout << "Loop" << std::endl;
    // }

    

    std::cout << "Done";

}