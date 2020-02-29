//Using this to test to LCD displays
#include <SPI.h> //I had to search for this in file explorer and add it's path to my workspace 
#include <Wire.h>
#include <Adafruit_GFX.h> //https://github.com/adafruit/Adafruit-GFX-Library make sure and rename file to include properly
#include <Adafruit_SSD1306.h> //https://github.com/adafruit/Adafruit_SSD1306 make sure and rename file to include properly
#include "Holloman_common.h"

Adafruit_SSD1306 display = Adafruit_SSD1306(128, 64, &Wire); //Configured to our board size, this always gives error for some reason

bool eyesOnOnce = false; //for some reason i can't get this to work when it's in common

void setup() {

    display.begin(SSD1306_SWITCHCAPVCC, 0x3C); // begins display, displays arduino logo by default
    display.clearDisplay(); //This will clear the adafruit logo

    digitalWrite(buttonPin, INPUT); //Button pin
    digitalWrite(buzzerPin, OUTPUT); //For the buzzer
    digitalWrite(ledPin, OUTPUT); //For the ledPin

    Serial.begin(9600);

}

void loop() {

    if (eyesOn == false && eyesOnOnce == false) {
        displayEyes();
        Serial.println("eyeson");
        eyesOnOnce = true;

    }
    
    if(digitalRead(buttonPin)) {
        Serial.println("button pressed");
        closeEyes();
        //makeNoise();
        //blink();

        eyesOn = false;
        eyesOnOnce = false;
    }
}

void displayEyes() {

    //display.begin(SSD1306_SWITCHCAPVCC, 0x3C); // begins display, displays arduino logo by default
    display.clearDisplay(); //This will clear the adafruit logo

    display.fillRect(x0, y0, width, height, YELLOW);

    display.display();

    eyesOn = true;

}

void closeEyes() {

    display.clearDisplay(); 

    //display.fillCircle(x1, y1, r, WHITE);
    void drawBitmap(int16_t x1, int16_t y1, uint8_t *bitmap, int16_t w, int16_t h, uint16_t color); 

    //display.display();

    delay(2000);

    
}



