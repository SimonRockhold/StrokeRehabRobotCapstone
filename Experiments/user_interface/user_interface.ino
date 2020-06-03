/**************************************************************************
  User Interface Example Code
**************************************************************************/
// Local Libraries
#include "user_interface_class.h"

user_interface_class  ui;
oled_screen_class screen;
//----------------------------------------------//
// Setup Function (One Time At Startup)
//----------------------------------------------//
void setup() {
  // User Interface Setup
  ui.ui_setup();
  screen.eyes_open();
}



//----------------------------------------------//
// Local Functions //
//----------------------------------------------//

void loop() {
  ui.ui_menu();
  /*String user_selection = ui.ui_menu();
  if (user_selection == "START")
  {
    screen.victory();
  }*/
}
