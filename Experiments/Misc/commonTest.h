
//const int DRIVE_TIME = 10; //Defines how long (in milliseconds) motors drive in forward functions. Keep this very low to reduce wagging
const byte DEFAULT_SPEED = 150;

// these constants are used to allow you to make your motor configuration
// line up with function names like forward.  Value can be 1 or -1
const int offsetA = 1;
const int offsetB = -1; //                                                     Change one of these, can't remember which

// Pins for all inputs, keep in mind the PWM defines must be on PWM pins,
// CHANGE THESE UPON WIRING

//Digital
#define AIN1 7
#define BIN1 5
#define AIN2 8
#define BIN2 4
#define PWMA 9
#define PWMB 3
//Optional
#define STBY 6

//// Sensor initializations
//const int NUM_SENSORS = 5;
//const int SENSOR_MAX = 1024;
//const int SENSOR_MAX = 0;

// int maxIR;
// int minIR;

// int sensorDataRaw[NUM_SENSORS];

// const int IRSensor[] = {A2, A1, A0, A7, A6}; //Pins reflect current wiring (2/18/2020)

class Timer
{
private:
    void updateTime()
    {
        prevTime = millis();
    }
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
    bool timeElapsed()
    {
        deltaTime = millis() - prevTime;
        if (deltaTime >= interval)
        {
            timeElapsedFlag = true;
            updateTime();
        }
        return timeElapsedFlag;
    }
};