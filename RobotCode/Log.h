#include <SPI.h>
#include <SD.h>
//namespace logNameSpace{
class Log
{
private:
    int flushCounter;
    File logFile;
    int initialize(int SD_cardSelect, String fileName);
    int SD_cardSelect;
public:
    Log(int SD_cardSelect, String fileName);
    File getLogFile();
};
//}