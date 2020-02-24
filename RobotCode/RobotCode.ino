
//Download library here https://learn.sparkfun.com/tutorials/tb6612fng-hookup-guide#library-and-example-code, manually import by copying library to User\Documents\Arduino\libraries
#include <SparkFun_TB6612.h>
#include "common.h"

Motor motor1 = Motor(AIN1, AIN2, PWMA, offsetA, STBY); //I need to check the motor code I wrote for testing to change these. I know offset needs to be changed
Motor motor2 = Motor(BIN1, BIN2, PWMB, offsetB, STBY);

void setup()
{
  Serial.begin(9600);
  Serial.println("Connected");

  //Call calibrate function in setup, We'll work on that code shortly
  //calibrate();

  //Wait 5 seconds before driving
  delay(5000);
}

//////////////////////// loop

void loop()
{

  readSensor(); //Collects data from sensors and stores in an array
  IRdirection = convertIR(); //Gets ratio from 0 to 1, going to try and move this higher up to see if maybe it helps?


  onLine(); //Detects if on line, NEEDS MORE WORK

  delay(500); //Adding this to be able to read/print percent diff values and see if it's a processing speed issue
  
  
  if (tempPercentDiff <= TURN_THRESHOLD)
  { //This function will check to see if there is a significant difference between the left and right sensors, if there is, it will move on to turning prompts, if not it will just drive straight

    forward(motor1, motor2, (MAX_SPEED * 0.6));
    straightForward();
  }

  else
  {
    propForward(IRdirection); //Changes made in the motor library 
  }
  //This will store sensor data in a large array, not using rn
  //storeData();
  
  //This will allow it to follow a line for a predetermined amount of time
  if(millis() >= runTime) {
    brake(motor1,motor2);
    blink();
}


  //Print things for debugging
  for(int i = 0; i < NUM_SENSORS; i++) {
    Serial.print(sensorDataRaw[i]);
    Serial.print(", ");
  }

  Serial.print("ratio: ");
  Serial.print(IRdirection);
  Serial.println(" ");

  delay(500); //Adding this to let things print/read data

    }


float percentDiff()
{

  //This will determine percent difference between outer sensors, and return that difference to the conditional in  the loop

  return ((abs((sensorDataRaw[0] + sensorDataRaw[1]) - (sensorDataRaw[3] + sensorDataRaw[4]))) / ((sensorDataRaw[0] + sensorDataRaw[1] + sensorDataRaw[3] + sensorDataRaw[4]) / 2));
}

//This should be a better way to find a turning value
float convertIR()
{
  float outputArr[NUM_SENSORS]; //Create temporary array to store values
  float sum = 0; //Initialize sum variable
  
  for (int i = 0; i < NUM_SENSORS; i++)
  {
      outputArr[i] = sensorDataRaw[i] * scaleArray[i]; //stores scaled values
  }

  for (int i = 0; i < NUM_SENSORS; i++)
  {
    if(i != 2) {
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
  Serial.print(temp/1000.00);

  Serial.println(" ");
  
  return (temp / 1000.00);
}

//Continually runs until the sensors detect a line (will be stuck in while loop while all sensors read < minIR*1.1)
void onLine()
{
  bool lineDetected = false;
  for (int i = 0; i < NUM_SENSORS; i++)
  {
    if (sensorDataRaw[i] >= (minIR * 1.3)) //May want to change the 1.3
    { //checks each sensor to see if a line is detected
      lineDetected = true; //if line is found, set lineDetected flag to true
      break;               // leave the loop, no reason to check other sensors.
    }
  }
  
  while (!lineDetected) //changed this notation
  { //if no line was found, brake for set time.
    brake(motor1, motor2);
    blink();
    readSensor();
    onLine(); //Brakes motors and keeps blinking while not on the line
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

void blink() //Changed blink to only blink once, but for the online function it calls blink() as long as the device isn't on a line
  {
  pinMode(13, OUTPUT);

  digitalWrite(13, HIGH);
  delay(500);
  digitalWrite(13, LOW);
  }







void updateTime()
  {
  //Make sure to always use prevTime = millis() before calling this function, or else it doesn't work
  deltaTime = prevTime - millis();

}

void propForward(float ratio)
{
  motor1.drive(((MAX_SPEED * ratio)), DRIVE_TIME);
  motor2.drive((MAX_SPEED * (1 - ratio)), DRIVE_TIME);
  }

//Try this in order to resolve issues around the normal forward() method being blocking
void straightForward() {
  motor1.drive((MAX_SPEED * 0.6), DRIVE_TIME);
  motor2.drive((MAX_SPEED * 0.6), DRIVE_TIME);
}









//It would be nice to eventually include a calibrate function, however for now we can simply hard code the IR max and min values based on
//results we get from our IR sensor only test code

//This is all code for debugging and calibrating, for now it may not be needed

/*void calibrate()
{

  //collects sensor data and defines the maximum and minimum line brihtness values

  //delay(5000); //waits 5 seconds to start calibrating

  //initialize temporary max and min, set max equal to real min, min equal to real max
  int tempMax = SENSOR_MIN;
  int tempMin = SENSOR_MAX;

  prevTime = millis();
  while (deltaTime < CALIBRATION_TIME)
  { // perform calibration for set time.
    updateTime();

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
*/
