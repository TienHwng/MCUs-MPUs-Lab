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

				if(button_index == 0) {
					systemMode++;
//					is_first_time = 1;
					main2temp();
				}

				else if(button_index == 1 && systemMode != INIT) {
//					if(is_first_time == 1) {
//						is_first_time = 0;
//						main2temp();
//					}

					duration_buffer_temp[systemMode - 2] += 1000;

					if (duration_buffer_temp[systemMode - 2] > 99000) {
						duration_buffer_temp[systemMode - 2] = 1000;
					}
				}

				else if(button_index == 2) {
					temp2main();

					duration_buffer_2[2] = round(duration_buffer_1[0] * 7 / 10);
					duration_buffer_2[1] = round(duration_buffer_1[0] * 3 / 10);
					duration_buffer_2[0] = duration_buffer_1[1] + duration_buffer_1[2];

					trafficState = RED1_GREEN2_AUTO;
					start_new_loop = 1;
					systemMode = INIT;
				}

				if (systemMode > SET_GREEN ) {
					trafficState = RED1_GREEN2_AUTO;
					start_new_loop = 1;
					systemMode = INIT;
				}
			}
			break;

		case BUTTON_PRESSED:
			if(!is_button_pressed(button_index)) {
				buttonState[button_index] = BUTTON_RELEASED;
			}
			else {
				if(is_button_pressed_1s(button_index)) {
					buttonState[button_index] = BUTTON_PRESSED_MORE_THAN_1_SECOND;
				}
			}
			break;

		case BUTTON_PRESSED_MORE_THAN_1_SECOND:
			if(!is_button_pressed(button_index)) {
				buttonState[button_index] = BUTTON_RELEASED;
			}
			else {
				if(flagForLongPress[button_index] == 1) {
					if(button_index == 0) {
						systemMode++;
						main2temp();

//						is_first_time = 1;
					}
					else if(button_index == 1 && systemMode != INIT) {
//						if(is_first_time == 1) {
//							is_first_time = 0;
//							main2temp();
//						}

						duration_buffer_temp[systemMode - 2] += 1000;

						if (duration_buffer_temp[systemMode - 2] > 99000) {
							duration_buffer_temp[systemMode - 2] = 1000;
						}
					}

					flagForLongPress[button_index] = 0;
				}

				if(systemMode > SET_GREEN) {
					trafficState = RED1_GREEN2_AUTO;
					start_new_loop = 1;
					systemMode = INIT;
				}
			}

			break;

		default:
			break;
		}
	}
}

void temp2main()
{
	duration_buffer_1[0] = duration_buffer_temp[0];
	duration_buffer_1[1] = duration_buffer_temp[1];
	duration_buffer_1[2] = duration_buffer_temp[2];
}

void main2temp()
{
	duration_buffer_temp[0] = duration_buffer_1[0];
	duration_buffer_temp[1] = duration_buffer_1[1];
	duration_buffer_temp[2] = duration_buffer_1[2];
}




















