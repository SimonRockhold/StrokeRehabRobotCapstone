#include <SparkFun_TB6612.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_GFX.h>
#include "common.h"
#include "Timer.h"
#include "SDCard.h"
#include "Calibration.h"
#include "oled_screen.h"
#include <SD.h>
#include <SPI.h>

using namespace defs; //inlcudes all definitions made in common.h

namespace //limits the scope of decalations inside namespace to this file.
{
  Motor leftMotor = Motor(AIN1, AIN2, PWMA, offsetA, STBY); //I need to check the motor code I wrote for testing to change these. I know offset needs to be changed
  Motor rightMotor = Motor(BIN1, BIN2, PWMB, offsetB, STBY);

  Timer calibrationTimer = Timer(CALIBRATION_TIME);
  Timer outputTimer = Timer(SECOND);  // one second interval between outputs
  Timer logTimer = Timer(SECOND * 5); // set a timer to log all values during that time.

  SDCard sensorRecord = SDCard(SD_CS, "sensors.csv");
  SDCard calibrationVals = SDCard(SD_CS, "calibr.dat");

  OledScreen screen = OledScreen();

  int maxIR = SENSOR_MAX;
  int minIR = SENSOR_MIN;
  //int maxIR = 800; //not currently used as calibrate method is functional
  //int minIR = 180;

  float Kp; //This will change using the potentiometer during testing
  float Kd; //This will change using the potentiometer during testing
  float Ki;
  float P, I, D;
  float direction;

  float voltage;
  float error;
  float previousError;
  float desiredCoefficient; //For testing
} // namespace

void setup()
//all actions that are only done once
{
  pinMode(13, OUTPUT); //only needed once so done in setup()
  //Serial.begin(9600);
  //Serial.println("Connected"); //quick check to make sure device is communicating

  startup();
}

void startup()
//distinct from setup as is used to begin user related behavior rather than hardware requirements.
{
  welcomeMessage(); //will display a welcome message
  pinMode(BUTTON, INPUT);
  Timer calibrateStartupTimer = Timer(5 * SECOND);
  //wait for 5 seconds at startup. If button is pressed, recalibrate. otherwise, load from file.
  bool newCalibration = false;
  while (!calibrateStartupTimer.timeElapsed())
  {
    if (digitalRead(BUTTON))
    {
      newCalibration = true;
      break;
    }
  }
  if (newCalibration)
  {
    calibrate();
  }
  else
  {
    calibrationFromFile();
  }

  screen.main_menu();
  //menu selection not yet implemented.
  runSession();
}

//////////////////////// loop

void loop()
{
}

void lineFollowTest()
// Robot follows the line indefinitely, logs sensor data for duration set by logTimer.
{
  Kp = getCoefficient(); //For testing PID values, set testing for Kp or Kd by changing values here

  readSensor(); //Collects data from sensors and stores in an array
  direction = calculatePID();
  propForward(direction);

  if (!logTimer.timeElapsed())
  {
    logToSD();
  }
  else
  {
    //Serial.println("done");
    digitalWrite(13, HIGH);
  }
}

void runSession()
{
  followLine();
  screen.display_score();
  // waits until button is pressed, then returns to menu
  while (true)
  {
    if (digitalRead(BUTTON))
    {
      menu();
    }
  }
}

void menu()
// menu selection not yet implemented
{
  screen.main_menu();
}

void followLine()
{
  Timer runTimer = Timer(SESSION_DURATION);
  while (!runTimer.timeElapsed())
  {
    Kp = getCoefficient(); //For testing PID values, set testing for Kp or Kd by changing values here

    readSensor(); //Collects data from sensors and stores in an array
    direction = calculatePID();
    propForward(direction);
  }
  screen.display_score(); //Display score, wait for 5 seconds without input, then allow user to press button to continue.
  delay(5 * SECOND);
}
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

  return (temp / 1000.00);
}

float calculatePID()
{
  //We want to redefine previousError first, or else previousError and error will always be the same
  previousError = error;
  //error was changed. Not sure if the order matters, but i read that error = desiredPoint - actualPoint
  error = 0.5 - getRatio();

  P = error;
  I = I + error;
  D = error - previousError;
  Serial.println(Kp); //Can't print while device is running, since potentiometer saves physical location the correct Kp should print whenr recconnected

  return (Kp * P) + (Ki * I) + (Kd * D);
}

void readSensor()
{
  for (int x = 0; x < NUM_SENSORS; x++)
  {
    sensorDataRaw[x] = analogRead(IRSensor[x]);
  }
}

void propForward(float PIDval)
{
  int speed1 = SPEED + PIDval;
  int speed2 = SPEED - PIDval;

  leftMotor.drive(speed1);
  rightMotor.drive(speed2);
}

// For testing with a potentiometer
float getCoefficient()
{
  voltage = analogRead(A3);
  desiredCoefficient = map(voltage, 0, 1023, 0, 500); //Change range of Kp/Kd values here (Syntax: map(value, fromLow, fromHigh, toLow, toHigh))

  return desiredCoefficient;
}

void calibrate()
{
  //collects sensor data and defines the maximum and minimum line brightness values

  //initialize temporary max and min, set max equal to real min, min equal to real max
  int tempMax = SENSOR_MIN;
  int tempMin = SENSOR_MAX;
  Serial.println("calibrating...");
  //writeToScreen("calibrating..."); //so far unimplemented, to replace printing information to serial
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

  Calibration values;
  calibrationVals.write((byte *)&values, sizeof(values));
}

void logToSD()
//writes the current state of the sensor array to a file called "SensorLog.txt" one line at a time;
{
  String temp = "";
  for (int i = 0; i < NUM_SENSORS; i++)
  {
    temp += sensorDataRaw[i];
    if (i < (NUM_SENSORS - 1))
    {
      temp += ", ";
    }
  }
  sensorRecord.printToSD(temp);
  Serial.println(temp);
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

void welcomeMessage()
//displays a welcome message
{
  screen.print_text("Welcome!");
}

void calibrationFromFile()
//sets the calibration values from the previous values stored on SD in "calibr.dat"
{
  Serial.println("reading previous calibration values from file");

  Calibration vals;
  calibrationVals.read((byte *)&vals, sizeof(vals));
  maxIR = vals.max_ir;
  minIR = vals.min_ir;
}
