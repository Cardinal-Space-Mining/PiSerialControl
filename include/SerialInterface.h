#pragma once
#include <cstdint>

constexpr int32_t MOTOR_MODE_PERCENT = 0;
constexpr double MOTOR_COAST = 0;
constexpr double MOTOR_BREAK = 1;

enum class MotorNeutralMode: uint32_t{
    MOTOR_COAST = 0,
    MOTOR_BREAK = 1
};

enum class MotorCallMode : uint32_t
{
    PERCENT = 0,
    VELOCITY = 2,
    DISABLE = 15,
    NEUTRAL_MODE = 16,
};

#pragma pack(1)
struct MotorDataStruct
{
    uint32_t motor_number;
    MotorCallMode call_mode;
    union{
        double percent_or_velocity;
        MotorNeutralMode neutral_mode;
    };
};