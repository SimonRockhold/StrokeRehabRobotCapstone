
#ifndef speaker_h
  #define speaker_h
  
  #include "Arduino.h"
  
  class speaker_class {
    private:
        void playTune(int melody[], int notes, int tempo);

    public:
        void victoryTune();
        void touchTune();
        //void encouragementTune(); // TO ADD
  };

#endif
