#include <Wire.h>
#include <Adafruit_MotorShield.h>
#include "utility/Adafruit_PWMServoDriver.h"

#define BAUD_RATE 9600
#define X_SWITCH_PIN 10
#define Y_SWITCH_PIN 11

boolean busy = true;
int current_x = 0;
int current_y = 0;

Adafruit_MotorShield AFMS = Adafruit_MotorShield();  
Adafruit_StepperMotor *xMotor = AFMS.getStepper(200, 1);
Adafruit_StepperMotor *yMotor = AFMS.getStepper(200, 2);
  
void setup() {
  
  Serial.begin(BAUD_RATE);
  
  pinMode(X_SWITCH_PIN, INPUT);
  pinMode(Y_SWITCH_PIN, INPUT);
    
  AFMS.begin();
  xMotor->setSpeed(60);
  yMotor->setSpeed(60);
  
  zero();
  
  busy = false;
}

void loop() {
  
  zero();
}

void pos(int x, int y) {
  
  int x_difference = x - current_x;
  int y_difference = y - current_y;
}

void dispense(int count) {
  
  
}

void zero() {
  
  boolean x_open = digitalRead(X_SWITCH_PIN);
  boolean y_open = digitalRead(Y_SWITCH_PIN);
    
  while(x_open | y_open)
  {
    if(x_open) xMotor->step(1, BACKWARD, INTERLEAVE);
    if(y_open) yMotor->step(1, BACKWARD, INTERLEAVE);
    
    x_open = digitalRead(X_SWITCH_PIN);
    y_open = digitalRead(Y_SWITCH_PIN);
  }
  
  current_x = 0;
  current_y = 0;
}
