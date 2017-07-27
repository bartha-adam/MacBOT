#include <Arduino.h>
#include "serial_logger.h"

void SerialLogger::Log(const char* format, ...) const
{
    Serial.print("LOG ");
    char dest[255];
    sprintf(dest, "%09lu: ", millis());
    Serial.print(dest);
    va_list argptr;
    va_start(argptr, format);
    vsprintf(dest, format, argptr);
    va_end(argptr);
    Serial.println(dest);
}
