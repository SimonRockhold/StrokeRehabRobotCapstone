//Download library here https://learn.sparkfun.com/tutorials/tb6612fng-hookup-guide#library-and-example-code, manually import by copying library to User\Documents\Arduino\libraries
#include <SparkFun_TB6612.h>
#include "common.h"

Motor motor1 = Motor(AIN1, BIN1, PWMA, offsetA, STBY);
Motor motor2 = Motor(AIN2, BIN2, PWMB, offsetB, STBY);

void setup()
{
  Serial.begin(9600);
  Serial.println("Connected");

  //Call calibrate function in setup, comment this out if not needed
  calibrate();

  //Wait 5 seconds before driving
  delay(5000);
}

//////////////////////// loop

void loop()
{
  Serial.println("");
  readSensor(); //Collects data from sensors and stores in an array
  delay(500);
  //storeData();
  //printData();
  //Serial.println(analogRead(IRSensor[0]));
  //Serial.println("value");

  onLine(); //Detects if on line, NEEDS MORE WORK

  if (percentDiff() < TURN_THRESHOLD)
  { //This function will check to see if there is a significant difference between the left and right sensors, if there is, it will move on to turning prompts, if not it will just drive straight

    goStraight();
  }

  else
  {
    turning(convertIR()); //starts reading and finds where line is relative to middle
  }
  //This will store sensor data in a large array
  storeData(); //changed implementation, see storeData() method
}

void calibrate()
{

  //collects sensor data and defines the maximum and minimum line brihtness values

  //delay(5000); //waits 5 seconds to start calibrating

  //initialize temporary max and min, set max equal to real min, min equal to real max
  int tempMax = SENSOR_MIN;
  int tempMin = SENSOR_MAX;

  prevTime = millis();
  while (deltaTime < CALIBRATION_TIME)
  { // perform calibration for set time.
    //updateTime();
    deltaTime = millis() - prevTime;

    readSensor(); //populate sensor data array

    //find max and min over x minutes
    tempMax = getMax(tempMax);
    tempMin = getMin(tempMin);
  }

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

float percentDiff()
{

  //This will determine percent difference between outer sensors, and return that difference to the conditional in  the loop

  return ((abs((sensorDataRaw[0] + sensorDataRaw[1]) - (sensorDataRaw[3] + sensorDataRaw[4]))) / ((sensorDataRaw[0] + sensorDataRaw[1] + sensorDataRaw[3] + sensorDataRaw[4]) / 2));
}

// smaller version of getMax and getMin using loops
int getMax(int tempMax)
{
  //Compares all readings from the IR sensors to the temporary max
  for (int i = 0; i < NUM_SENSORS; i++)
  {
    tempMax = max(IRSensor[i], tempMax); // sets tempMax to be the larger of the two between each sensor reading and the previous tempMax.
  }
  return tempMax; //Returns tempMax to the calibrate while loop
}

int getMin(int tempMin)
{
  //Compares all readings from the IR sensors to the temporary minimum.
  for (int i = 0; i < NUM_SENSORS; i++)
  {
    tempMin = min(IRSensor[i], tempMin);
  }
  return tempMin; //Returns tempMin to the calibrate while loop
}

//This should be a better way to find a turning value

//This should be a better way to find a turning value
float convertIR()
{
  int outputArr[NUM_SENSORS]; //Create temporary array to store values
  int sum; //Initialize sum variable
  for (int i = 0; i < NUM_SENSORS; i++)
  {
    //if (i != 2)
    //{
      outputArr[i] = sensorDataRaw[i] * scaleArray[i]; //stores scaled values
    //}
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

  //This will constrain to -3*maxIR, 3*maxIR.
  sum = constrain(sum, -OUTER_WEIGHT * maxIR, OUTER_WEIGHT * maxIR);

  //maps to -1000 to 1000
  //return map(sum, -MAX_SPEED, MAX_SPEED, -OUTER_WEIGHT * maxIR, OUTER_WEIGHT * maxIR);
  float temp = map(sum, 0, 1000, -OUTER_WEIGHT * maxIR, OUTER_WEIGHT * maxIR);
  return temp/1000;
}

//This will make the motors stop in the case that it doesn't detect any line, could later involve turning until it detects something
//May want to change this to say if all sensors are within ~5%, just incase they read small variations from eachother
void onLine()
{
  bool lineDetected = false;
  for (int i = 0; i < NUM_SENSORS; i++)
  {
    if (sensorDataRaw[i] < minIR)
    {                      //checks each sensor to see if a line is detected
      lineDetected = true; //if line is found, set lineDetected flag to true
      break;               // leave the loop, no reason to check other sensors.
    }
  }
  if (!lineDetected)
  { //if no line was found, brake for set time.
    brake(motor1, motor2);
    //Code here to stop voltage output to motors, or maybe for testing it could 'shake' the robot
    delay(0.5 * minutes);
  }
}

//This will take in our number that defines the direction based on which IR sensors are sensing and by how much
void turning(int IRdirection)
{

  //We will need to change these based on how the motors are oriented/wired
  if (IRdirection > 0)
  {

    motor1.drive(IRdirection, DRIVE_TIME);
    motor2.drive(MAX_SPEED - IRdirection, DRIVE_TIME);
  }
  else if (IRdirection < 0)
  {

    motor1.drive(MAX_SPEED + IRdirection, DRIVE_TIME);
    motor1.drive(-IRdirection, DRIVE_TIME);
  }
}

void goStraight()
{
  //This will store the code for going straight, probably just output constant voltage to both motors equally

  forward(motor1, motor2, DRIVE_TIME);
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

void readSensor()
{
  //Serial.println("readSensor()");
  for (int x = 0; x < NUM_SENSORS; x++)
  {
    //Serial.println(analogRead(IRSensor[x]));
    sensorDataRaw[x] = analogRead(IRSensor[x]);
  }
}

void blink()
{
  pinMode(13, OUTPUT);
  for (int i = 0; i < 10; i++)
  {
    digitalWrite(13, HIGH);
    delay(1000);
    digitalWrite(13, LOW);
  }
}

void updateTime()
{
  deltaTime = prevTime - millis();
  //if (deltaTime >= duration){
  // prevTime = millis();
  //}
}

void propForward(Motor motor1, Motor motor2, int ratio)
{
  motor1.drive(MAX_SPEED * ratio);
  motor2.drive(MAX_SPEED * (1 - ratio));
}
