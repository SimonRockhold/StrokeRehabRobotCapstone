
#ifndef user_interface_h
  #define user_interface_h

  #include "src/oled_screen/oled_screen_class.h"
  #include "src/ui_button/ui_button_class.h"
  #include "src/speaker/speaker_class.h"



  class user_interface_class {


    public:
        void ui_setup();
        String ui_menu();
  };

#endif
