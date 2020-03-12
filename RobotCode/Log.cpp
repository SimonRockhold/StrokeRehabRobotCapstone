#include <Arduino.h>
#include "Log.h"

Log::Log(int SD_CS, String fileName)
{
    initialize(SD_CS, fileName);
}

int Log::initialize(int SD_cardSelect, String fileName)
{
    if (!SD.begin(SD_cardSelect))
    {
        Serial.println("SD card initialization failed.");
        return 0;
    }
    if (SD.exists(fileName))
    {
        Serial.println("log file already present.");
        return 0;
    }
    else
    {
        logFile = SD.open(fileName, FILE_WRITE);
    }
}
File Log::getLogFile()
{
    return logFile;
}