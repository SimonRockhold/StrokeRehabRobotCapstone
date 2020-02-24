// function declaration, included to ensure that we don't get some dumb compile time errors
void onLine();
void goStraight();
float convertIR();
//void turning(int IRdirection);
void storeData();
void readSensor();
float percentDiff();
int getMax();
int getMin();
void blink();
void updateTime();
void setMaxSpeed();

// We weren't using the minute variable
unsigned long deltaTime; //initialize a change in time variable.
unsigned long prevTime;
unsigned long runTime = 60000 * 1; //decides how long to run our code, 60,000 is one minute, the second number multiplies that miute

/////////// pin definitions
const int IRSensor[] = {A2, A1, A0, A7, A6}; //Pins reflect current wiring (2/18/2020)

// to be set when the sensor max and min are known, and put here to improve code flexibility if we change sensors at any point.
const int NUM_SENSORS = 5;

const int SENSOR_MAX = 1024;
const int SENSOR_MIN = 0;

//Initialize max and min IR here, they will be the 'modified' values
float maxIR;
float minIR;

//Initialize IRdirection. This will be a value from 0-1 that determines our direction, with 0 is left, 1 is right, and 0.5 is forward
float IRdirection;

//Initialize array to store sensor values
int sensorDataRaw[5];

//const int CALIBRATION_TIME = 30 * 1000; // sets the calibration time to 30 seconds., not using rn

const float TURN_THRESHOLD = 0.1; // defines the threshold the deviation must reach before turning.
float tempPercentDiff;            //placeholder for the percent difference

//These are the multipliers were using to assign weight to inner and outer sensors
const int INNER_WEIGHT = 2;
const int OUTER_WEIGHT = 3;
const float CENTER_WEIGHT = 1; //Dont know if we'll need the center weight, I think I found a workaround

// stores the scale factors used in weighted turning.
const float scaleArray[] = {-OUTER_WEIGHT, -INNER_WEIGHT, CENTER_WEIGHT, INNER_WEIGHT, OUTER_WEIGHT};

/*

Not using atm

//Initialize variables for data collection
int logIndex = 0;
const int dataPoints = 50;
int sensorLog[dataPoints][NUM_SENSORS]; //rather than store data sequentially,
//it might be better to use a 2D array to better organize what data points are from the same collection.
*/

//                   initialize motor variables

const int DRIVE_TIME = 10; //Defines how long (in milliseconds) motors drive in forward functions. Keep this very low to reduce wagging
const byte MAX_SPEED = 150;

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

//Not using
//const byte DRIVE_TIME = 1; //in milliseconds
