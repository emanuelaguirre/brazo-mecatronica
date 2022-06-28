/*  Configuration file - Libraries / Pinout / Calibration values
 *  Written by: Emanuel Aguirre
 *  Date: 17 June 2022
 */

#ifndef conf_h
#define conf_h

#pragma once

/********************** Libraries included **********************/
#include <Arduino.h>
#include <Servo.h>

/**************************** Pinout ****************************/
// Physical button 
#define BUTTON 2  // Interrupt available

// SW Configuration
const int _recorded_points_amount = 30;

// Servo Manager
#define SERVO_PWM_1 11 
#define SERVO_PWM_2 10
#define SERVO_PWM_3 9
#define SERVO_PWM_4 6 

Servo base;
Servo shoulder;
Servo elbow;
Servo wrist;

void attachServos (){ 
    base.attach(SERVO_PWM_1);
    shoulder.attach(SERVO_PWM_2);
    elbow.attach(SERVO_PWM_3);
    wrist.attach(SERVO_PWM_4);
}

void detachServos (){ 
    base.detach();
    shoulder.detach();
    elbow.detach();
    wrist.detach();
}

// Analog inputs
#define ANALOG_POS_1 A0 // GPIO 14 -> base
#define ANALOG_POS_2 A1 // GPIO 15 -> shoulder
#define ANALOG_POS_3 A2 // GPIO 16 -> elbow
#define ANALOG_POS_4 A3 // GPIO 17 -> wrist 


void beginConf() {
  Serial.begin(115200);
  Serial.println("Brazo robótico by EA");
  pinMode(LED_BUILTIN, OUTPUT); // Debug led
}
// Led blink
int ledState = LOW;             // ledState used to set the LED
unsigned long previousMillis = 0;        // will store last time LED was updated
unsigned long interval = 1000;           // interval at which to blink (milliseconds)

void loopLed(long delay_time){
    interval = delay_time;

    unsigned long currentMillis = millis();

  if (currentMillis - previousMillis >= interval) {
    // save the last time you blinked the LED
    previousMillis = currentMillis;

    // if the LED is off turn it on and vice-versa:
    ledState = !ledState;

    // set the LED with the ledState of the variable:
    digitalWrite(LED_BUILTIN, ledState);
  }
}

#endif