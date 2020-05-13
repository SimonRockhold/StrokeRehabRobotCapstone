#include "SDCard.h"
#include "Calibration.h"
#include <SD.h>
#include <SPI.h>

int CS = 10;
bool buttonPressed = true; // dummy variable placeholder

<<<<<<< HEAD
namespace //limits the scope of decalations inside namespace to this file.
{
Motor motor1 = Motor(AIN1, AIN2, PWMA, offsetA, STBY); //I need to check the motor code I wrote for testing to change these. I know offset needs to be changed
Motor motor2 = Motor(BIN1, BIN2, PWMB, offsetB, STBY);

Timer calibrationTimer = Timer(CALIBRATION_TIME);
Timer outputTimer = Timer(SECOND);   // one second interval between outputs
Timer logTimer = Timer(SECOND * 30); // set a 30 second timer to log all values during that time.

Log sensorLog = Log(SD_CS, "sensorLog.csv");

File logFile = sensorLog.getLogFile();

int logIndex = 0;
int sensorLog[dataPoints][NUM_SENSORS];
int maxIR = 775;
int minIR = 230;
} //end namespace init
=======
Calibration values = {4, 5};
Calibration v2;
>>>>>>> ddce9c7... SDCard class development, all methods functional

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

<<<<<<< HEAD
void blink()
//Changed blink to only blink once, but for the online function it calls blink() as long as the device isn't on a line
{
  digitalWrite(13, HIGH);
  delay(500);
  digitalWrite(13, LOW);
=======
    //sensorRecord.writeToSD("test, test, 1,2,3,test");
    Serial.println("beep");
    Serial.println(v2.max_ir);
    Serial.println(v2.min_ir);
>>>>>>> ddce9c7... SDCard class development, all methods functional
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