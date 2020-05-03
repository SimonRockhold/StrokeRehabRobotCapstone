#include <SD.h>
#include <SPI.h>

class SDCard
{
private:
    File file;
    String filename;
public:
    SDCard(int cardSelect, String filenameIN);
    void writeToSD(String in);
};