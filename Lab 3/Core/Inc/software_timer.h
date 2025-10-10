/*
 * timer.h
 *
 *  Created on: Oct 10, 2025
 *      Author: Hwng
 */

#ifndef INC_SOFTWARE_TIMER_H_
#define INC_SOFTWARE_TIMER_H_

#include "main.h"
#include <stdint.h>

#define TIMER_CYCLE		10

void setTimer0(int duration);
void setTimer1(int duration);
void setTimer2(int duration);
void timerRun(void);

#endif /* INC_SOFTWARE_TIMER_H_ */
