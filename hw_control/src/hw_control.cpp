#include <Arduino.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

#include "queue.h"
#include "serial_logger.h"
#include "motor_config.h"
#include "motor_pin_config.h"
#include "movement_controller.h"

const char* CMD_MOVE_FORWARD = "mv_fw";
const char* CMD_MOVE_BACKWARD = "mv_bk";
const char* CMD_TURN_LEFT = "turn_left";
const char* CMD_TURN_RIGHT = "turn_right";
const char* CMD_SPEED = "speed";
const char* CMD_ABORT = "abort";
#define COMMAND_QUEUE_SIZE 20

const char* INSTANT_COMMANDS[] = {CMD_SPEED, CMD_ABORT};
const uint32_t INSTANT_COMMANDS_COUNT = sizeof(INSTANT_COMMANDS) / sizeof(char*);

struct CommandExecutionInfo
{
  bool isExecutingCommand;
  bool abortRequested;
  String command;
  uint32_t commandEndAt;
};

CommandExecutionInfo execInfo {false, false, "", 0};
Queue<String> q(COMMAND_QUEUE_SIZE);
const MotorConfig LeftMotorConfig =  {"Left",   { 10/*EnablePin*/, 2/*DirectionPin1*/, 4/*DirectionPin2*/} };
const MotorConfig RightMotorConfig = {"Right", { 9 /*EnablePin*/, 8/*DirectionPin1*/, 7/*DirectionPin2*/} };
SerialLogger logger;
MovementController movement(LeftMotorConfig, RightMotorConfig, logger);

// Command processing methods
bool processMoveForwardCommand(String cmd, uint32_t& duration);
bool processMoveBackwardCommand(String cmd, uint32_t& duration_ms);
bool processTurnLeftCommand(String cmd, uint32_t& duration_ms);
bool processTurnRightCommand(String cmd, uint32_t& duration_ms);
bool processSetSpeedCommand(String cmd);

bool tryExecCmd(const String& cmd, uint32_t& duration_ms, bool &executedInstantly);

// The setup function runs once when you press reset or power the board
void setup()
{
  Serial.begin(115200);
  Serial.println("Init HW Control");
  movement.setSpeed(100);
  movement.stop();
}

// the loop function runs over and over again forever
void loop()
{
  if (execInfo.isExecutingCommand)
  {
    if (execInfo.abortRequested)
    {
      auto elementsInQueue = q.count();
      q.Clear();
      movement.stop();
      logger.Log("Abort requested => removed %d elements from queue", elementsInQueue);
      execInfo.abortRequested = false;
      execInfo.isExecutingCommand = false;
    }
    else if (execInfo.commandEndAt != 0 && execInfo.commandEndAt < millis())
    {
      execInfo.isExecutingCommand = false;
      if (q.IsEmpty())
        movement.stop();
    }
  }
  else
  {
    if (!q.IsEmpty())
    {
      uint32_t cmdDuration;
      bool executedInstantly;
      String cmd;
      if (q.Pop(cmd))
      {
        if (tryExecCmd(cmd, cmdDuration, executedInstantly))
        {
          if (executedInstantly)
          {
            logger.Log("Executed instantly %s", cmd.c_str());
          }
          else
          {
            execInfo.isExecutingCommand = true;
            execInfo.command = cmd;
            logger.Log("Executing now '%s' duration=%d", cmd.c_str(), cmdDuration);
            if (cmdDuration != 0) // 0 means infinite
              execInfo.commandEndAt = millis() + cmdDuration;
            else
              execInfo.commandEndAt = 0;
          }
        }
        else
        {
          logger.Log("Unexpected command: %s", cmd.c_str());
        }
      }
      else
      {
        logger.Log("Failed to pop from commands queue");
      }
    }
  }
}

bool tryExecCmd(const String& cmd, uint32_t& duration_ms, bool &executedInstantly)
{
  executedInstantly = false;
  bool cmdProcessed = false;
  if (cmd.startsWith(CMD_MOVE_FORWARD))
  {
    String params = cmd.substring(strlen(CMD_MOVE_FORWARD) + 1);
    cmdProcessed = processMoveForwardCommand(params, duration_ms);
  }
  else if (cmd.startsWith(CMD_MOVE_BACKWARD))
  {
    String params = cmd.substring(strlen(CMD_MOVE_BACKWARD) + 1);
    cmdProcessed = processMoveBackwardCommand(params, duration_ms);
  }
  else if (cmd.startsWith(CMD_TURN_LEFT))
  {
    String params = cmd.substring(strlen(CMD_TURN_LEFT) + 1);
    cmdProcessed = processTurnLeftCommand(params, duration_ms);
  }
  else if (cmd.startsWith(CMD_TURN_RIGHT))
  {
    String params = cmd.substring(strlen(CMD_TURN_RIGHT) + 1);
    cmdProcessed = processTurnRightCommand(params, duration_ms);
  }
  return cmdProcessed;
}

bool processMoveForwardCommand(String params, uint32_t& duration_ms)
{
  if (1 == sscanf(params.c_str(), "%lu", &duration_ms))
  {
    logger.Log("Moving forwards %lu ms", duration_ms);
  }
  else
  {
    duration_ms = 0;
    logger.Log("Moving forwards");
  }
  movement.moveForward();
  return true;
}

bool processMoveBackwardCommand(String params, uint32_t& duration_ms)
{
  if (1 == sscanf(params.c_str(), "%lu", &duration_ms))
  {
    logger.Log("Moving backwards %lu ms", duration_ms);
  }
  else
  {
    duration_ms = 0;
    logger.Log("Moving backwards");
  }
  movement.moveBackward();
  return true;
}

bool processTurnLeftCommand(String params, uint32_t& duration_ms)
{
  if (1 == sscanf(params.c_str(), "%lu", &duration_ms))
  {
    logger.Log("Turning left %lu ms", duration_ms);
  }
  else
  {
    duration_ms = 0;
    logger.Log("Turning left");
  }
  movement.turnLeft();
  return true;
}

bool processTurnRightCommand(String params, uint32_t& duration_ms)
{
  if (1 == sscanf(params.c_str(), "%lu", &duration_ms))
  {
    logger.Log("Turning left %lu ms", duration_ms);
  }
  else
  {
    duration_ms = 0;
    logger.Log("Turning left");
  }
  movement.turnRight();
  return true;
}

bool processSetSpeedCommand(String cmd)
{
  int speed;
  if (1 == sscanf(cmd.c_str(), "%d", &speed))
  {
    movement.setSpeed(speed);
    logger.Log("Set speed to %d", speed);
  }
  else
  {
    logger.Log("SetSpeedCommand: Missing 'speed' param");
    return false;
  }
  return true;
}

bool isInstantCommand(String command)
{
  for(unsigned int i = 0; i < INSTANT_COMMANDS_COUNT; ++i)
    if(command.startsWith(INSTANT_COMMANDS[i]))
      return true;
  return false;
}

bool processInstantCommand(String cmd)
{
  return true;
}

void serialEvent()
{
  static String inputBuffer;
  while (Serial.available())
  {
    char inChar = (char)Serial.read();
    if (inChar == '\n' || inChar == '\r') // Complete command received
    {
      if(isInstantCommand(inputBuffer))
      {
        bool cmdProcessed = false;
        if (inputBuffer == CMD_ABORT)
        {
          execInfo.abortRequested = true;
          cmdProcessed = true;
        }
        else if (inputBuffer.startsWith(CMD_SPEED))
        {
          String params = inputBuffer.substring(strlen(CMD_SPEED) + 1);
          cmdProcessed = processSetSpeedCommand(params);
        }
        if(!cmdProcessed)
        {
          logger.Log("Failed to process instant command: %s", inputBuffer.c_str());
        }
      }
      else if (q.Push(inputBuffer))
      {
        logger.Log("Command '%s' Pushed to cmd queue | QSize=%d", inputBuffer.c_str(), q.count());
      }
      else
      {
        logger.Log("Failed to Push command '%s' to cmd queue | QSize=%d", inputBuffer.c_str(), q.count());
      }
      inputBuffer = "";
    }
    else
    {
      inputBuffer += inChar;
    }
  }
}
