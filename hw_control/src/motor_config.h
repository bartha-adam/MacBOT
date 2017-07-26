#ifndef MOTOR_CONFIG_H
#define MOTOR_CONFIG_H

#include <WString.h>
#include "motor_pin_config.h"

struct MotorConfig
{
    String MotorName;
    MotorPinConfig PinConfig;
};

#endif //MOTOR_CONFIG_H
