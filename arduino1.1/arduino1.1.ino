/*
This is the program to run the line-following robot for
PoE Lab 3.
Team Members: Hannah Kolano, Chao Li, Harry Young
Primary contact: Hannah Kolano
hannah.kolano@students.olin.edu
Last modified: 10/2/17
*/

//import necessary libraries
#include <Wire.h>
#include <Adafruit_MotorShield.h>
#include "utility/Adafruit_MS_PWMServoDriver.h"

// Create the motor shield object with the default I2C address
Adafruit_MotorShield AFMS = Adafruit_MotorShield(); 

// Select which 'port' motor is connected to
Adafruit_DCMotor *leftMotor = AFMS.getMotor(1);
Adafruit_DCMotor *rightMotor = AFMS.getMotor(2);

//initialize variables and pins
const byte IR_PIN_R = A0;
const byte IR_PIN_L = A1;
int irValR = 0;
int irValL = 0;
int speedL = 0;
int speedR = 0;
byte mode = 0;
int IR_RANGE_LOW = 815;
// int IR_RANGE_HIGH = 870;
int turning_speed = 25;
int default_speed = 40;
String parameter = '';
int new_value = 0;



void setup() {
  // put your setup code here, to run once:
  pinMode(IR_PIN_R, INPUT);
  pinMode(IR_PIN_L, INPUT);
  Serial.begin(9600);
  AFMS.begin();  // create with the default frequency 1.6KHz
}

void loop() {
  // put your main code here, to run repeatedly:
  uint8_t i;

  /*__SENSE__*/
  //check readings of the ir sensors
  irValR = analogRead(IR_PIN_R);
  irValL = analogRead(IR_PIN_L);
  //Serial.print("left"); Serial.print('\t'); Serial.println("right");
  //Serial.print(irValL); Serial.print('\t'); Serial.println(irValR);

  if(Serial.available() > 0) {
    parameter  = Serial.readStringUntil(',');
    Serial.read(); //next character is comma, so skip it using this
    new_value = Serial.readStringUntil('\0');
    Serial.println(parameter)
    Serial.println(new_value)
  } else { 
    parameter = ''
    new_value = 0;
  }


  /*__THINK__*/
  //make different commands based on how different the readings are
  if(irValL > IR_RANGE_LOW && irValR < IR_RANGE_LOW) {         //turns left
    mode = 1;
  } else if(irValL < IR_RANGE_LOW && irValR > IR_RANGE_LOW) { //turns right 
    mode = 2;
  } else {                    //pretty much the same
    mode = 3;
  }

  /*__ACT__*/
  leftMotor->run(BACKWARD);
  rightMotor->run(BACKWARD);
  
  //makes sure robot is moving first
  if(speedL <= 10 && speedR <= 10) {
    for (i=0; i<default_speed; i++) {
      leftMotor->setSpeed(i);
      rightMotor->setSpeed(i);
      speedL = default_speed;
      speedR = default_speed;
      delay(10);      
    }  
  }
  //changes motor readings to deal with said reading
  switch(mode) {
    case 1: {
      veer_left();
      break;
    } case 2: {
      veer_right();
      break;
    } default: {
      go_straight();
      break;
    }
  }
  leftMotor->setSpeed(speedL);
  rightMotor->setSpeed(speedR);
  delay(25);
}

void veer_left() {
  Serial.println("veer left");
  speedL = 0;
  speedR = turning_speed;
  //speedL = speedL; //- ((irValL-IR_RANGE_LOW)/10);
  //speedR = speedR + ((irValL-IR_RANGE_LOW)/10);
}

void veer_right() {
  Serial.println("veer right");
  speedL = turning_speed;
  speedR = 0;
  //speedL = speedL + ((irValR-IR_RANGE_LOW)/10);
  //speedR = speedR; // - ((irValR-IR_RANGE_LOW)/5);
}

void go_straight() {
  Serial.println("forward");
  speedL = default_speed;
  speedR = default_speed;
  //speedL = speedL + (default_speed - speedL)/2;
  //speedR = speedR + (default_speed - speedR)/2;
}

/*
Possible helpfulness/extensions
-goes faster when has been straight for a bit
-conversely slows down when hasn't 
-PID control?
*/
