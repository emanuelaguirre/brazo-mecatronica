#include <Arduino.h>
#include "configuration.h"
#include "calibration.h"
#include "variables.h"
#include "buttonManager.h"
#include "positionManager.h"

void setup() {
  beginConf();
  calibrationRun(false);  // false: use hardcoded calibration. true: actually calibrate
  beginButton();          // Start listening to button status
}

void loop() {

  /* On long press: Start recording loop
   * On short press after long press -> Store position
   * 
  */
  if (status == RECORDING) { // Change status to IDLE to scape RECORDING
    detachServos();
    clearRecorded(); /* clear recorded_positions array */
    //updateCurrentPosition(); // update to pos current_position

    unsigned long recording_started_at = millis();  /* Starting time point */
    unsigned long timestamp = 0L;                   
    recorder_index = 0;                             /* Reset point counter */
    

    while (status == RECORDING || status == ADD_POINT) {
      updateCurrentPosition(); // update to pos current_position

      if (status == ADD_POINT){ /* Single click during RECORDING event */
        timestamp = millis();

        recorded_positions [recorder_index].base = current_position.base;
        recorded_positions [recorder_index].shoulder = current_position.shoulder;
        recorded_positions [recorder_index].elbow = current_position.elbow;
        recorded_positions [recorder_index].wrist = current_position.wrist;

        recorded_positions [recorder_index].recorded_time = timestamp - recording_started_at;

        Serial.println("Single click during RECORDING... ");
        Serial.print("position: "); Serial.print(current_position.base); 
        Serial.print(","); Serial.print(current_position.shoulder); 
        Serial.print(","); Serial.print(current_position.elbow); 
        Serial.print(","); Serial.println(current_position.wrist);

        if (recorder_index >= _recorded_points_amount){
          Serial.println("[Error]: Exiting recorded. Max mem reached: " + String(recorder_index));    
          status = IDLE;
        }else{
          recorder_index++;
          status = RECORDING; /* wait for next button push */ 
        }
      }
      
      loopButton(); // Keep loop for button long press detection 
    }

  } else if (status == PLAYING){
    loopLed(3000);
    Serial.println("PLAYING...");
    attachServos();
 
    int delay_time = 0;
  
    for (int i = 0; i<recorder_index; i++){

      Serial.print(recorded_positions [i].base); Serial.print(","); 
      Serial.print(recorded_positions [i].shoulder); Serial.print(","); 
      Serial.print(recorded_positions [i].elbow); Serial.print(","); 
      Serial.print(recorded_positions [i].wrist); Serial.print(","); 
      Serial.print(recorded_positions [i].recorded_time);
      Serial.println();

      fastMoveTo( recorded_positions [i].base, 
                  recorded_positions [i].shoulder, 
                  recorded_positions [i].elbow, 
                  recorded_positions [i].wrist);


      if (i == 0){
        delay_time = recorded_positions [i].recorded_time;
      } else {
        if (recorded_positions [i].recorded_time - delay_time > 2000){
          delay(2000); 
        }else{
          delay(recorded_positions [i].recorded_time - delay_time);
        }   
      }
    }
    status = IDLE;   

  } else if ((status == IDLE)){
    loopLed(1000);
  } else {
    Serial.println("[Error]: Unhandled status " + String (status));
  }

  loopButton();
}