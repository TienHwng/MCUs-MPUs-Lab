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

#define N0_OF_BUTTONS 				    	1
#define N0_OF_MODES							4

#define DURATION_FOR_AUTO_INCREASING	   100 // ms

#define BUTTON_IS_PRESSED                  GPIO_PIN_RESET
#define BUTTON_IS_RELEASED                 GPIO_PIN_SET

extern int BUTTON_Pins[N0_OF_BUTTONS];

void button_reading(void);
unsigned char is_button_pressed(unsigned char index);
unsigned char is_button_pressed_1s(unsigned char index);

#endif /* INC_INPUT_READING_H_ */
