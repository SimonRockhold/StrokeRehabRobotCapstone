#include "commonTest.h"
Timer testTimer = Timer(1000);

void setup()
{
    Timer testTimer = Timer(10000);
    Serial.begin(9600);
}

void loop()
{
    if (testTimer.timeElapsed())
    {
        Serial.println("done");
    }
    else
    {
        Serial.print(testTimer.deltaTime);
    }
}