#include "Arduino.h"
#include "user_interface_class.h"


// Class Instantiations
ui_button_class ui_button;
oled_screen_class oled;



// State Definitions
enum ui_state_enum {INACTIVE,
                    MENU_START,
                        START_BACK,
                        START_START,
                    MENU_GUIDE,
                        GUIDE,
                    MENU_SCORE,
                        SCORE_BACK,
                    MENU_CAL,
                        CAL_BACK,
                        CAL_START};

// Variables
uint8_t ui_state = INACTIVE;


String user_interface_class::ui_menu()
{
    // Start with Main Menu and Start Selected
    ui_state = MENU_START;
    oled.menu_start();

    // Loop during User Input
    while(ui_state != INACTIVE)
    {
        switch(ui_state)
        {
            // Catch Weird State
            case INACTIVE:
                break;



            case MENU_START:
                if(ui_button.checkSelect() == 1){
                    ui_state = START_BACK;
                    oled.start_back();
                }

                else if(ui_button.checkNext() == 1) {
                    ui_state = MENU_GUIDE;
                    oled.menu_guide();
                }
                break;


            case START_BACK:
                if(ui_button.checkSelect() == 1){
                    ui_state = MENU_START;
                    oled.menu_start();
                }

                else if(ui_button.checkNext() == 1) {
                    ui_state = START_START;
                    oled.start_start();
                }
                break;


            case START_START:
                if(ui_button.checkSelect() == 1){
                    ui_state = INACTIVE;
                    oled.countdown();
                    oled.eyes_open();
                    return("START");
                }

                else if(ui_button.checkNext() == 1) {
                    ui_state = START_BACK;
                    oled.start_back();
                }
                break;


            case MENU_GUIDE:
                if(ui_button.checkSelect() == 1){
                    ui_state = GUIDE;
                    oled.guide();
                }

                else if(ui_button.checkNext() == 1) {
                    ui_state = MENU_SCORE;
                    oled.menu_score();
                }
                break;

            case GUIDE:
                if(ui_button.checkSelect() == 1){
                    ui_state = MENU_GUIDE;
                    oled.menu_guide();
                }

                else if(ui_button.checkNext() == 1) {
                    ui_state = MENU_GUIDE;
                    oled.menu_guide();
                }
                break;


            case MENU_SCORE:
                if(ui_button.checkSelect() == 1){
                    ui_state = SCORE_BACK;
                    oled.score_back();
                }

                else if(ui_button.checkNext() == 1) {
                    ui_state = MENU_CAL;
                    oled.menu_cal();
                }
                break;

            case SCORE_BACK:
                if(ui_button.checkSelect() == 1){
                    ui_state = MENU_SCORE;
                    oled.menu_score();
                }

                else if(ui_button.checkNext() == 1) {
                    ui_state = MENU_SCORE;
                    oled.menu_score();
                }
                break;

            case MENU_CAL:
                if(ui_button.checkSelect() == 1){
                    ui_state = CAL_BACK;
                    oled.cal_back();
                }

                else if(ui_button.checkNext() == 1) {
                    ui_state = MENU_START;
                    oled.menu_start();
                }
                break;

            case CAL_BACK:
                if(ui_button.checkSelect() == 1){
                    ui_state = MENU_CAL;
                    oled.menu_cal();
                }

                else if(ui_button.checkNext() == 1) {
                    ui_state = CAL_START;
                    oled.cal_start();
                }
                break;

            case CAL_START:
                if(ui_button.checkSelect() == 1){
                    ui_state = INACTIVE;
                    oled.calibrate();
                    oled.eyes_resting_2();
                    oled.eyes_happy_2();
                    oled.eyes_open_2();
                    return("CALIBRATION");
                }

                else if(ui_button.checkNext() == 1) {
                    ui_state = CAL_BACK;
                    oled.cal_back();
                }
                break;


        }
    }
}


void user_interface_class::ui_setup()
{
    // UI Button Setup
    ui_button.ui_button_setup();

    // OLED Setup
    oled.oled_setup();
}
