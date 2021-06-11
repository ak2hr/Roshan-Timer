#include <Arduino.h>
#include "Bounce2.h"
#include "Adafruit_LEDBackpack.h"
#include "Timer.h"

//function prototypes
void executeFunction();
void startTimer(int startingTime);
void updateDisplay();
void resetTimer();
void startLED();
void steadyLED();
void changeLEDBrightness();
void turnOff();
void blinkClock();
void endBlink();
void endTimer();

//objects
Bounce2::Button button = Bounce2::Button();
Adafruit_7segment clock = Adafruit_7segment();
Timer t;
Timer t2;

//pinConfig
#define BUTTON_PIN 5
#define LED_PIN 6
#define DISPLAY_ADDRESS 0x70

//parameters
int aegisExpire = 5;
int spawnIntervalStart = 8;
int spawnIntervalEnd = 11;

//global variables
long currentTime;
int secondTimer;
int ledTimer;
int buttonTimer;
int ledBrightness = 0;
int fadeAmount = 5;
int presses = 0;

void setup() {
  Serial.begin(9600);
  clock.begin(DISPLAY_ADDRESS);
  clock.setBrightness(0);
  pinMode(LED_PIN, OUTPUT);
  button.attach(BUTTON_PIN, INPUT_PULLUP);
  button.interval(10);
}

void loop() {

  button.update();
  t.update();
  t2.update();

  if(button.pressed()) {
    presses += 1;
    t.stop(buttonTimer);
    buttonTimer = t.after(500, executeFunction);
  }
  
}

void executeFunction() {
  switch(presses) {
    case 1:
      startTimer(1);
      break;
    case 2:
      startTimer((aegisExpire * 60) + 1);
      break;
    case 3:
      resetTimer();
      break;
    default:
      turnOff();
      break;
  }
  presses = 0;
}

void startTimer(int startingTime) {
  steadyLED();
  t.stop(secondTimer);
  currentTime = startingTime;
  secondTimer = t.every(1000, updateDisplay);
}

void updateDisplay() {
  int displayTime = ((currentTime / 60) * 100) + (currentTime % 60);
  Serial.println(displayTime);
  if(currentTime == spawnIntervalEnd * 60) {
    endTimer();
    return;
  } else if(currentTime == spawnIntervalStart * 60) {
    blinkClock();
  } else if(currentTime == (aegisExpire * 60) - 30) {
    blinkClock();
  }
  clock.print(displayTime, DEC);
  if(displayTime < 10) {
    clock.writeDigitNum(3, 0);
  }
  if(displayTime < 100) {
    clock.writeDigitNum(1, 0);
  }
  if(displayTime < 1000) {
    clock.writeDigitNum(0, 0);
  }
  clock.drawColon(true);
  clock.writeDisplay();
  currentTime += 1;
}

void resetTimer() {
  t.stop(secondTimer);
  startLED();
  clock.clear();
  clock.blinkRate(0);
  clock.writeDisplay();
}

void startLED() {
  ledBrightness = 0;
  fadeAmount = 5;
  ledTimer = t.every(30, changeLEDBrightness);
}

void steadyLED() {
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

void turnOff() {
  clock.clear();
  clock.writeDisplay();
  t.stop(ledTimer);
  analogWrite(LED_PIN, 0);
}

void blinkClock() {
  clock.blinkRate(2);
  t.after(3500, endBlink);
}

void endBlink() {
  clock.blinkRate(0);
}

void endTimer() {
  t.stop(secondTimer);
  clock.print(1100, DEC);
  clock.drawColon(true);
  clock.blinkRate(2);
  clock.writeDisplay();
  t2.after(3900, resetTimer);
}