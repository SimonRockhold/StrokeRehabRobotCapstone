//Download library here https://learn.sparkfun.com/tutorials/tb6612fng-hookup-guide#library-and-example-code, manually import by copying library to User\Documents\Arduino\libraries
#include <SparkFun_TB6612.h>
#include "common.h"
#include "Timer.h"

using namespace defs; //inlcudes all definitions made in common.h

namespace //limits the scope of decalations inside namespace to this file.
{
Motor motor1 = Motor(AIN1, AIN2, PWMA, offsetA, STBY); //I need to check the motor code I wrote for testing to change these. I know offset needs to be changed
Motor motor2 = Motor(BIN1, BIN2, PWMB, offsetB, STBY);

Timer calibrationTimer = Timer(CALIBRATION_TIME);
Timer outputTimer = Timer(SECOND); // one second interval between outputs
int logIndex = 0;                  // used by calibrate method
int sensorLog[dataPoints][NUM_SENSORS];
int maxIR = 775;
int minIR = 230;
} //end namespace init

void setup()
//all actions that are only done once
{
  pinMode(13, OUTPUT); //only needed once so done in setup()
  Serial.begin(9600);
  Serial.println(F("Connected")); //quick check to make sure device is communicating
  //calibrate();

  delay(5000);
}

//////////////////////// loop

void loop()
{
  //Serial.print("loop");
  readSensor(); //Collects data from sensors and stores in an array
  IRdirection = getRatio();
  propForward(IRdirection);
}

float percentDiff()
{
  //This will determine percent difference between outer sensors, and return that difference to the conditional in  the loop

  return ((abs((sensorDataRaw[0] + sensorDataRaw[1]) - (sensorDataRaw[3] + sensorDataRaw[4]))) / ((sensorDataRaw[0] + sensorDataRaw[1] + sensorDataRaw[3] + sensorDataRaw[4]) / 2));
}

//This should be a better way to find a turning value

float getRatio()
{
  //Serial.print(F("getRatio"));
  int outputArr[NUM_SENSORS]; //Create temporary array to store values
  int sum = 0;                //Initialize sum variable

  for (int i = 0; i < NUM_SENSORS; i++)
  {
    outputArr[i] = sensorDataRaw[i] * weightArray[i]; //stores weighted values
  }

  for (int i = 0; i < NUM_SENSORS; i++)
  {
    if (i != 2)
    {
      sum = sum + outputArr[i];
    }
  }

  //This will include the center sensor. We're assuming that the center sensor has a fairly small value
  //(handled by the turn threshold), however if the line is between the center and inner sensors, we want to still
  //consider the center sensor value

  if (sum > 0)
  {
    sum = sum + (CENTER_WEIGHT * outputArr[2]);
  }
  else if (sum < 0)
  {
    sum = sum - (CENTER_WEIGHT * outputArr[2]);
  }

  //This will constrain sum incase somehow it reaches values we wouldn't expect (likely to come up for very sharp turns).
  //May want to try and remove this
  sum = constrain(sum, (-OUTER_WEIGHT * maxIR), (OUTER_WEIGHT * maxIR));

  //maps to 0 to 1, try and mess with this
  float temp = map(sum, (-OUTER_WEIGHT * maxIR), (OUTER_WEIGHT * maxIR), 0, 1000.00); //Values were in wrong order, they've been rearranged

  //Printing for debugging
  Serial.print("Sum: ");
  Serial.print(sum);

  Serial.print(" ratio: ");
  Serial.print(temp / 1000.00);

  Serial.println(" ");

  return (temp / 1000.00);
}

//returns whether or not a line is detected.
bool onLine()
{
  bool lineDetected = false;
  for (int i = 0; i < NUM_SENSORS; i++)
  {
    if (sensorDataRaw[i] >= (minIR * 1.3)) //May want to change the 1.3
    {                                      //checks each sensor to see if a line is detected
      lineDetected = true;                 //if line is found, set lineDetected flag to true
      break;                               // leave the loop, no reason to check other sensors.
    }
  }
  return lineDetected; // returns true if a line is detected, false otherwise
}

void readSensor()
{
  //Serial.println("readSensor()");
  for (int x = 0; x < NUM_SENSORS; x++)
  {
    //Serial.println(analogRead(IRSensor[x]));
    sensorDataRaw[x] = analogRead(IRSensor[x]);
  }
}

void blink() //Changed blink to only blink once, but for the online function it calls blink() as long as the device isn't on a line
{
  digitalWrite(13, HIGH);
  delay(500);
  digitalWrite(13, LOW);
}

void propForward(float ratio)
{
  int speed1 = SPEED * ratio;
  int speed2 = SPEED * (1 - ratio);
  motor1.drive(speed1);
  motor2.drive(speed2);
}

//It would be nice to eventually include a calibrate function, however for now we can simply hard code the IR max and min values based on
//results we get from our IR sensor only test code

//This is all code for debugging and calibrating, for now it may not be needed

void calibrate()
{
  //collects sensor data and defines the maximum and minimum line brihtness values

  //initialize temporary max and min, set max equal to real min, min equal to real max
  int tempMax = SENSOR_MIN;
  int tempMin = SENSOR_MAX;
  Serial.println("calibrating...");
  while (!calibrationTimer.timeElapsed())
  {               // perform calibration for set time.
    readSensor(); //populate sensor data array

    //find max and min over x minutes
    tempMax = getMax(tempMax);
    tempMin = getMin(tempMin);
  }
  Serial.println("calibration complete");
  Serial.println(tempMax);
  Serial.println(tempMin);

  //Now that we have the absolute max and min the sensors found, we can modify those to find an operational range. This will hopefully account for outliers
  //Modifiers can be increase or decreased as needed
  //Honestly, we might want to completely remove modifiers, as there's a chance in the IRdirection function it'll create negatives where we don't want them

  maxIR = 0.9 * tempMax;
  minIR = 1.1 * tempMin;
}

void printData()
{
  // prints debug info to serial connection
  Serial.println("calibrated min and max: ");
  Serial.println(maxIR);
  Serial.println(minIR);
  Serial.println();
  int readoutIndex = 0;
  while (readoutIndex < logIndex)
  {
    for (int i = 0; i < NUM_SENSORS; i++)
    {
      Serial.print(sensorLog[readoutIndex][i]);
      Serial.print(", ");
    }
    Serial.println(readoutIndex);
  }
}

// smaller version of getMax and getMin using loops
int getMax(int tempMax)
{
  //Compares all readings from the IR sensors to the temporary max
  for (int i = 0; i < NUM_SENSORS; i++)
  {
    tempMax = max(sensorDataRaw[i], tempMax); // sets tempMax to be the larger of the two between each sensor reading and the previous tempMax.
  }
  return tempMax; //Returns tempMax to the calibrate while loop
}

int getMin(int tempMin)
{
  //Compares all readings from the IR sensors to the temporary minimum.
  for (int i = 0; i < NUM_SENSORS; i++)
  {
    tempMin = min(sensorDataRaw[i], tempMin);
  }
  return tempMin; //Returns tempMin to the calibrate while loop
}

void storeData()
{
  //This will store sensor data, use just for debugging/information presenting
  if (logIndex < dataPoints)
  { // checks whether or not sensorLog is full.
    for (int i = 0; i < NUM_SENSORS; i++)
    {
      sensorLog[logIndex][i] = sensorDataRaw[i];
    }
    logIndex++; //increments the log index value.
  }
}
