#pragma once
#include <cstdint>

// Enum to set neutral mode
enum class MotorNeutralMode : uint32_t
{
    MOTOR_COAST = 0,
    MOTOR_BREAK = 1
};

// Enum to set call mode
enum class MotorCallMode : uint32_t
{
    PERCENT = 0,
    VELOCITY = 2,
    DISABLE = 15,
    NEUTRAL_MODE = 16,
};

// Motor message struct
#pragma pack(1)
struct MotorDataStruct
{
    uint32_t motor_number;
    MotorCallMode call_mode;
    union
    {
        double percent;
        MotorNeutralMode neutral_mode;
        double velocity_turns_per_second;
    };
};

// Defines Serial Message Types
enum class SerialMsgType : uint32_t
{
    MotorMessage = 0
};

// Base message to be sent over serial
#pragma pack(1)
struct SerialMsg
{
    SerialMsgType type;
    union
    {
        MotorDataStruct mds;
    };
};

enum class SerialResponse : uint8_t
{
    SUCCESS = 0,
    GENERAL_FAILURE = 1,
    INVALID_ARGUMENT = 2
};