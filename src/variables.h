#ifndef variables_h
#define variables_h

#pragma once

// Global variables
struct pos{
  int base;     // Valores entre 0-1024
  int shoulder;
  int elbow;
  int wrist;
  unsigned long recorded_time;
};

pos current_position = {0, 0, 0, 0, 0}; // 
pos last_position = {0, 0, 0, 0, 0}; // 

//bool already_called_recordPositions = false;
int recorder_index = 0;
pos recorded_positions [_recorded_points_amount];
// [_recorded_points_amount];


#endif