#include <Arduino.h>
#include "SDCard.h"

SDCard::SDCard(int cardSelect, String filenameIn)
// for some reason, filenames (including extenstions) MUST be less than 13 characters. I don't know why, and it isn't clearly documented anywhere.
{
    init = false;
    filenameIn.toCharArray(filename, 15);

    if (!SD.begin(cardSelect))
    //check if there is a card at the specified pin select.
    {
        Serial.println("Card failed, or not present");
    }
    else
    {
        if (!SD.exists(filename))
        {
            Serial.print(filename);
            Serial.println(": file not found, failed to initialize.");
        }
        else
        {
            Serial.print(filename);
            Serial.println(": file initialized");
            init = true;
        }
    };
}

void SDCard::write(byte *buf, int len)
{
    File f = SD.open(filename, FILE_WRITE);
    if (f)
    {
        Serial.print(f.write(buf, len));
        Serial.println(" wrote stuff");
    }
    else
    {
        Serial.println("failed to write, dang");
    }
    f.close();
}

byte SDCard::read(byte *buf, int len)
{
    File f = SD.open(filename);
    if (f)
    {
        f.readBytes(buf, len);
    }
    else
    {
        Serial.println("failed to read file, dang");
        Serial.println(filename);
    }
    f.close();
}

void SDCard::printToSD(String in)
{
    File file = SD.open(filename, FILE_WRITE);
    if (file)
    {
        file.println(in);
        Serial.print("wrote to SD: " + in);
        Serial.println();
    }
    else
    {
        digitalWrite(13, LOW);
        Serial.print("failed to write to ");
        Serial.println(filename);
    }
    file.close();
}

void SDCard::readLine(byte *buffer, int bufferLength)
{
    Serial.print("opening ");
    Serial.print(filename);
    Serial.println("...");
    File file = SD.open(filename, FILE_READ);
    if (file)
    {
        int i = 0;
        for (int i = 0; i < bufferLength; i++)
        {
            buffer[i] = file.read();
        }
    }
    else
    {

        Serial.println("file not found while reading");
    }

    file.close();
}

bool SDCard::initialized()
//returns true if the card was initialized successfully. returns a copy to prevent accidental reassignment.
{
    if (init)
    {
        return true;
    }
    else
    {
        return false;
    }
}
