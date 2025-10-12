/*
 * fsm_traffic.c
 *
 *  Created on: Oct 10, 2025
 *      Author: Hwng
 */

#include "fsm_traffic.h"
#include "leds_display.h"

void fsm_traffic_run() {
	switch(systemMode)
	{
	// ---------------- AUTO MODE ----------------
	case INIT:
		if (start_new_loop == 1) {
			start_new_loop = 0;
			setTimer2(duration_buffer_2[2]);

			countDownDur1 = duration_buffer_1[0];
			countDownDur2 = duration_buffer_2[2];
		}

		switch (trafficState)
		{
		case RED1_GREEN2_AUTO:
			setTrafficLights(SET, RESET, RESET, RESET, RESET, SET);
			if (timer2_flag == 1) {
				// We dont need to update countDownDur1 here
				countDownDur2 = duration_buffer_2[1];

				trafficState = RED1_YELLOW2_AUTO;

				setTimer2(duration_buffer_2[1]);
			}
			break;

		case RED1_YELLOW2_AUTO:
			setTrafficLights(SET, RESET, RESET, RESET, SET, RESET);
			if (timer2_flag == 1) {
				countDownDur1 = duration_buffer_1[2];
				countDownDur2 = duration_buffer_2[0];

				trafficState = GREEN1_RED2_AUTO;

				setTimer2(duration_buffer_1[2]);
			}
			break;

		case GREEN1_RED2_AUTO:
			setTrafficLights(RESET, RESET, SET, SET, RESET, RESET);
			if (timer2_flag == 1) {
				countDownDur1 = duration_buffer_1[1];
				// We dont need to update countDownDur2 here

				trafficState = YELLOW1_RED2_AUTO;

				setTimer2(duration_buffer_1[1]);
			}
			break;

		case YELLOW1_RED2_AUTO:
			setTrafficLights(RESET, SET, RESET, SET, RESET, RESET);
			if (timer2_flag == 1) {
				countDownDur1 = duration_buffer_1[0];
				countDownDur2 = duration_buffer_2[2];

				trafficState = RED1_GREEN2_AUTO;

				setTimer2(duration_buffer_2[2]);
			}
			break;

		default:
			break;
		}
		break;

	// ---------------- SET MODES ----------------
	case SET_RED:
		clearTrafficLEDs();

		if(timer0_flag == 1) {
			HAL_GPIO_TogglePin(GPIOB, LED_RED_1_Pin);
			HAL_GPIO_TogglePin(GPIOB, LED_RED_2_Pin);

			setTimer0(250);
		}

		break;

	case SET_YELLOW:
		clearTrafficLEDs();

		if(timer0_flag == 1) {
			HAL_GPIO_TogglePin(GPIOB, LED_YELLOW_1_Pin);
			HAL_GPIO_TogglePin(GPIOB, LED_YELLOW_2_Pin);

			setTimer0(250);
		}

		break;

	case SET_GREEN:
		clearTrafficLEDs();

		if(timer0_flag == 1) {
			HAL_GPIO_TogglePin(GPIOB, LED_GREEN_1_Pin);
			HAL_GPIO_TogglePin(GPIOB, LED_GREEN_2_Pin);

			setTimer0(250);
		}

		break;

	default:
		break;
	}
}
