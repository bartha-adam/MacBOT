#ifndef SERIAL_LOGGER_H
#define SERIAL_LOGGER_H

#include "logger.h"

class SerialLogger : public ILogger
{
    public:
        void Log(const char* format, ...) const;
};

#endif //SERIAL_LOGGER_H
