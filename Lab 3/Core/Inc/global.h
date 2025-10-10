/*
 * global.h
 *
 *  Created on: Oct 10, 2025
 *      Author: Hwng
 */

#ifndef INC_GLOBAL_H_
#define INC_GLOBAL_H_

#include "software_timer.h"
#include "input_processing.h"
#include "input_reading.h"

#define INIT					1

#define SET_RED					2
#define SET_YELLOW				3
#define SET_GREEN				4

#define RED1_GREEN2_AUTO		12
#define RED1_YELLOW2_AUTO		13
#define GREEN1_RED2_AUTO		14
#define YELLOW1_RED2_AUTO		15

extern int systemMode;
extern int normalTrafficStatus;

#endif /* INC_GLOBAL_H_ */
