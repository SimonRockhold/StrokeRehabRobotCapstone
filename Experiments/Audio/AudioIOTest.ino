#include <arduino.h>

const int beepPin = 2;
const int button = 3;
int tones[] = {440, 554, 659, 880};
int length = sizeof(tones)/sizeof(tones[0]);

void setup()
{
    pinMode(beepPin, OUTPUT);
    pinMode(button, INPUT);
    pinMode(LED_BUILTIN, OUTPUT);

    //TimerMain();
}

void loop()
{
    if (digitalRead(button))
    {
        digitalWrite(LED_BUILTIN, HIGH);
        chord(200);
        delay(400);
    }
    else
    {
        digitalWrite(LED_BUILTIN, LOW);
    }
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

void chord(int duration)
{
    for (int i = 0; i < length; i++)
    {
        tone(beepPin, tones[i]);
        delay(duration);
        noTone(beepPin);
    }
}
