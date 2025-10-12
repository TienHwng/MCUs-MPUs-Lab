/*
 * global.c
 *
 *  Created on: Oct 10, 2025
 *      Author: Hwng
 */

#include "global.h"

// ===== Mode =====
int systemMode = 0;
int trafficState = 0;

// ===== Flag =====
int start_new_loop = 1;
int is_first_time = 1;

int duration_buffer_temp[3] 	= {DEFAULT_RED, DEFAULT_YELLOW, DEFAULT_GREEN};
int duration_buffer_1[3] 		= {DEFAULT_RED, DEFAULT_YELLOW, DEFAULT_GREEN};
int duration_buffer_2[3] 		= {DEFAULT_RED, DEFAULT_YELLOW, DEFAULT_GREEN};

//int duration_buffer_temp[3] 	= {5000, 2000, 3000};
//int duration_buffer_1[3] 		= {5000, 2000, 3000};
//int duration_buffer_2[3] 		= {5000, 2000, 3000};

int countDownDur1;
int countDownDur2;
