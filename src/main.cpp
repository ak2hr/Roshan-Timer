#include <Arduino.h>
#include "Bounce2.h"
#include "Adafruit_LEDBackpack.h"
#include "Timer.h"

//function prototypes
void startTimer();
void updateDisplay();
void endTimer();
void startLED();
void stopLED();
void changeLEDBrightness();

//objects
Bounce2::Button button = Bounce2::Button();
Adafruit_7segment clock = Adafruit_7segment();
Timer t;

//pinConfig
#define BUTTON_PIN 5
#define LED_PIN 6

//parameters
int aegisExpire = 5;
int spawnIntervalStart = 8;
int spawnIntervalEnd = 11;

//global variables
long currentTime;
int secondTimer;
int ledTimer;
long lastPress = -1000;
bool secondPress = false;
int ledBrightness = 0;
int fadeAmount = 5;

void setup() {
  Serial.begin(9600);
  pinMode(LED_PIN, OUTPUT);
  button.attach(BUTTON_PIN, INPUT_PULLUP);
  button.interval(10);
  startLED();
}

void loop() {

  button.update();
  t.update();

  if(button.pressed()) {
    startTimer();
  }
  
}

void startTimer() {
  stopLED();
  t.stop(secondTimer);
  if(millis() - lastPress < 1000) {
    if(secondPress) {
      secondPress = false;
      endTimer();
      return;
    } else {
      currentTime = aegisExpire * 60;
      secondPress = true;
    }
  } else {
    currentTime = 1;
  }
  lastPress = millis();
  secondTimer = t.every(1000, updateDisplay);
}

void updateDisplay() {
  secondPress = false;
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
  startLED();
  // clock.print(0000, HEX);
  // clock.drawColon(true);
  // clock.writeDisplay();
}

void startLED() {
  ledBrightness = 0;
  fadeAmount = 5;
  ledTimer = t.every(30, changeLEDBrightness);
}

void stopLED() {
  t.stop(ledTimer);
  analogWrite(LED_PIN, 255);
}

void changeLEDBrightness() {
  ledBrightness += fadeAmount;
  if(ledBrightness == 0 || ledBrightness == 255) {
    fadeAmount = -fadeAmount;
  }
  analogWrite(LED_PIN, ledBrightness);
}