/*
 * input_processing.c
 *
 *  Created on: Oct 10, 2025
 *      Author: Hwng
 */

#include "main.h"
#include "input_processing.h"
#include "global.h"

enum ButtonState{BUTTON_RELEASED, BUTTON_PRESSED, BUTTON_PRESSED_MORE_THAN_1_SECOND} ;
enum ButtonState buttonState = BUTTON_RELEASED;

void fsm_for_input_processing(void) {
	switch(buttonState) {
		case BUTTON_RELEASED:
			if(is_button_pressed(0)) {
				buttonState = BUTTON_PRESSED;

				// Tăng mode và quay vòng
				systemMode++;
				if (systemMode > SET_GREEN) {
					systemMode = INIT;
					trafficState = RED1_GREEN2_AUTO;
				}

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
