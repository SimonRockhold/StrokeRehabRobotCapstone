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
void OledScreen::oled_setup()
{
  //No longer needs to be called, added to constructor to run on creation of new OledScreen instance.
  // Open I2C Serial Connection at 9600 Baud
  //Serial.begin(9600); perfomed in RobotCode setup

  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C))
  { // Address 0x3C for 128x64
    Serial.println(F("SSD1306 allocation failed"));
    for (;;)
      ; // Don't proceed, loop forever
  }

  // Show initial display buffer contents on the screen --
  // the library initializes this with an Adafruit splash screen.
  display.display();
  wait(2000); // Pause for 2 seconds

  // Clear the buffer (must be done every time)
  display.clearDisplay();

  main_menu(); //draw_bitmap(0, 0, main_menu_bmp, SCREEN_WIDTH, SCREEN_HEIGHT);    // Draw a bitmap image
};
//
//Constructor for OledScreen Object, only runs oled_setup on creation of new object instance,
//
OledScreen::OledScreen()
{
  oled_setup();
}

//---------------------------------
// Function to draw a specified bitmap onto screen, offset by x_pos, y_pos, with a width of w and height of h
//---------------------------------
void OledScreen::draw_bitmap(uint8_t x_pos, uint8_t y_pos, const uint8_t *bitmap, uint8_t w, uint8_t h)
{
  display.clearDisplay();

  // drawbitmap(Width Center Point, Height Center Point, bit map array, bit map width, bit map height, color)
  display.drawBitmap(x_pos, y_pos, bitmap, w, h, 1);
  display.display();
  wait(1000);
}

//---------------------------------
// Function to wait a specified delay, in milliseconds
//---------------------------------
void OledScreen::wait(long delay)
{
  unsigned long currentMillis = millis();
  previousMillis = currentMillis;

  while (currentMillis - previousMillis < delay)
  {
    currentMillis = millis();
  }
}

//---------------------------------
// Function to write an input string to the display
//---------------------------------
void OledScreen::print_text(char str[], int textSize = 2)
{
#define charsPerLine 20 // Number of characters (textsize = 1) that fit on a line
  //int TextSize = 2;

  int currentPosition = 0;
  int currentLine = 0;

  display.clearDisplay();

  display.setTextSize(textSize);
  display.setTextColor(WHITE);
  display.setCursor(0, 0);

  for (int i = 0; i < strlen(str); i++)
  {
    if (str[i] == "\\" && str[i + 1] == "n")
    {
      currentPosition = 0;
      currentLine += textSize;
      display.setCursor(currentPosition, currentLine);
      i++;
    }
    else if (currentPosition + textSize >= charsPerLine)
    {
      currentPosition = 0;
      currentLine += textSize;
      display.setCursor(currentPosition, currentLine);
    }
    else
    {
      display.print(str[i]);
      display.display();
      wait(10);
    }
  }
}

void OledScreen::display_score()
{
  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(0, 0);

  display.print("\nScore = ");
  display.print(score);
  display.display();
}

void OledScreen::update_score(int newScore)
{
  score = newScore;
}

void OledScreen::lightShow()
{
  int16_t i;

  display.clearDisplay(); // Clear display buffer

  for (i = 0; i < display.width(); i += 4)
  {
    display.drawLine(0, 0, i, display.height() - 1, WHITE);
    display.display(); // Update screen with each newly-drawn line
    wait(1);
  }
  for (i = 0; i < display.height(); i += 4)
  {
    display.drawLine(0, 0, display.width() - 1, i, WHITE);
    display.display();
    wait(1);
  }
  wait(250);

  display.clearDisplay();

  for (i = 0; i < display.width(); i += 4)
  {
    display.drawLine(0, display.height() - 1, i, 0, WHITE);
    display.display();
    wait(1);
  }
  for (i = display.height() - 1; i >= 0; i -= 4)
  {
    display.drawLine(0, display.height() - 1, display.width() - 1, i, WHITE);
    display.display();
    wait(1);
  }
  wait(250);

  display.clearDisplay();

  for (i = display.width() - 1; i >= 0; i -= 4)
  {
    display.drawLine(display.width() - 1, display.height() - 1, i, 0, WHITE);
    display.display();
    wait(1);
  }
  for (i = display.height() - 1; i >= 0; i -= 4)
  {
    display.drawLine(display.width() - 1, display.height() - 1, 0, i, WHITE);
    display.display();
    wait(1);
  }
  wait(250);

  display.clearDisplay();

  for (i = 0; i < display.height(); i += 4)
  {
    display.drawLine(display.width() - 1, 0, 0, i, WHITE);
    display.display();
    wait(1);
  }
  for (i = 0; i < display.width(); i += 4)
  {
    display.drawLine(display.width() - 1, 0, i, display.height() - 1, WHITE);
    display.display();
    wait(1);
  }
}

//---------------------------------
// Function to display "Victory" animation
//---------------------------------
void OledScreen::victory()
{
  // Display a scrolling "VICTORY!"
  display.clearDisplay();

  //display.setTextSize(2); // Draw 2X-scale text
  //display.setTextColor(WHITE);
  //display.setCursor(40, 00);
  print_text(" VICTORY!");
  //display.display();
  display.startscrolldiagright(0x00, 0x07);
  wait(8000);
  display.stopscroll();
  wait(2000);

  // Light Show
  lightShow();
}

//---------------------------------
// Function shortcuts to display various screens
//---------------------------------
void OledScreen::main_menu()
{
  draw_bitmap(0, 0, main_menu_bmp, SCREEN_WIDTH, SCREEN_HEIGHT);
}

void OledScreen::guide()
{
  draw_bitmap(0, 0, guide_bmp, SCREEN_WIDTH, SCREEN_HEIGHT);
}

void OledScreen::eyes_happy()
{
  draw_bitmap(0, 0, eyes_happy_bmp, SCREEN_WIDTH, SCREEN_HEIGHT);
}

void OledScreen::eyes_open()
{
  draw_bitmap(0, 0, eyes_open_bmp, SCREEN_WIDTH, SCREEN_HEIGHT);
}

void OledScreen::eyes_resting()
{
  draw_bitmap(0, 0, eyes_resting_bmp, SCREEN_WIDTH, SCREEN_HEIGHT);
}
