/*
 * fsm_traffic.c
 *
 *  Created on: Oct 10, 2025
 *      Author: Hwng
 */

#include "fsm_traffic.h"

void fsm_traffic_run() {
	switch(systemMode) {
		case INIT:
			HAL_GPIO_WritePin(GPIOB, LED_RED_1_Pin, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(GPIOB, LED_RED_2_Pin, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(GPIOB, LED_YELLOW_1_Pin, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(GPIOB, LED_YELLOW_2_Pin, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(GPIOB, LED_GREEN_1_Pin, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(GPIOB, LED_GREEN_2_Pin, GPIO_PIN_RESET);

			if (is_button_pressed(0)) {
				systemMode = SET_RED;
			}
//			setTimer1(5000);
			break;

		case SET_RED:

			HAL_GPIO_WritePin(GPIOB, LED_YELLOW_1_Pin, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(GPIOB, LED_YELLOW_2_Pin, GPIO_PIN_RESET);

			HAL_GPIO_WritePin(GPIOB, LED_GREEN_1_Pin, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(GPIOB, LED_GREEN_2_Pin, GPIO_PIN_RESET);

			if(timer0_flag == 1) {
				setTimer0(250);

				HAL_GPIO_TogglePin(GPIOB, LED_RED_1_Pin);
				HAL_GPIO_TogglePin(GPIOB, LED_RED_2_Pin);
			}

//			if(timer1_flag == 1) {
//				systemMode = SET_YELLOW;
//				setTimer1(3000);
//			}

			if (is_button_pressed(0)) {
				systemMode = SET_YELLOW;
			}

			break;

		case SET_YELLOW:
			HAL_GPIO_WritePin(GPIOB, LED_RED_1_Pin, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(GPIOB, LED_RED_2_Pin, GPIO_PIN_RESET);

			HAL_GPIO_WritePin(GPIOB, LED_GREEN_1_Pin, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(GPIOB, LED_GREEN_2_Pin, GPIO_PIN_RESET);

			if(timer0_flag == 1) {
				setTimer0(250);

				HAL_GPIO_TogglePin(GPIOB, LED_YELLOW_1_Pin);
				HAL_GPIO_TogglePin(GPIOB, LED_YELLOW_2_Pin);
			}

//			if(timer1_flag == 1) {
//				systemMode = SET_GREEN;
//				setTimer1(5000);
//			}

			if (is_button_pressed(0)) {
				systemMode = SET_GREEN;
			}

			break;

		case SET_GREEN:
			HAL_GPIO_WritePin(GPIOB, LED_RED_1_Pin, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(GPIOB, LED_RED_2_Pin, GPIO_PIN_RESET);

			HAL_GPIO_WritePin(GPIOB, LED_YELLOW_1_Pin, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(GPIOB, LED_YELLOW_2_Pin, GPIO_PIN_RESET);


			if(timer0_flag == 1) {
				setTimer0(250);

				HAL_GPIO_TogglePin(GPIOB, LED_GREEN_1_Pin);
				HAL_GPIO_TogglePin(GPIOB, LED_GREEN_2_Pin);
			}



//			if(timer1_flag == 1) {
//				systemMode = SET_RED;
//				setTimer1(5000);
//			}

			if (is_button_pressed(0)) {
				systemMode = SET_RED;
			}

			break;

		default:
			break;
	}
}
