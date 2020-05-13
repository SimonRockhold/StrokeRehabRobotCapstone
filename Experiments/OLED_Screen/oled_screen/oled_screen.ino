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
  delay(2000);  // Add two second delay

  
  
  // Request Eyes Resting
  oled.eyes_resting();
  delay(2000);

  // Request Eyes Open
  oled.eyes_open();
  delay(2000);

  // Request Menu
  oled.main_menu();
  delay(2000);

  // Request Guide
  oled.guide();
  delay(2000);

  // Request Eyes Happy
  oled.eyes_happy();
  delay(2000);
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
