#include <SD.h>
#include <SPI.h>

class SDCard
{
private:
    char filename[13];
    bool init;
public:
    SDCard(int cardSelect, String filenameIN);
    void printToSD(String in);
    void write(byte *buf, int len);
    byte read(byte *buf, int len);
    bool initialized();
    void readLine(byte *buffer, int bufflerLength);
};