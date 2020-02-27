//Using this to test to LCD displays
#include <SPI.h> //I had to search for this in file explorer and add it's path to my workspace 
#include <Wire.h>
#include <Adafruit_GFX.h> //https://github.com/adafruit/Adafruit-GFX-Library make sure and rename file to include properly
#include <Adafruit_SSD1306.h> //https://github.com/adafruit/Adafruit_SSD1306 make sure and rename file to include properly

Adafruit_SSD1306 display = Adafruit_SSD1306(128, 64, &Wire); //Configured to our board size, this always gives error for some reason

void setup() {

    Serial.begin(9600);
    
    Serial.println("initialized");


    display.begin(SSD1306_SWITCHCAPVCC, 0x3C); // begins display

    display.display(); //This displays the Adafruit logo, for some reason it is essential to running this code
    //delay(1000); //Leave this commented if you want to skip the adafruit screen
    display.clearDisplay();

    display.setTextSize(1); //Define text size
    display.setTextColor(SSD1306_WHITE); //Not sure yet how to adjust color settings, it seems that there's just a predefined line where yellow and blue split
    display.setCursor(0,0); //defines where to start your message
    display.println("Hello world!"); //Prints hello world to screen

    //display.print("connected!"); //This was all in the example code
    //display.println("IP: 10.0.1.23");
    //display.println("Sending val #0");
    //display.setCursor(0,0);

    display.display(); // add this after your display prompts to actually display things

    delay(5000);

    display.clearDisplay(); //clears display
    display.display(); //Use this to actually clear display
}

void loop() {

}
