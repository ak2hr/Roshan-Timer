#include <Arduino.h>
#include "Bounce2.h"
#include "Adafruit_LEDBackpack.h"
#include "Timer.h"

//function prototypes
void startTimer();
void updateDisplay();
void endTimer();

//objects
Bounce2::Button button = Bounce2::Button();
Adafruit_7segment clock = Adafruit_7segment();
Timer t;

//pinConfig
#define BUTTON_PIN 5

//parameters
int aegisExpire = 5;
int spawnIntervalStart = 8;
int spawnIntervalEnd = 11;

//global variables
long currentTime;
int secondTimer;
long lastPress;

void setup() {
  Serial.begin(9600);
  button.attach(BUTTON_PIN, INPUT);
  button.interval(10);
}

void loop() {

  button.update();
  t.update();

  if(button.pressed()) {
    startTimer();
  }
  
}

void startTimer() {
  t.stop(secondTimer);
  if(millis() - lastPress < 1000) {
    currentTime = aegisExpire * 60;
  } else {
    currentTime = 1;
  }
  lastPress = millis();
  secondTimer = t.every(1000, updateDisplay);
}

void updateDisplay() {
  int displayTime = ((currentTime / 60) * 100) + (currentTime % 60);
  Serial.println(displayTime);
  if(currentTime == spawnIntervalEnd * 60) {
    endTimer();
  }
  // clock.print(displayTime, DEC);
  // if(displayTime < 1000) {
  //   clock.writeDigitNum(1, 0);
  // }
  // clock.drawColon(true);
  // clock.writeDisplay()
  currentTime += 1;
}

void endTimer() {
  t.stop(secondTimer);
  // clock.print(0000, HEX);
  // clock.drawColon(true);
  // clock.writeDisplay();
}