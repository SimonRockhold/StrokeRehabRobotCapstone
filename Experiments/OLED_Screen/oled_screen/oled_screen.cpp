#include "oled_screen.h"
void oled_screen_class::oled_setup ()
{
    

    Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

    
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
  
    // Draw a single pixel in white
    display.drawPixel(10, 10, WHITE);
  
    // Show the display buffer on the screen. You MUST call display() after
    // drawing commands to make them visible on screen!
    display.display();
    delay(100);
    // display.display() is NOT necessary after every single drawing command,
    // unless that's what you want...rather, you can batch up a bunch of
    // drawing operations and then update the screen all at once by calling
    // display.display(). These examples demonstrate both approaches...
  
    // Scrolling Text
    //printText("This is where you will put");
    //printText("sample messages");

};


// Function to draw a specified bitmap onto screen, offset by x_pos, y_pos, with a width of w and height of h
void oled_screen_class::draw_bitmap (uint8_t x_pos, uint8_t y_pos, const uint8_t *bitmap, uint8_t w, uint8_t h)
{
  Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
  display.clearDisplay();

  display.drawBitmap(
    x_pos, y_pos, bitmap, w, h, 1);
  display.display();
  delay(1000);
}
