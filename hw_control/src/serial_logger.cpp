#include <Arduino.h>
#include "serial_logger.h"

void SerialLogger::Log(const char* format, ...) const
{
    const unsigned int BufferSize = 256;
    Serial.print("LOG ");
    char dest[BufferSize];
    sprintf(dest, "%09lu: ", millis());
    Serial.print(dest);
    va_list argptr;
    va_start(argptr, format);
    vsnprintf(dest, BufferSize, format, argptr);
    va_end(argptr);
    Serial.println(dest);
}
