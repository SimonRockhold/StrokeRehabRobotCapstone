#include <Arduino.h>
#include "SDCard.h"

SDCard::SDCard(int cardSelect, String filenameIN)
{
    filename = filenameIN;

    if (!SD.begin(cardSelect))
    //check if there is a card at the specified pin select.
    {
        Serial.println("Card failed, or not present");
        // don't do anything more:
        while (1)
            ;
    }
    else
    {
        Serial.println("card initialized");
    };
}

void SDCard::writeToSD(String in)
{

    char charBuffer[13];
    filename.toCharArray(charBuffer, 15);

    Serial.println(charBuffer);
    Serial.println();
    file = SD.open(charBuffer, FILE_WRITE);
    if (file)
    {
        file.println(in);
        file.close();
        Serial.print("wrote to SD: " + in);
    }
    else
    {
        digitalWrite(13, LOW);
        Serial.println("ARRGG");
    }
}

