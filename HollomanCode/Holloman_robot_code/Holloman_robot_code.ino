//Using this to test to LCD displays
#include <SPI.h> //I had to search for this in file explorer and add it's path to my workspace 
#include <Wire.h>
#include <Adafruit_GFX.h> //https://github.com/adafruit/Adafruit-GFX-Library make sure and rename file to include properly
#include <Adafruit_SSD1306.h> //https://github.com/adafruit/Adafruit_SSD1306 make sure and rename file to include properly
#include "Holloman_common.h"

#define BITMAP_WIDTH 50
#define BITMAP_HEIGHT 50

Adafruit_SSD1306 display = Adafruit_SSD1306(128, 64, &Wire); //Configured to our board size, this always gives error for some reason

bool eyesOnOnce = false; //for some reason i can't get this to work when it's in common


//So far screens work, just need different bitmap
const static unsigned char PROGMEM myBitmap_bmp[] = {B00000000, B11000000,
  B00000001, B11000000,
  B00000001, B11000000,
  B00000011, B11100000,
  B11110011, B11100000,
  B11111110, B11111000,
  B01111110, B11111111,
  B00110011, B10011111,
  B00011111, B11111100,
  B00001101, B01110000,
  B00011011, B10100000,
  B00111111, B11100000,
  B00111111, B11110000,
  B01111100, B11110000,
  B01110000, B01110000,
  B00000000, B00110000
};

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
        makeNoise();
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
    display.drawBitmap(((display.width()  - BITMAP_WIDTH ) / 2), ((display.height() - BITMAP_HEIGHT) / 2), myBitmap_bmp, BITMAP_WIDTH, BITMAP_HEIGHT, 1); 

    display.display();

    //delay(2000);

    
}

const int beepPin = 2;
const int button = 3;
int tones[] = {440, 554, 659, 880};
int length = sizeof(tones) / sizeof(tones[0]);

void makeNoise()
{
    chord(500);
}

void chord(int duration)
{
    for (int i = 0; i < length; i++)
    {
        tone(beepPin, tones[i]);
        delay(duration);
        noTone(beepPin);
    }
}
