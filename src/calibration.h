#ifndef calibration_h
#define calibration_h

#pragma once

#include "variables.h"
#include "configuration.h"

pos calibration_min = {0, 0, 0, 0, 0};
pos calibration_max = {0, 0, 0, 0, 0};

void fastMoveTo(int base_angle, int shoulder_angle, int elbow_angle, int wrist_angle){
    base.write(base_angle);
    elbow.write(elbow_angle);
    shoulder.write(shoulder_angle);
    wrist.write(wrist_angle);
}

/* HOME ALL AXIS: Move each servo in steps to get analog values as reference for angle to analogRead */
void calibrationRun (bool calibrate) { 
  if (calibrate){ 
    attachServos();

    Serial.println("Iniciando calibración...");
    
    /* Calibrate wrist */
    fastMoveTo(0, 50, 0, 0);
    delay(500);

    wrist.write(0); // Move to first position
    delay(1000);
    calibration_min.wrist = analogRead(ANALOG_POS_4); // Record analog value
    delay(500);
    wrist.write(180); // Move to last position position
    delay(1000);
    calibration_max.wrist = analogRead(ANALOG_POS_4); // Record analog value

    Serial.print("calibration_min.wrist = ");  Serial.println(calibration_min.wrist + String(";"));
    Serial.print("calibration_max.wrist = ");  Serial.println(calibration_max.wrist + String(";"));
    Serial.println();

    /* Calibrate elbow */
    fastMoveTo(0, 50, 0, 90);
    delay(500);
    elbow.write(0); // Move to first position
    delay(1000);  
    calibration_min.elbow = analogRead(ANALOG_POS_3); // Record analog value  

    // Slow move to pos
    shoulder.write(100);
    delay(500);

    fastMoveTo(0, 120, 180, 180); // Calibrate elbow position 2
    shoulder.write(120);
    delay(300);
    shoulder.write(130);
    delay(300);

    elbow.write(180); // Move to first position
    delay(2000);  
    calibration_max.elbow = analogRead(ANALOG_POS_3); // Record analog value  

    Serial.print("calibration_min.elbow = ");  Serial.println(calibration_min.elbow + String(";"));
    Serial.print("calibration_max.elbow = ");  Serial.println(calibration_max.elbow + String(";"));
    Serial.println();

    /* Calibrate shoulder */
    fastMoveTo(0, 180, 180, 110);

    shoulder.write(180); // Move to first position
    delay(1000);
    calibration_max.shoulder = analogRead(ANALOG_POS_2); // Record analog value

    fastMoveTo(0, 90, 90, 90);
    fastMoveTo(0, 10, 0, 90);

    shoulder.write(0); // Move to first position
    delay(1000);
    calibration_min.shoulder = analogRead(ANALOG_POS_2); // Record analog value

    Serial.print("calibration_min.shoulder = ");  Serial.println(calibration_min.shoulder + String(";"));
    Serial.print("calibration_max.shoulder = ");  Serial.println(calibration_max.shoulder + String(";"));
    Serial.println();

    /* Calibrate base */
    delay(1000);
    fastMoveTo(0, 50, 0, 45);
    base.write(0);
    calibration_min.base = analogRead(ANALOG_POS_1); // Record analog value
    delay(300);

    //Slowly move the base
    for (int i = 0; i<180; i++){
      base.write(i);  
      delay(20);
    }
    delay(300);
    base.write(180);
    calibration_max.base = analogRead(ANALOG_POS_1); // Record analog value
    
    Serial.print("calibration_min.base = ");  Serial.println(calibration_min.base + String(";")); 
    Serial.print("calibration_max.base = ");  Serial.println(calibration_max.base + String(";"));
    Serial.println();
    Serial.println("Calibración exitosa!");

    detachServos();
  }else{
    Serial.println("Calibración salteada! Usando valores standard");
  
    calibration_min.wrist = 66; 
    calibration_max.wrist = 645;

    calibration_min.elbow = 61; 
    calibration_max.elbow = 644;

    calibration_min.shoulder = 60; 
    calibration_max.shoulder = 644;

    calibration_min.base = 62;
    calibration_max.base = 641;
  }
  
}

#endif

