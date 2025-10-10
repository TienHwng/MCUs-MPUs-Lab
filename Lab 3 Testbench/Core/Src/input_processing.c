/*
 * input_reading.c
 *
 *  Created on: Oct 8, 2025
 *      Author: Hwng
 */

#include "main.h"
#include "global.h"
#include "input_processing.h"
#include "input_reading.h"
#include "display7seg.h"

enum ButtonState{BUTTON_RELEASED, BUTTON_PRESSED, BUTTON_PRESSED_MORE_THAN_1_SECOND} ;
enum ButtonState buttonState = BUTTON_RELEASED;

void fsm_for_input_processing(void) {
	switch(buttonState) {
		case BUTTON_RELEASED:
			if(is_button_pressed(0)) {
				buttonState = BUTTON_PRESSED;
				//INCREASE VALUE OF PORT A BY ONE UNIT
				mode++;
			}
			break;

		case BUTTON_PRESSED:
			if(!is_button_pressed(0)) {
				buttonState = BUTTON_RELEASED;
			}
			else {
				if(is_button_pressed_1s(0)) {
					buttonState = BUTTON_PRESSED_MORE_THAN_1_SECOND;
				}
			}
			break;

		case BUTTON_PRESSED_MORE_THAN_1_SECOND:
			if(!is_button_pressed(0)) {
				buttonState = BUTTON_RELEASED;
			}
			//todo
			break;
		default:
			break;
	}
}
