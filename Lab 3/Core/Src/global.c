/*
 * global.c
 *
 *  Created on: Oct 10, 2025
 *      Author: Hwng
 */

#include "global.h"

int systemMode = 0;
int trafficState = 0;
int start_new_loop = 1;

// Mặc định thời gian (ms)
int red_ms = 5000;
int yellow_ms = 2000;
int green_ms = 3000;

int duration_buffer[N0_OF_MODES] = {5000, 2000, 3000};
int duration_buffer_1[N0_OF_MODES] = {5000, 2000, 3000};
int duration_buffer_2[N0_OF_MODES] = {5000, 2000, 3000};

int countDownDur1;
int countDownDur2;
