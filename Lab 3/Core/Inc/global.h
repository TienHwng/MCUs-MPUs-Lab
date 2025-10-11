/*
 * global.h
 *
 *  Created on: Oct 10, 2025
 *      Author: Hwng
 */

#ifndef INC_GLOBAL_H_
#define INC_GLOBAL_H_

#include <stdio.h>
#include <math.h>

#include "software_timer.h"
#include "input_processing.h"
#include "input_reading.h"


// ==== SYSTEM MODES ====
#define INIT       			 	1

#define SET_RED     			2
#define SET_YELLOW  			3
#define SET_GREEN  				4

// ==== TRAFFIC STATES ====
#define RED1_GREEN2_AUTO   		11
#define RED1_YELLOW2_AUTO  		12
#define GREEN1_RED2_AUTO   		13
#define YELLOW1_RED2_AUTO  		14

extern int systemMode;
extern int trafficState;
extern int start_new_loop;

extern int countDownDur1;
extern int countDownDur2;

extern int duration_buffer[N0_OF_MODES];
extern int duration_buffer_1[N0_OF_MODES];
extern int duration_buffer_2[N0_OF_MODES];


#endif /* INC_GLOBAL_H_ */
