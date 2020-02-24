// function declaration, included to ensure that we don't get some dumb compile time errors
void onLine();
void goStraight();
float convertIR();
void turning(int IRdirection);
void storeData();
void readSensor();
float percentDiff();
int getMax();
int getMin();
void blink();
void updateTime();
void setMaxSpeed();

unsigned long minutes = 6000; //This will assign 1 minute in real time to minute variable
unsigned long deltaTime; //initialize a change in time variable.
unsigned long prevTime;


/////////// pin definitions
const int IRSensor[] = {A2, A1, A0, A7, A6};//Pins reflect current wiring (2/18/2020)



// to be set when the sensor max and min are known, and put here to improve code flexibility if we change sensors at any point.
const int NUM_SENSORS = 5;

const int SENSOR_MAX = 1024;
const int SENSOR_MIN = 0;

//Initialize max and min IR here, they will be the 'modified' values
float maxIR;
float minIR;

//Initialize IRdirection. This will be a value from -1000 to 1000 that determines our direction, with -1000 is left, 1000 is right, and 0 is forward
float IRdirection;

//Used to limit our max output voltage
const int MAX_VOLTAGE = 5; //volts

//Initialize array to store sensor values
int sensorDataRaw[5];

const int CALIBRATION_TIME = 30 * 1000; // sets the calibration time to 30 seconds.


const int TURN_THRESHOLD = 0.1; // defines the threshold the deviation must reach before turning.

//These are the multipliers were using to assign weight to inner and outer sensors
const int INNER_WEIGHT = 2;
const int OUTER_WEIGHT = 3;
const float CENTER_WEIGHT = 1; //Dont know if we'll need the center weight, I think I found a workaround


// stores the scale factors used in weighted turning.
const float scaleArray[] = { -OUTER_WEIGHT, -INNER_WEIGHT, CENTER_WEIGHT, INNER_WEIGHT, OUTER_WEIGHT};


//Initialize variables for data collection
int logIndex = 0;
const int dataPoints = 50;
int sensorLog[dataPoints][NUM_SENSORS]; //rather than store data sequentially,
//it might be better to use a 2D array to better organize what data points are from the same collection.



//                   initialize motor variables

const byte MAX_SPEED = 255;

// these constants are used to allow you to make your motor configuration
// line up with function names like forward.  Value can be 1 or -1
const int offsetA = 1;
const int offsetB = 1;

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

const byte DRIVE_TIME = 1; //in milliseconds
