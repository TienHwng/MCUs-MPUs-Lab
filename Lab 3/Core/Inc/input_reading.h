/*
 * input_reading.h
 *
 *  Created on: Oct 10, 2025
 *      Author: Hwng
 */

#ifndef INC_INPUT_READING_H_
#define INC_INPUT_READING_H_

#include "main.h"
#include "input_processing.h"

extern int BUTTON_Pins[N0_OF_BUTTONS];

void button_reading(void);
unsigned char is_button_pressed(unsigned char index);
unsigned char is_button_pressed_1s(unsigned char index);

#endif /* INC_INPUT_READING_H_ */
