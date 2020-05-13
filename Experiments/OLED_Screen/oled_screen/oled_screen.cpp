/*******************************************************************************
* OLED Screen Library
*
* File name:    oled_screen.cpp
*
*
*
*******************************************************************************/

// Local Libraries
#include "oled_screen.h"

// Call Adafruit display definition
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

//---------------------------------
// OLED Screen Setup Function (One Time Call At Startup)
//---------------------------------
void oled_screen_class::oled_setup()
{
    // Open I2C Serial Connection at 9600 Baud
    Serial.begin(9600);

    // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
    if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3C for 128x64
      Serial.println(F("SSD1306 allocation failed"));
      for(;;); // Don't proceed, loop forever
    }

    // Show initial display buffer contents on the screen --
    // the library initializes this with an Adafruit splash screen.
    display.display();
    delay(2000); // Pause for 2 seconds

    // Clear the buffer (must be done every time)
    display.clearDisplay();

    main_menu();//draw_bitmap(0, 0, main_menu_bmp, SCREEN_WIDTH, SCREEN_HEIGHT);    // Draw a bitmap image
};



//---------------------------------
// Function to draw a specified bitmap onto screen, offset by x_pos, y_pos, with a width of w and height of h
//---------------------------------
void oled_screen_class::draw_bitmap(uint8_t x_pos, uint8_t y_pos, const uint8_t *bitmap, uint8_t w, uint8_t h)
{
    display.clearDisplay();

    // drawbitmap(Width Center Point, Height Center Point, bit map array, bit map width, bit map height, color)
    display.drawBitmap(x_pos, y_pos, bitmap, w, h, 1);
    display.display();
    delay(1000);
}


//---------------------------------
// Function to wait a specified delay, in milliseconds
//---------------------------------
//void oled_screen_class::wait(long delay)
//{
//    unsigned long currentMillis = millis();
//    previousMillis = currentMillis;
//    
//    while(currentMillis - previousMillis < delay) {
//        currentMillis = millis();
//    }
//}


//---------------------------------
// Function shortcuts to display various screens
//---------------------------------
void oled_screen_class::main_menu()
{
    draw_bitmap(0, 0, main_menu_bmp, SCREEN_WIDTH, SCREEN_HEIGHT);
}

void oled_screen_class::guide()
{
    draw_bitmap(0, 0, guide_bmp, SCREEN_WIDTH, SCREEN_HEIGHT);
}

void oled_screen_class::eyes_happy()
{
    draw_bitmap(0, 0, eyes_happy_bmp, SCREEN_WIDTH, SCREEN_HEIGHT);
}

void oled_screen_class::eyes_open()
{
    draw_bitmap(0, 0, eyes_open_bmp, SCREEN_WIDTH, SCREEN_HEIGHT);
}

void oled_screen_class::eyes_resting()
{
    draw_bitmap(0, 0, eyes_resting_bmp, SCREEN_WIDTH, SCREEN_HEIGHT);
}
