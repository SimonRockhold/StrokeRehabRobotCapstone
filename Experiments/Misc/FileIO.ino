#include <SD.h>
#include <SPI.h>

const int cardSelect = 4;

void setup()
{
    pinMode(13, INPUT);
    File testFile;
    // Open serial communications and wait for port to open:
    Serial.begin(9600);
    while (!Serial)
    {
        ; // wait for serial port to connect. Needed for native USB port only
    }

    Serial.print("Initializing SD card...");

    if (!SD.begin(cardSelect))
    {
        Serial.println("initialization failed!");
        while (1)
            ;
    }
    Serial.println("initialization done.");

    if (SD.exists("test.txt"))
    {
        Serial.println("example.txt exists. opening file");
        testFile = SD.open("test.txt", FILE_READ);
    }
    else
    {
        Serial.println("example.txt doesn't exist.");
    }

    digitalWrite(13, testFile);

    if (testFile)
    {
        while (testFile.available())
        {
            Serial.write(testFile.read());
        }
        testFile.close(); //testFile.println("test test, one two three");
    }
    else
    {
        Serial.println("write failed");
        while (true)
            ;
    }
}

void loop()
{
}