#include <Arduino.h>
#include "bitmapLoader.h"

bitmapLoader::bitmapLoader(int cardSelect, byte *imageLocation, int length)
// for some reason, filenames (including extenstions) MUST be less than 13 characters for compliance with FAT16 formatting
{
    buf = imageLocation;
    len = length;
    if (!SD.begin(cardSelect))
    //check if there is a card at the specified pin select.
    {
        Serial.println("Card failed, or not present");
    }
};

void bitmapLoader::loadImg(String filenameIn)
{
    char filename[13];
    filenameIn.toCharArray(filename, 15);

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
