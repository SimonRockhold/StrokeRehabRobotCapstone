// System Libraries
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// Local Libraries
#include "bitmaps.h"

// Definitions
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
#define SCROLL_PERIOD 100

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)

class oled_screen_class
{
    // Variables and Functions not accessible outside of class
    private:
        
    // Variables and Functions accessible outside of class
    public:
        void oled_setup();
        void draw_bitmap(uint8_t x_pos, uint8_t y_pos, const uint8_t *bitmap, uint8_t w, uint8_t h);
};
