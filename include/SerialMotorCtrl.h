#pragma once
#include <cstdint>

constexpr int32_t MOTOR_MODE_PERCENT = 0;

enum class MotorMode : uint32_t
{
    PERCENT = 0
};

#pragma pack(1)
struct MotorDataStruct
{
    uint32_t motor_number;
    MotorMode mode;
    double outputValue;
};

class MotorSerialConnection
{
public:
    MotorSerialConnection(const char *port);

    void send_command(const MotorDataStruct &data);

    ~MotorSerialConnection();

private:
    int serial_fd;
};