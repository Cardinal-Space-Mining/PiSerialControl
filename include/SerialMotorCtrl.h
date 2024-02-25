#pragma once
#include <cstdint>

#include "SerialInterface.h"


class MotorSerialConnection
{
public:
    MotorSerialConnection(const char *port);

    inline void set_percent(uint32_t motor_number, double duty_cycle){
        MotorDataStruct mds;
        mds.motor_number = motor_number;
        mds.call_mode = MotorCallMode::PERCENT;
        mds.percent_or_velocity = duty_cycle;
        this->send_command(mds);
    }

    inline void set_neutral_mode(uint32_t motor_number, MotorNeutralMode mode){
        MotorDataStruct mds;
        mds.motor_number = motor_number;
        mds.call_mode = MotorCallMode::NEUTRAL_MODE;
        mds.neutral_mode = mode;
        this->send_command(mds);
    }

    inline void set_velocity(uint32_t motor_number, double turns_per_second){
        MotorDataStruct mds;
        mds.motor_number = motor_number;
        mds.call_mode = MotorCallMode::VELOCITY;
        mds.percent_or_velocity = turns_per_second;
        this->send_command(mds);
    }


    //Probably do not call this
    void send_command(const MotorDataStruct &data);

    ~MotorSerialConnection();

private:
    int serial_fd;
};