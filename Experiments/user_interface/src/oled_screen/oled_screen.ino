/**************************************************************************
OLED Screen Code
**************************************************************************/
// Local Libraries
#include "oled_screen_class.h"

// Variables
int ledPin = 13;                // TEMPORARY
int buttonPin = 2;              // TO DO: Move Pin Assignment To Global Pin Assignment List

int ledToggle;                  // TEMPORARY
int previousState = 1;       // Used to determine button transitions
unsigned int previousPress;     // Used to time button press durations
volatile int buttonFlag;
int buttonDebounce  = 20;       // Used to debounce button input (20ms)
int selectTimer     = 500;      // Used to identify Select vs Next (500ms)

int select  = 0;                // User Pressed Select
int next    = 0;                // User Pressed Next

oled_screen_class oled;
//----------------------------------------------//
// Setup Function (One Time At Startup)
//----------------------------------------------//
void setup() {
  // UI Button Setup
  pinMode(ledPin, OUTPUT);
  pinMode(buttonPin, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(2), button_ISR, CHANGE);
  
  // Class Declaration
  
  
  // Call class function
  oled.oled_setup();
  delay(500);  // Add two second delay

  // Request Eyes Resting
  //oled.eyes_resting();
  //delay(2000);

  // Request Eyes Open
  //oled.eyes_open();
  //delay(2000);

  // Request Menu
  //oled.main_menu();
  //delay(2000);

  // Main Menu, Start Highlighted
  oled.mm_start();
  //delay(2000);

  // Menu Guide Selected
  //oled.main_menu_guide_selected();
  //delay(4000);

  // Request Guide
  //oled.guide();
  //delay(8000);

  // Display Score
  //oled.display_score();
  //delay(2000);

  // Update and Display Score
  //oled.update_score(1);
  //oled.display_score();
  //delay(2000);

  // Print Text Test
  //oled.print_text("\nYou Can \nDo It!");
  //delay(2000);

  // Update and Display Score again
  //oled.update_score(20);
  //oled.display_score();
  //delay(2000);

  // Declare Victory
  //oled.victory();
  ///delay(1000);

  // Request Eyes Happy
  //oled.eyes_happy();
  //delay(2000);
}



//----------------------------------------------//
// Local Functions //
//----------------------------------------------//

void buttonEvent()
{
  // Button Debouncer
  if((millis() - previousPress) > buttonDebounce)
  {
    // Button Pushed
    if(digitalRead(buttonPin) == LOW && previousState == HIGH)
    {
      previousPress = millis();
      previousState = LOW;
    }

    // Button Released
    else if(digitalRead(buttonPin) == HIGH && previousState == LOW)
    {
      
      // Select
      if((millis() - previousPress) > selectTimer)
      {
        //oled.main_menu();
        select = 1;
      }

      // Next
      else
      {
        // Do nothing
        //oled.eyes_resting();
      }
      previousState = HIGH;
    }
  }
}


void button_ISR()
{
  buttonEvent();
}


void loop() {
  // User Interface
  if (select)
  {
    ledToggle =! ledToggle;
    digitalWrite(ledPin, ledToggle);
    select = 0;
  }
}
