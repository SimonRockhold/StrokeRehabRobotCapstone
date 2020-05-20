#include <SD.h>
#include <SPI.h>

class bitmapLoader
{
private:
byte *buf;
int len;
public:
    bitmapLoader(int cardSelect, byte *imageLocation, int length);
    void loadImg(String filenameIn);
    byte read(byte *buf, int len);
    
  
};