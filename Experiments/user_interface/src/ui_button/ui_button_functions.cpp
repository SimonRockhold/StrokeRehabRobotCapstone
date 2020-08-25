#include "Arduino.h"
#include "ui_button_class.h"

// Local Variables
char buttonPin = PD1;              // TO DO: Move Pin Assignment To Global Pin Assignment List
int ledPin = 13;
int select = 0;
int next   = 0;
int previousState = 1;       // Used to determine button transitions
unsigned int previousPress;     // Used to time button press durations
int buttonDebounce  = 20;       // Used to debounce button input (20ms)
int selectTimer     = 500;      // Used to identify Select vs Next (500ms)


// Class Functions
int ui_button_class::checkSelect()
{
    int temp = select;
    select = 0;
    return temp;
    
}

int ui_button_class::checkNext()
{
    int temp = next;
    next = 0;
    select = 0;
    return temp;
}

void ui_button_class::ui_button_setup()
{
  PCMSK2 |= bit (PCINT17); 	// Pin TXO
  PCIFR	 |= bit (PCIF2);	// Clear any outstanding interrupts
  PCICR  |= bit (PCIE2);	// Enable pin change interrupts for PCINT23 to PCINT16

  pinMode(buttonPin, INPUT_PULLUP);
  pinMode(ledPin, OUTPUT);
}


// Global Functions
void buttonEvent()
{
  ui_button_class ui_btn;
  // Button Debouncer
  if ((millis() - previousPress) > buttonDebounce)
  {
    // Button Pushed
    if (digitalRead(buttonPin) == LOW && previousState == HIGH)
    {
      previousPress = millis();
      previousState = LOW;
    }

    // Button Released
    else if (digitalRead(buttonPin) == HIGH && previousState == LOW)
    {

      // Select
      if ((millis() - previousPress) > selectTimer)
      {
        //oled.main_menu();
        select = 1;
      }

      // Next
      else
      {
        next = 1;
      }
      previousState = HIGH;
    }
  }
}

ISR(PCINT2_vect)
{
  buttonEvent();
}
