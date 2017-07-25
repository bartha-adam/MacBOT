#include "movement_controller.h"
#include "arduino.h"

MovementController::MovementController(const MotorConfig& leftMotorConfig, const MotorConfig& rightMotorConfig, const ILogger& logger):
  logger(logger), speed(0)
{
    leftMotor = new MotorController(leftMotorConfig, logger);
    rightMotor = new MotorController(rightMotorConfig, logger);
}

MovementController::~MovementController()
{
    if(leftMotor)
        delete leftMotor;
    if(rightMotor)
        delete rightMotor;
}

void MovementController::moveForward()
{
    leftMotor->runForwards();
    rightMotor->runForwards();
}

void MovementController::moveBackward()
{
    leftMotor->runBackwards();
    rightMotor->runBackwards();
}

void MovementController::turnLeft()
{
    leftMotor->runBackwards();
    rightMotor->runForwards();
}

void MovementController::turnRight()
{
    leftMotor->runForwards();
    rightMotor->runBackwards();
}

void MovementController::stop()
{
    leftMotor->stop();
    rightMotor->stop();
}

void MovementController::setSpeed(int speed)
{
    if(speed < 0)
    {
        speed = 0;
        logger.Log("Warn: Speed truncated to 0");
    }    
    else if(speed > 100)
    {
        speed = 100;
        logger.Log("Warn: Speed truncated to 100");
    }
    
    this->speed = speed;
    leftMotor->setSpeed(speed);
    rightMotor->setSpeed(speed);
}

int MovementController::getSpeed()
{
    return speed;
}



