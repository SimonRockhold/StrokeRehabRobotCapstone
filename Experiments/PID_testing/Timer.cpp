#include <Arduino.h>
#include "Timer.h"
//using namespace n;
void Timer::updateTime()
{
    prevTime = millis();
}
bool Timer::timeElapsed()
{
    deltaTime = millis() - prevTime;
    if (deltaTime >= interval)
    {
        timeElapsedFlag = true;
        updateTime();
    }
    return timeElapsedFlag;
}