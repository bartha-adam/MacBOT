#ifndef MOVEMENT_CONTROLLER_H
#define MOVEMENT_CONTROLLER_H

#include "motor_config.h"
#include "motor_controller.h"
#include "logger.h"

class MovementController
{
    private:
        MotorController* leftMotor;
        MotorController* rightMotor;
        const ILogger& logger;
        int speed;

    public:
        MovementController(const MotorConfig& leftMotorConfig, const MotorConfig& rightMotorConfig, const ILogger& logger);
        ~MovementController();
       
    public:
        void moveForward();
        void moveBackward();
        void turnLeft();
        void turnRight();
        void stop();
        void setSpeed(int speed);
        int getSpeed();
};

#endif //MOVEMENT_CONTROLLER_H
