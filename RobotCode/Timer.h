class Timer
{
private:
    void updateTime();
    long interval;
    bool timeElapsedFlag;

public:
    unsigned long deltaTime;
    unsigned long prevTime;

    Timer(long intervalIn)
    {
        interval = intervalIn;
        prevTime = millis();
        deltaTime = 0;
        timeElapsedFlag = false;
    }
    bool timeElapsed();
};
