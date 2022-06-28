#ifndef positionManager_h
#define positionManager_h

#include "variables.h"
#include "configuration.h"

#pragma once

int analogInToDegrees (uint8_t);
//int calc_variation (pos last, pos now);
void clearRecorded ();

// Keeps current_position values updated
void updateCurrentPosition () {
    current_position.base = analogInToDegrees(ANALOG_POS_1); 
    current_position.shoulder = analogInToDegrees(ANALOG_POS_2);
    current_position.elbow = analogInToDegrees(ANALOG_POS_3);
    current_position.wrist = analogInToDegrees(ANALOG_POS_4);
}

// Execute on long press
void recordPositions (){    


    // status == RECORDING keep recording
    // add
/*
    recorded_positions[recorder_index].base = current_position.base;
    recorded_positions[recorder_index].shoulder = current_position.shoulder;
    recorded_positions[recorder_index].elbow = current_position.elbow;
    recorded_positions[recorder_index].wrist = current_position.wrist; 

    Serial.print("recorded ["); 
    Serial.print(recorder_index);
    Serial.print("]: ");
    
    Serial.print(recorded_positions[recorder_index].base); 
    Serial.print(","); Serial.print(recorded_positions[recorder_index].shoulder); 
    Serial.print(","); Serial.print(recorded_positions[recorder_index].elbow); 
    Serial.print(","); Serial.println(recorded_positions[recorder_index].wrist); 

    Serial.print("position: "); Serial.print(current_position.base); 
    Serial.print(","); Serial.print(current_position.shoulder); 
    Serial.print(","); Serial.print(current_position.elbow); 
    Serial.print(","); Serial.println(current_position.wrist);

    */
    
}

void clearRecorded () {
    //float sizeofrecorded = sizeof(recorded_positions)/sizeof(pos);

    //Serial.print("sizeof recorded_positions: "); Serial.println(sizeofrecorded);

    for (unsigned int i = 0; i < (sizeof(recorded_positions)/sizeof(pos)); i++){
        recorded_positions[i].base = 0;
        recorded_positions[i].shoulder = 0;
        recorded_positions[i].elbow = 0;
        recorded_positions[i].wrist = 0;
    } 
}

// Transforms analog values (0-1023) to angles (0-180°). Exponential filter
float alpha = 0.5;      // Smoothing factor in range [0,1]. Higher the value - less smoothing 
pos lastOutput = {0, 0, 0, 0, 0};     // Save output for next time use. Internal use only for exponential smoothing

/* FILTRO EXPONENCIAL:
    //int output = alpha * analogRead(analog_input) + (1 - alpha) * lastOutput;
    //lastOutput = output;
*/

int analogInToDegrees (uint8_t analog_input){

    int output = 0;

    switch (analog_input){
    // ANALOG_POS_1 -> GPIO 14 -> base
    case ANALOG_POS_1: 
        output = alpha * analogRead(analog_input) + (1 - alpha) * lastOutput.base;
        lastOutput.base = output; 
        output = map(output, calibration_min.base, calibration_max.base, 0, 180); // map(value, fromLow, fromHigh, toLow, toHigh)
        break;
    case ANALOG_POS_2:
        output = alpha * analogRead(analog_input) + (1 - alpha) * lastOutput.shoulder;
        lastOutput.shoulder = output; 
        output = map(output, calibration_min.shoulder, calibration_max.shoulder, 0, 180); // map(value, fromLow, fromHigh, toLow, toHigh)
        break;
    case ANALOG_POS_3:
        output = alpha * analogRead(analog_input) + (1 - alpha) * lastOutput.elbow;
        lastOutput.elbow = output; 
        output = map(output, calibration_min.elbow, calibration_max.elbow, 0, 180); // map(value, fromLow, fromHigh, toLow, toHigh)
        break;
    case ANALOG_POS_4:
        output = alpha * analogRead(analog_input) + (1 - alpha) * lastOutput.wrist;
        lastOutput.wrist = output; 
        output = map(output, calibration_min.wrist, calibration_max.wrist, 0, 180); // map(value, fromLow, fromHigh, toLow, toHigh)
        break;
    default:
        Serial.println("[Error]: analogInToDegrees() says \"Requested analog_input not recognized\"");
    }

    output = constrain(output, 0, 180);

    return output;
}

#endif