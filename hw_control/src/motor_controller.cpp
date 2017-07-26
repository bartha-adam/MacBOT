#include <Arduino.h>
#include "motor_controller.h"


MotorController::MotorController(const MotorConfig& config, const ILogger& logger)
{
    this->config = config;
    this->logger = &logger;
    
    pinMode(config.PinConfig.EnablePin, OUTPUT);
    pinMode(config.PinConfig.DirectionPin1, OUTPUT);
    pinMode(config.PinConfig.DirectionPin2, OUTPUT);
}

void MotorController::stop()
{
    digitalWrite(config.PinConfig.DirectionPin1, LOW);
    digitalWrite(config.PinConfig.DirectionPin2, LOW);
    logger->Log("Stopped %s motor", config.MotorName.c_str());
}

void MotorController::runForwards()
{
    digitalWrite(config.PinConfig.DirectionPin1, HIGH);
    digitalWrite(config.PinConfig.DirectionPin2, LOW);
    logger->Log("Started %s motor forwards", config.MotorName.c_str());
}

void MotorController::runBackwards()
{
    digitalWrite(config.PinConfig.DirectionPin1, LOW);
    digitalWrite(config.PinConfig.DirectionPin2, HIGH);
    logger->Log("Started motor %s motor backwards", config.MotorName.c_str());
}

void MotorController::setSpeed(int motorSpeed)
{
    static int minSpeed = 70;
    static int maxSpeed = 255;
    
    if(motorSpeed < 0)
        motorSpeed = 0;
    else if(motorSpeed > 100)
        motorSpeed = 100;

    /*PWM value range is 0 to 255*/
    int pwmValue = (float)motorSpeed/100 * (maxSpeed - minSpeed) + minSpeed;
    analogWrite(config.PinConfig.EnablePin, pwmValue);
    logger->Log("Set %s motor speed to %d | %d", config.MotorName.c_str(), motorSpeed, pwmValue);
}
