#ifndef buttonManager_h
#define buttonManager_h

#include <Arduino.h>
#include "configuration.h"
#include "OneButton.h"

#pragma once

OneButton pulsador_rojo(BUTTON, true); // Button between D2 and GND

#define IDLE 0          // "IDLE"
#define PLAYING 1       // "PLAYING"
#define RECORDING 2     // "RECORDING"
#define ADD_POINT 3     // "Only ADD_POINT while adding a point"

int status = IDLE;

void click () {
    /* If status is RECORDING && click -> status = ADD_POINT */ 
    if (status == RECORDING){
        Serial.println("[Button manager]: Single click while recording!");
        status = ADD_POINT;
    }else{
        Serial.println("[Button manager]: Single click!");
    }
}

void doubleclick () {
    status = PLAYING;
    Serial.println("[Button manager]: Double click!");
}

void longPressStop () {

}

void longPressStart () {
    /* Long press enters RECORDING STATUS if status is IDLE */
    /* Long press exits RECORDING STATUS to IDLE */ 

    if (status == RECORDING){
        status = IDLE;
        Serial.println("[Button manager]: Long press! Status: RECORDING -> IDLE");
    } else if (status == IDLE){
        status = RECORDING;
        Serial.println("[Button manager]: Long press! Status: IDLE -> RECORDING");
    } else {
        Serial.println("[Warning]: Unhandled long press event. Last status was: " + String(status));      
    }

}

void beginButton(){
    Serial.println("[Info]: Button pin " + String(BUTTON));

    pulsador_rojo.attachClick(click);
    pulsador_rojo.attachDoubleClick(doubleclick);
    pulsador_rojo.attachLongPressStart(longPressStart);
    pulsador_rojo.attachLongPressStop(longPressStop);
}

void loopButton (){
    pulsador_rojo.tick();
}

#endif