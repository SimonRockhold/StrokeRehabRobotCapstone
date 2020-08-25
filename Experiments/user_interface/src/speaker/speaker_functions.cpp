#include "speaker_class.h"
#include "speaker_tunes.h"


// Local Variables
int buzzer_pin = 10;            // Pin assignment for Buzzer, must be a Digital PWM(~) capable pin
int divider = 0, noteDuration = 0;



// Class Functions
void speaker_class::playTune(int melody[], int notes, int tempo)
{
  // this calculates the duration of a whole note in ms
  int wholenote = (60000 * 4) / tempo;
  
  // iterate over the notes of the melody.
  // Remember, the array is twice the number of notes (notes + durations)
  for (int thisNote = 0; thisNote < notes * 2; thisNote = thisNote + 2) {

    // calculates the duration of each note
    divider = melody[thisNote + 1];
    if (divider > 0) {
      // regular note, just proceed
      noteDuration = (wholenote) / divider;
    } else if (divider < 0) {
      // dotted notes are represented with negative durations!!
      noteDuration = (wholenote) / abs(divider);
      noteDuration *= 1.5; // increases the duration in half for dotted notes
    }

    // we only play the note for 90% of the duration, leaving 10% as a pause
    tone(buzzer_pin, melody[thisNote], noteDuration * 0.9);

    // Wait for the specief duration before playing the next note.
    delay(noteDuration);

    // stop the waveform generation before the next note.
    noTone(buzzer_pin);
  }
}


// Shortcuts
void speaker_class::victoryTune()
{
    // sizeof gives the number of bytes, each int value is composed of two bytes (16 bits)
    // there are two values per note (pitch and duration), so for each note there are four bytes
    int notes = sizeof(victory_tune) / sizeof(victory_tune[0]) / 2;
    
    // change this to make the song slower or faster
    int tempo = 132;

    playTune(victory_tune, notes, tempo);
}

void speaker_class::touchTune()
{
    // sizeof gives the number of bytes, each int value is composed of two bytes (16 bits)
    // there are two values per note (pitch and duration), so for each note there are four bytes
    int notes = sizeof(touch_tune) / sizeof(touch_tune[0]) / 2;
    
    // change this to make the song slower or faster
    int tempo = 240;

    playTune(touch_tune, notes, tempo);
}

/* To ADD
void speaker_class::encouragementTune()
{
    // sizeof gives the number of bytes, each int value is composed of two bytes (16 bits)
    // there are two values per note (pitch and duration), so for each note there are four bytes
    int notes = sizeof(encouragement_tune) / sizeof(encouragement_tune[0]) / 2;
    
    // change this to make the song slower or faster
    int tempo = 80;

    playTune(encouragement_tune, notes, tempo);
}*/
