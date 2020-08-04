/*
 * This will test the ultrasound and display the time it took to reach out and interact
 */
#include <Adafruit_SSD1306.h>
#include <Adafruit_GFX.h>
#include "common.h"
#include <SPI.h>

Adafruit_SSD1306 display = Adafruit_SSD1306(128, 64, &Wire);
unsigned long time;
unsigned long duration;
unsigned long elapsed;
bool isFirst = True;

void setup() {

  display.begin(SSD1306_SWITCHCAPVCC, 0x3C); // begins display, displays arduino logo by default
  display.clearDisplay(); //This will clear the adafruit logo

  display.setTextSize(1);
  display.setCursor(0, 0);

  display.print("Starting");
  display.display();
  delay(3000);
  display.clearDisplay();

}

void loop() {

  byte distance = runUltrasound();
  time = millis();
  if (distance <= 6) {

      elapsed += time;
      if(!isFirst) {
        duration = time - elapsed;
      }
      
      else if(isFirst) {
        duration = time;
        isFirst = False;
      }

      printDisplay(duration);
  }


}

byte runUltrasound() {
  
  pinMode(trigger, OUTPUT);
  digitalWrite(trigger, LOW);
  delayMicroseconds(2);
  digitalWrite(trigger, HIGH);
  delayMicroseconds(5);
  digitalWrite(trigger, LOW);
  
  duration = pulseIn(echo, HIGH);

  // convert the time into a distance
  return byte(duration / 74 / 2);
}

void printStart() {

    display.clearDisplay();
    display.print("Interact");
    display.display();
}

void printDisplay(float duration) {
  display.clearDisplay();
  display.print("duration (seconds): ");
  display.print(duration/1000);
  display.display();

  delay(3000);
}