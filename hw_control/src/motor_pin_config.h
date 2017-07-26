#ifndef MOTOR_PIN_CONFIG_H
#define MOTOR_PIN_CONFIG_H

struct MotorPinConfig
{
    int EnablePin;    // Connected to PWM
    int DirectionPin1;
    int DirectionPin2;
    // DirectionPin1 = HIGH and DirectionPin2 = LOW  -> motor spins formward
    // DirectionPin1 = LOW  and DirectionPin2 = HIGH -> motor spins backwards
    // DirectionPin1 = LOW  and DirectionPin2 = LOW  -> motor is stopped
};

#endif //MOTOR_PIN_CONFIG_H