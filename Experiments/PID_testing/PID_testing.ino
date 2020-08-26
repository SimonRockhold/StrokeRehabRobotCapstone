#include <SPI.h>
#include <Wire.h>
#include <SparkFun_TB6612.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "common.h"
#include "Timer.h"


using namespace defs; //inlcudes all definitions made in common.h

Adafruit_SSD1306 display = Adafruit_SSD1306(128, 64, &Wire);

namespace //limits the scope of decalations inside namespace to this file.
{
  Motor leftMotor = Motor(AIN1, AIN2, PWMA, offsetA, STBY); //I need to check the motor code I wrote for testing to change these. I know offset needs to be changed
  Motor rightMotor = Motor(BIN1, BIN2, PWMB, offsetB, STBY);

  Timer calibrationTimer = Timer(CALIBRATION_TIME);
  Timer outputTimer = Timer(SECOND);  // one second interval between outputs
  Timer logTimer = Timer(SECOND * 5); // set a timer to log all values during that time.

  int maxIR = SENSOR_MAX;
  int minIR = SENSOR_MIN;
  //int maxIR = 800; //not currently used as calibrate method is functional
  //int minIR = 180;

  float Kp = 26; //This will change using the button during testing
  float Kd = 90; //This will change using the button during testing
  float Ki = 0;
  float P, I, D;
  float direction;
  
  float error;
  float previousError;

  long duration;

  //For the button sensitivity
  int buttonState;
  int lastButtonState = LOW;

  unsigned long lastTime = 0;
  unsigned long debounce = 75;


  // SPECIFICALLY FOR THIS TEST
  int increaseBy = 0;

} // namespace

void setup()
//all actions that are only done once
{
  pinMode(13, OUTPUT); //only needed once so done in setup()
  pinMode(BUTTON, INPUT);
  Serial.begin(9600);
  //Serial.println("Connected"); //quick check to make sure device is communicating

  display.begin(SSD1306_SWITCHCAPVCC, 0x3C); // begins display, displays arduino logo by default
  display.clearDisplay(); //This will clear the adafruit logo
  
  display.setTextSize(2);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);

  display.println("Starting");
  display.display();
  delay(3000);
  display.clearDisplay();
  //exit(0);

}

//////////////////////// loop

void loop()
{
  readSensor(); //Collects data from sensors and stores in an array
  direction = calculatePID();
  propForward(direction);
  PIDadjust();
  updatePID();
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
  int speed1 = SPEED + PIDval + increaseBy;
  int speed2 = SPEED - PIDval;

  //Added to make sure speeds don't exceed 255 or dip below 0, not sure if necessary but have noticed straing behavior
  speed1 = constrain(speed1, 0, 255);
  speed2 = constrain(speed2, 0, 255);
  
  leftMotor.drive(speed2);
  rightMotor.drive(speed1);
}

// Adjust Kp below to change to testing Kd or Ki, also adjust some values in common.h
void PIDadjust() {

  int state = digitalRead(BUTTON);
  /*
  //Approach 1 (no adjustment for noise)
  if (state == HIGH) {
    Kp += increaseBy;
  }
  */
  
  //Approach 2 (adjusts for noise)

  //Stores time button changed from LOW to HIGH or vice versa
  if(state != lastButtonState) {
    lastTime = millis();
  }

  //Checks if button has held the same state for <debounce> milliseconds
  if ((millis() - lastTime) > debounce) {

    //If the current state is different than what the button was previously, update it
    if (state != buttonState) {
      buttonState = state;

      //Checks to see if button is currently HIGH
      if (buttonState == HIGH) {
        increaseBy += 1; //Specifically for Kp, can update for Kd and Ki
        brake(leftMotor, rightMotor);
        delay(1000);
      }
    }
  }
  //Updates previous button state
  lastButtonState = state;
  
  
  
}

void updatePID() {
  display.clearDisplay();
  display.setCursor(0, 0);
  display.println(increaseBy); //Adjust this for changing Kd or Ki
  float temp = getRatio();
  display.println(temp);
  display.display();
}
