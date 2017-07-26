#ifndef MOTOR_CONTROLLER_H
#define MOTOR_CONTROLLER_H

#include "motor_config.h"
#include "logger.h"

class MotorController
{
    private:
        MotorConfig config;
        const ILogger* logger;
    public:
        MotorController(const MotorConfig& config, const ILogger& logger);
        void stop();
        void runForwards();
        void runBackwards();
        void setSpeed(int motorSpeed /*Range: 0-100*/);
};

#endif // MOTOR_CONTROLLER_H