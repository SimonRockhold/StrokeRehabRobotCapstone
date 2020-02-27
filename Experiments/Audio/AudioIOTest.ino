//#include "commonTest.h"
#include <arduino.h>

const int beepPin = 5;
int tones[] = {600, 750, 900};

void setup()
{
    pinMode(beepPin, OUTPUT);

    //TimerMain();
}

void loop()
{
    chord(50, 4, 100);
    delay(400);
}

void chirp(int freq, int beeps, int duration)
{
    for (int i = 0; i < beeps; i++)
    {
        noTone(beepPin);
        tone(beepPin, freq);
        delay(duration);
        noTone(beepPin);
        delay(100);
    }

}

void chord(int freq, int beeps, int duration)
{
    for (int i = 0; i < beeps; i++)
    {
        tone(beepPin, (freq/4)*(4+i*2));
        delay(duration);
        noTone(beepPin);
        delay(duration);
    }
}
