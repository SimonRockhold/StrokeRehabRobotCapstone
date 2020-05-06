/**************************************************************************
OLED Screen Code
**************************************************************************/
// Local Libraries
#include "oled_screen.h"


//----------------------------------------------//
// Setup Function (One Time At Startup)
//----------------------------------------------//
void setup() {
  // Class Declaration
  oled_screen_class oled;
  
  // Call class function
  oled.oled_setup();

  // drawbitmap(Width Center Point, Height Center Point, bit map array, bit map width, bit map height, color)
  oled.draw_bitmap(0, 0, main_menu_bmp, SCREEN_WIDTH, SCREEN_HEIGHT);    // Draw a bitmap image
  
  delay(2000); // Wait two seconds
  oled.draw_bitmap(0, 0, eyes_happy_bmp, SCREEN_WIDTH, SCREEN_HEIGHT);    // Draw a bitmap image

}



//----------------------------------------------//
// Local Functions //
//----------------------------------------------//



void loop() {
  // Unused loop, required by Arduino code
}














/*

// Maybe Need To Remove?
char *substring(char *string, int position, int length)
{
   char *pointer;
   int c;
 
   pointer = malloc(length+1);
   
   if (pointer == NULL)
   {
      printf("Unable to allocate memory.\n");
      exit(1);
   }
 
   for (c = 0 ; c < length ; c++)
   {
      *(pointer+c) = *(string+position-1);      
      string++;  
   }
 
   *(pointer+c) = '\0';
 
   return pointer;
}

int string_length(char* given_string) 
{ 
    // variable to store the 
    // length of the string 
    int length = 0; 
    while (*given_string != '\0') { 
        length++; 
        given_string++; 
    } 
  
    return length; 
} 

void printText(char *s) {
  int textLength = string_length(s);
  char *text;
  char *subText;
  
  if (strlen(s) > 10)
  {   
    
    text = s;
    while (*text != '\0') {
      display.clearDisplay();
      subText = substring(text, 1, 9);
      display.setTextSize(2); // Draw 2X-scale text
      display.setTextColor(WHITE);
      display.setCursor(10, 0);
      display.println(subText);
      display.display();
      delay(SCROLL_PERIOD);
      text++;
    }
  }

  else {
      display.clearDisplay();
    
      display.setTextSize(2); // Draw 2X-scale text
      display.setTextColor(WHITE);
      display.setCursor(10, 0);
    
      display.println(s);
      display.display();
      delay(1000);
  }
  

  
  //display.println("0123456789ABCDEF");
  //display.display();      // Show initial text
  //delay(100);
}

*/
