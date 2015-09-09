#include <Wire.h>
#include <Adafruit_MotorShield.h>
#include "utility/Adafruit_PWMServoDriver.h"

#define BAUD_RATE 9600

#define X_SWITCH_PIN 10
#define Y_SWITCH_PIN 11
#define Z_SWITCH_PIN 12

#define LINE_UP_DISTANCE 10
#define VALVE_TRAVEL 10
#define DISPENSE_TIME 10
#define REFILL_TIME 10

boolean busy = true;
int current_x = 0;
int current_y = 0;

Adafruit_MotorShield AFMS = Adafruit_MotorShield();  

Adafruit_StepperMotor *xMotor = AFMS.getStepper(200, 1);
Adafruit_StepperMotor *yMotor = AFMS.getStepper(200, 2);
Adafruit_StepperMotor *zMotor = AFMS.getStepper(200, 3);
  
void setup() {
  
  Serial.begin(BAUD_RATE);
  
  pinMode(X_SWITCH_PIN, INPUT);
  pinMode(Y_SWITCH_PIN, INPUT);
  
  AFMS.begin();
  xMotor->setSpeed(60);
  yMotor->setSpeed(60);
  yMotor->setSpeed(60);
    
  zero();
  
  busy = false;
}

void loop() {
  
  
  moveTo(x, y);
  dispense(n);
  zero();
}

void moveTo(int x, int y) {
  
  int x_difference = x - current_x;
  int y_difference = y - current_y;
  int x_direction = 1;
  int y_direction = 1;
  
  if(x_difference < 0) x_direction = 2;
  if(y_difference < 0) y_direction = 2;
  
  while(current_x != x || current_y != y)
  {  
    if(current_x != x) 
    {
      xMotor->step(1, x_direction, INTERLEAVE);
      current_x++;
    }
    if(current_y != y) 
    {
      yMotor->step(1, y_direction, INTERLEAVE);
      current_y++;
    }
  }
}

void dispense(int count) {
  
  zMotor->step(LINE_UP_DISTANCE, FORWARD, INTERLEAVE);
  
  for(int shot = 0; shot < count; shot++)
  {  
    zMotor->step(VALVE_TRAVEL, FORWARD, INTERLEAVE);
    delay(DISPENSE_TIME);
    zMotor->step(VALVE_TRAVEL, BACKWARD, INTERLEAVE);
    if((shot + 1) < count) delay(REFILL_TIME);
  }
  
  zMotor->step(LINE_UP_DISTANCE, BACKWARD, INTERLEAVE);  
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
