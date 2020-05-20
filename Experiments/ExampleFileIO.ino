#include "SDCard.h"
#include "Calibration.h"
#include <SD.h>
#include <SPI.h>

int CS = 10;
bool buttonPressed = true; // dummy variable placeholder

Calibration values = {4, 5};
Calibration v2;

void setup()
//all actions that are only done once
{
    Serial.begin(9600);
    while (!Serial)
    {
        ; // wait for serial port to connect. Needed for native USB port only
    }
    Serial.println("Serial connected"); //quick check to make sure device is communicating

    testCode();

    SDCard calibration = SDCard(CS, "calibr.dat");

    calibration.write((byte *)&values, sizeof(values));
    //SDCard sensorRecord = SDCard(CS, "SENSORS.csv");

    if (buttonPressed && calibration.initialized())
    {
        Serial.println("reading previous calibration values from file");

        calibration.read((byte *)&v2, sizeof(values));
    }

    //sensorRecord.writeToSD("test, test, 1,2,3,test");
    Serial.println("beep");
    Serial.println(v2.max_ir);
    Serial.println(v2.min_ir);
}

void loop()
{
}

void testCode()
{
    Calibration c = {'a', 'b'};
    Calibration c2;

    File f = SD.open("borkle.dat", FILE_WRITE);
    if (f)
    {
        f.write((char *)&c, sizeof(c));
        Serial.println("wrote");
    }
    else
    {
        Serial.println("ARGRRGF");
    }
    f.close();

    f = SD.open("test.dat");
    if (f)
    {
        f.read((char *)&c2, sizeof(c));
    }

    Serial.println(c2.max_ir);
    Serial.println(c2.min_ir);
}