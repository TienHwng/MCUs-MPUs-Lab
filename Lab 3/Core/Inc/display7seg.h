/*
 * display7seg.h
 *
 *  Created on: Oct 10, 2025
 *      Author: Hwng
 */

#ifndef INC_DISPLAY7SEG_H_
#define INC_DISPLAY7SEG_H_

#include "main.h"
#include <stdint.h>

void display7SEG(int num);

void update7SEG(int index);

void updateClockBuffer(int led_duration, int mode);

#endif /* INC_DISPLAY7SEG_H_ */
