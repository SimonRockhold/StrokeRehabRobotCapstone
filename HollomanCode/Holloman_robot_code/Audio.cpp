const int beepPin = 2;
const int button = 3;
int tones[] = {440, 554, 659, 880};
int length = sizeof(tones) / sizeof(tones[0]);

void makeNoise()
{
    chord(500);
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