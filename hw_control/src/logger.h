#ifndef LOGGER_H
#define LOGGER_H

class ILogger
{
    public:
        virtual void Log(const char* format, ...) const = 0;
};

#endif //LOGGER_H