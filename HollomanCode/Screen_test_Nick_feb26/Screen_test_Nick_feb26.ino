//Using this to test to LCD displays
#include <SPI.h> //I had to search for this in file explorer and add it's path to my workspace 
#include <Wire.h>
#include <Adafruit_GFX.h> //https://github.com/adafruit/Adafruit-GFX-Library make sure and rename file to include properly, info here https://learn.adafruit.com/adafruit-gfx-graphics-library/overview 
#include <Adafruit_SSD1306.h> //https://github.com/adafruit/Adafruit_SSD1306 make sure and rename file to include properly
#include "Holloman_common_test.h"

Adafruit_SSD1306 display = Adafruit_SSD1306(128, 64, &Wire); //Configured to our board size, this always gives error for some reason

void setup() {

    Serial.begin(9600);
    
    Serial.println("initialized");


    display.begin(SSD1306_SWITCHCAPVCC, 0x3C); // begins display, displays arduino logo by default
    display.clearDisplay(); //This will clear the adafruit logo

    display.setTextSize(2); //Define text size
    display.setTextColor(SSD1306_WHITE); //Not sure yet how to adjust color settings, it seems that there's just a predefined line where yellow and blue split
    display.setCursor(0,0); //defines where to start your message
    display.println("Hello world!"); //Prints hello world to screen
    display.display(); // add this after your display prompts to actually display things

    delay(2000);

    display.clearDisplay(); //clears display

    display.fillRect(x0, y0, width, height, BLUE); //This is how you fill a rectangle

    display.display();

    delay(2000);

    display.clearDisplay();
    display.display();

}



void loop() {

}
