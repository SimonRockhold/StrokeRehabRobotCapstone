//Using this to test to LCD displays
#include <SPI.h> //I had to search for this in file explorer and add it's path to my workspace 
#include <Wire.h>
#include <Adafruit_GFX.h> //https://github.com/adafruit/Adafruit-GFX-Library make sure and rename file to include properly
#include <Adafruit_SSD1306.h> //https://github.com/adafruit/Adafruit_SSD1306 make sure and rename file to include properly
#include "Holloman_common.h"

Adafruit_SSD1306 display = Adafruit_SSD1306(128, 64, &Wire); //Configured to our board size, this always gives error for some reason
