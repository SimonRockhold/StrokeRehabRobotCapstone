/*******************************************************************************
* OLED Screen Library
*
* File name:    oled_screen.cpp
*
*
*
*******************************************************************************/

// Local Libraries
#include "oled_screen_class.h"

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
    wait(2000); // Pause for 2 seconds

    // Clear the buffer (must be done every time)
    display.clearDisplay();

    //main_menu();//draw_bitmap(0, 0, main_menu_bmp, SCREEN_WIDTH, SCREEN_HEIGHT);    // Draw a bitmap image
};



//---------------------------------
// Function to draw a specified bitmap onto screen, offset by x_pos, y_pos, with a width of w and height of h
//---------------------------------
void oled_screen_class::draw_bitmap(uint8_t x_pos, uint8_t y_pos, const uint8_t *bitmap, uint8_t w, uint8_t h, uint8_t color=1)
{
    display.clearDisplay();

    // drawbitmap(Width Center Point, Height Center Point, bit map array, bit map width, bit map height, color)
    display.drawBitmap(x_pos, y_pos, bitmap, w, h, color);
    display.display();
    wait(1000);
}


//---------------------------------
// Function to wait a specified delay, in milliseconds
//---------------------------------
void oled_screen_class::wait(long delay)
{
    unsigned long currentMillis = millis();
    previousMillis = currentMillis;
    
    while(currentMillis - previousMillis < delay) {
        currentMillis = millis();
    }
}



//---------------------------------
// Function to write an input string to the display
//---------------------------------
void oled_screen_class::print_text(char str[], int textSize=2)
{
    #define charsPerLine 20 // Number of characters (textsize = 1) that fit on a line
    //int TextSize = 2;
    
    int currentPosition = 0;
    int currentLine = 0;

    display.clearDisplay();

    display.setTextSize(textSize);
    display.setTextColor(WHITE);
    display.setCursor(0,0);


    for ( int i = 0 ; i < strlen(str); i++)
    {
        if(str[i] == "\\" && str[i+1]=="n"){
            currentPosition = 0;
            currentLine += textSize;
            display.setCursor(currentPosition,currentLine);
            i++;
        }
        else if(currentPosition+textSize >= charsPerLine){
            currentPosition = 0;
            currentLine += textSize;
            display.setCursor(currentPosition,currentLine);
        }
        else{
          display.print(str[i]);
          display.display();
          wait(10);
        }
    }
}



//---------------------------------
// Function to display the score
//---------------------------------
void oled_screen_class::display_score() {
    display.clearDisplay();
    display.setTextSize(2);
    display.setTextColor(WHITE);
    display.setCursor(0,0);

    display.print("\nScore = ");
    display.print(score);
    display.display();
}



//---------------------------------
// Function to update the current score and display the new score
//---------------------------------
void oled_screen_class::update_score(int newScore) {
    score = newScore;
    display_score();
}



//---------------------------------
// Function to play animation
//---------------------------------
void oled_screen_class::lightShow() {
  int16_t i;

  display.clearDisplay(); // Clear display buffer

  for(i=0; i<display.width(); i+=4) {
    display.drawLine(0, 0, i, display.height()-1, WHITE);
    display.display(); // Update screen with each newly-drawn line
    wait(1);
  }
  for(i=0; i<display.height(); i+=4) {
    display.drawLine(0, 0, display.width()-1, i, WHITE);
    display.display();
    wait(1);
  }
  wait(250);

  display.clearDisplay();

  for(i=0; i<display.width(); i+=4) {
    display.drawLine(0, display.height()-1, i, 0, WHITE);
    display.display();
    wait(1);
  }
  for(i=display.height()-1; i>=0; i-=4) {
    display.drawLine(0, display.height()-1, display.width()-1, i, WHITE);
    display.display();
    wait(1);
  }
  wait(250);

  display.clearDisplay();

  for(i=display.width()-1; i>=0; i-=4) {
    display.drawLine(display.width()-1, display.height()-1, i, 0, WHITE);
    display.display();
    wait(1);
  }
  for(i=display.height()-1; i>=0; i-=4) {
    display.drawLine(display.width()-1, display.height()-1, 0, i, WHITE);
    display.display();
    wait(1);
  }
  wait(250);

  display.clearDisplay();

  for(i=0; i<display.height(); i+=4) {
    display.drawLine(display.width()-1, 0, 0, i, WHITE);
    display.display();
    wait(1);
  }
  for(i=0; i<display.width(); i+=4) {
    display.drawLine(display.width()-1, 0, i, display.height()-1, WHITE);
    display.display();
    wait(1);
  }
}



//---------------------------------
// Function to display "Victory" animation
//---------------------------------
void oled_screen_class::victory()
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

void oled_screen_class::menu_start()
{
    draw_bitmap(0, 0, menu_start_bmp, SCREEN_WIDTH, SCREEN_HEIGHT, WHITE);
}

void oled_screen_class::start_back()
{
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.setCursor(0,0);

    display.print("Place robot on line");

    display.drawBitmap(0, SCREEN_HEIGHT/2, back_selected_bmp, SCREEN_WIDTH/2, SCREEN_HEIGHT/2, WHITE);
    display.drawBitmap(SCREEN_WIDTH/2, SCREEN_HEIGHT/2, start_bmp, SCREEN_WIDTH/2, SCREEN_HEIGHT/2, WHITE);

    display.display();
}

void oled_screen_class::start_start()
{
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.setCursor(0,0);

    display.print("Place robot on line");

    display.drawBitmap(0, SCREEN_HEIGHT/2, back_bmp, SCREEN_WIDTH/2, SCREEN_HEIGHT/2, WHITE);
    display.drawBitmap(SCREEN_WIDTH/2, SCREEN_HEIGHT/2, start_selected_bmp, SCREEN_WIDTH/2, SCREEN_HEIGHT/2, WHITE);

    display.display();
}

void oled_screen_class::countdown()
{
    print_text("\n  5", 4);
    wait(1000);
    print_text("\n  4", 4);
    wait(1000);
    print_text("\n  3", 4);
    wait(1000);
    print_text("\n  2", 4);
    wait(1000);
    print_text("\n  1", 4);
    wait(1000);
    eyes_open();
}

void oled_screen_class::menu_guide()
{
    draw_bitmap(0, 0, menu_guide_bmp, SCREEN_WIDTH, SCREEN_HEIGHT, WHITE);
}

void oled_screen_class::guide()
{
    draw_bitmap(0, 0, guide_bmp, SCREEN_WIDTH, SCREEN_HEIGHT, WHITE);
}

void oled_screen_class::menu_score()
{
    draw_bitmap(0, 0, menu_score_bmp, SCREEN_WIDTH, SCREEN_HEIGHT, WHITE);
}

void oled_screen_class::score_back()
{
    display.clearDisplay();
    display.setTextSize(2);
    display.setTextColor(WHITE);
    display.setCursor(0,0);

    display.print("Last Score = ");
    display.print(score);

    display.drawBitmap(0, SCREEN_HEIGHT/2, back_selected_bmp, SCREEN_WIDTH/2, SCREEN_HEIGHT/2, WHITE);

    display.display();
}

void oled_screen_class::menu_cal()
{
    draw_bitmap(0, 0, menu_cal_bmp, SCREEN_WIDTH, SCREEN_HEIGHT, WHITE);
}

void oled_screen_class::cal_back()
{
    display.clearDisplay();
    display.setTextSize(2);
    display.setTextColor(WHITE);
    display.setCursor(0,0);

    display.print("Calibrate?");

    display.drawBitmap(0, SCREEN_HEIGHT/2, back_selected_bmp, SCREEN_WIDTH/2, SCREEN_HEIGHT/2, WHITE);
    display.drawBitmap(SCREEN_WIDTH/2, SCREEN_HEIGHT/2, start_bmp, SCREEN_WIDTH/2, SCREEN_HEIGHT/2, WHITE);

    display.display();
}

void oled_screen_class::cal_start()
{
    display.clearDisplay();
    display.setTextSize(2);
    display.setTextColor(WHITE);
    display.setCursor(0,0);

    display.print("Calibrate?");

    display.drawBitmap(0, SCREEN_HEIGHT/2, back_bmp, SCREEN_WIDTH/2, SCREEN_HEIGHT/2, WHITE);
    display.drawBitmap(SCREEN_WIDTH/2, SCREEN_HEIGHT/2, start_selected_bmp, SCREEN_WIDTH/2, SCREEN_HEIGHT/2, WHITE);

    display.display();
}

void oled_screen_class::calibrate()
{
    display.setTextSize(2);
    display.setTextColor(WHITE);

    display.clearDisplay();
    display.setCursor(0,0);
    display.print("Calibrate\n.");
    display.display();
    wait(1000);

    display.clearDisplay();
    display.setCursor(0,0);
    display.print("Calibrate\n..");
    display.display();
    wait(1000);

    display.clearDisplay();
    display.setCursor(0,0);
    display.print("Calibrate\n...");
    display.display();
    wait(1000);

    eyes_resting();
    eyes_open();
    eyes_happy();
}


void oled_screen_class::eyes_happy()
{
    draw_bitmap(0, 0, eyes_happy_bmp, SCREEN_WIDTH, SCREEN_HEIGHT, WHITE);
}

void oled_screen_class::eyes_happy_2()
{
    draw_bitmap(0, 0, eyes_happy_2_bmp, SCREEN_WIDTH, SCREEN_HEIGHT, WHITE);
}

void oled_screen_class::eyes_open()
{
    draw_bitmap(0, 0, eyes_open_bmp, SCREEN_WIDTH, SCREEN_HEIGHT, WHITE);
}

void oled_screen_class::eyes_open_2()
{
    draw_bitmap(0, 0, eyes_open_2_bmp, SCREEN_WIDTH, SCREEN_HEIGHT, WHITE);
}

void oled_screen_class::eyes_resting()
{
    draw_bitmap(0, 0, eyes_resting_bmp, SCREEN_WIDTH, SCREEN_HEIGHT, WHITE);
}

void oled_screen_class::eyes_resting_2()
{
    draw_bitmap(0, 0, eyes_resting_2_bmp, SCREEN_WIDTH, SCREEN_HEIGHT, WHITE);
}

/*/void oled_screen_class::main_menu_guide_selected()
{
    display.clearDisplay();
    display.invertDisplay(true);
    display.drawBitmap(0, 0, guide_button_bmp, SCREEN_WIDTH/2, SCREEN_HEIGHT/2, INVERSE);
    display.invertDisplay(false);
    display.drawBitmap(SCREEN_WIDTH/2, 0, score_button_bmp, SCREEN_WIDTH/2, SCREEN_HEIGHT/2, WHITE);
    display.drawBitmap(0, SCREEN_HEIGHT/2, start_button_bmp, SCREEN_WIDTH/2, SCREEN_HEIGHT/2, WHITE);
    display.drawBitmap(SCREEN_WIDTH/2, SCREEN_HEIGHT/2, more_button_bmp, SCREEN_WIDTH/2, SCREEN_HEIGHT/2, WHITE);
    display.display();
    wait(1000);
}*/
