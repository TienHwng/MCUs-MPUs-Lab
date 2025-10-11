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
enum ButtonState buttonState[N0_OF_BUTTONS] = {BUTTON_RELEASED, BUTTON_RELEASED, BUTTON_RELEASED};

void fsm_for_input_processing(void) {
	for(int button_index = 0; button_index < N0_OF_BUTTONS; button_index++) {
		switch(buttonState[button_index])
		{
		case BUTTON_RELEASED:
			if(is_button_pressed(button_index)) {
				buttonState[button_index] = BUTTON_PRESSED;

				// Tăng mode và quay vòng
				if(button_index == 0) systemMode++;

				else if(button_index == 1) {
					switch(systemMode)
					{
					case SET_RED:
						duration_buffer_1[systemMode] += 1000;
						break;

					case SET_YELLOW:
						duration_buffer_1[systemMode] += 1000;
						break;

					case SET_GREEN:
						duration_buffer_1[systemMode] += 1000;
						break;
					}

					if (duration_buffer_1[systemMode] > 99000) {
						duration_buffer_1[systemMode] = 1000;
					}
				}

				else if(button_index == 2) {
					// button_index == 2, if more button is added, this function has to be fixed

					duration_buffer_2[2] = ceil(duration_buffer_1[0] * 7 / 10);
					duration_buffer_2[1] = ceil(duration_buffer_1[0] * 3 / 10);
					duration_buffer_2[0] = duration_buffer_1[1] + duration_buffer_1[2]; // red nhung ma khong dung den

					systemMode = INIT;
					trafficState = RED1_GREEN2_AUTO;
					start_new_loop = 1;
				}

				if (systemMode > SET_GREEN ) { // thêm điều kiện or ở đây cũng được
					systemMode = INIT;
					trafficState = RED1_GREEN2_AUTO;
					start_new_loop = 1;
				}
			}
			break;

		case BUTTON_PRESSED:
			if(!is_button_pressed(0)) {
				buttonState[button_index] = BUTTON_RELEASED;
			}
			else {
				if(is_button_pressed_1s(0)) {
					buttonState[button_index] = BUTTON_PRESSED_MORE_THAN_1_SECOND;
				}
			}
			break;

		case BUTTON_PRESSED_MORE_THAN_1_SECOND:
			if(!is_button_pressed(0)) {
				buttonState[button_index] = BUTTON_RELEASED;
			}

			//todo


			break;

		default:
			break;
		}
	}
}
