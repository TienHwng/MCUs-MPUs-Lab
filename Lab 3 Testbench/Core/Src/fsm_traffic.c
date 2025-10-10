/*
 * fsm_traffic.c
 *
 *  Created on: Oct 10, 2025
 *      Author: Hwng
 */


#include "fsm_traffic.h"

uint16_t red_timer = 5000, green_timer = 3000, yellow_timer = 2000;
//enum status_2 status_2 = RED1_GREEN2_AUTO;

int status;
int start_new_loop = 1;

void fsm_traffic_run() {
	switch (mode) {
		case INIT:
			if(start_new_loop == 1) {
				start_new_loop = 0;

				status = RED1_GREEN2_AUTO;
				setTimer2(green_timer);
			}

			switch(status) {
				case RED1_GREEN2_AUTO:
					// Đèn đỏ 1 và xanh 2
					HAL_GPIO_WritePin(GPIOB, LED_RED_1_Pin, SET);
					HAL_GPIO_WritePin(GPIOB, LED_GREEN_1_Pin, RESET);
					HAL_GPIO_WritePin(GPIOB, LED_YELLOW_1_Pin, RESET);
					HAL_GPIO_WritePin(GPIOB, LED_RED_2_Pin, RESET);
					HAL_GPIO_WritePin(GPIOB, LED_GREEN_2_Pin, SET);
					HAL_GPIO_WritePin(GPIOB, LED_YELLOW_2_Pin, RESET);

					if (timer2_flag == 1) {
						timer2_flag = 0;
						status = RED1_YELLOW2_AUTO;
						setTimer2(yellow_timer);
					}
					break;

				case RED1_YELLOW2_AUTO:
					// Đèn đỏ 1 và vàng 2
					HAL_GPIO_WritePin(GPIOB, LED_RED_1_Pin, RESET);
					HAL_GPIO_WritePin(GPIOB, LED_GREEN_1_Pin, SET);
					HAL_GPIO_WritePin(GPIOB, LED_YELLOW_1_Pin, SET);
					HAL_GPIO_WritePin(GPIOB, LED_RED_2_Pin, SET);
					HAL_GPIO_WritePin(GPIOB, LED_GREEN_2_Pin, SET);
					HAL_GPIO_WritePin(GPIOB, LED_YELLOW_2_Pin, RESET);

					if (timer2_flag == 1) {
						timer2_flag = 0;
						status = GREEN1_RED2_AUTO;
						setTimer2(green_timer);  // Đặt thời gian đếm cho đèn xanh
					}
					break;

				case GREEN1_RED2_AUTO:
					// Đèn xanh 1 và đỏ 2
					HAL_GPIO_WritePin(GPIOB, LED_RED_1_Pin, SET);
					HAL_GPIO_WritePin(GPIOB, LED_GREEN_1_Pin, RESET);
					HAL_GPIO_WritePin(GPIOB, LED_YELLOW_1_Pin, SET);
					HAL_GPIO_WritePin(GPIOB, LED_RED_2_Pin, RESET);
					HAL_GPIO_WritePin(GPIOB, LED_GREEN_2_Pin, SET);
					HAL_GPIO_WritePin(GPIOB, LED_YELLOW_2_Pin, SET);

					if (timer2_flag == 1) {
						timer2_flag = 0;
						status = YELLOW1_RED2_AUTO;
						setTimer2(yellow_timer);  // 2 giây cho đèn vàng
					}
					break;

				case YELLOW1_RED2_AUTO:
					// Đèn vàng 1 và đỏ 2
					HAL_GPIO_WritePin(GPIOB, LED_RED_1_Pin, SET);
					HAL_GPIO_WritePin(GPIOB, LED_GREEN_1_Pin, SET);
					HAL_GPIO_WritePin(GPIOB, LED_YELLOW_1_Pin, RESET);
					HAL_GPIO_WritePin(GPIOB, LED_RED_2_Pin, RESET);
					HAL_GPIO_WritePin(GPIOB, LED_GREEN_2_Pin, SET);
					HAL_GPIO_WritePin(GPIOB, LED_YELLOW_2_Pin, SET);

					if (timer2_flag == 1) {
						timer2_flag = 0;
						status = RED1_GREEN2_AUTO;
						setTimer2(red_timer);  // 5 giây cho đèn đỏ
					}
					break;
			}

		case SET_RED:

			HAL_GPIO_WritePin(GPIOB, LED_YELLOW_1_Pin, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(GPIOB, LED_YELLOW_2_Pin, GPIO_PIN_RESET);

			HAL_GPIO_WritePin(GPIOB, LED_GREEN_1_Pin, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(GPIOB, LED_GREEN_2_Pin, GPIO_PIN_RESET);

			if(timer0_flag == 1) {
				timer0_flag = 0;

				HAL_GPIO_TogglePin(GPIOB, LED_RED_1_Pin);
				HAL_GPIO_TogglePin(GPIOB, LED_RED_2_Pin);
			}


//			if(is_button_pressed(0) == 1) {
//				status = MAN_YELLOW;
//				setTimer1(3000);
//			}

			break;

		case SET_YELLOW:
			HAL_GPIO_WritePin(GPIOB, LED_RED_1_Pin, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(GPIOB, LED_RED_2_Pin, GPIO_PIN_RESET);

			HAL_GPIO_WritePin(GPIOB, LED_GREEN_1_Pin, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(GPIOB, LED_GREEN_2_Pin, GPIO_PIN_RESET);

			if(timer0_flag == 1) {
				timer0_flag = 0;

				HAL_GPIO_WritePin(GPIOB, LED_YELLOW_1_Pin, GPIO_PIN_SET);
				HAL_GPIO_WritePin(GPIOB, LED_YELLOW_2_Pin, GPIO_PIN_SET);
			}

//			if(is_button_pressed(0) == 1) {
//				status = MAN_GREEN;
//				setTimer1(3000);
//			}

			break;

		case SET_GREEN:
			HAL_GPIO_WritePin(GPIOB, LED_RED_1_Pin, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(GPIOB, LED_RED_2_Pin, GPIO_PIN_RESET);

			HAL_GPIO_WritePin(GPIOB, LED_YELLOW_1_Pin, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(GPIOB, LED_YELLOW_2_Pin, GPIO_PIN_RESET);


			if(timer0_flag == 1) {
				timer0_flag = 0;

				HAL_GPIO_WritePin(GPIOB, LED_GREEN_1_Pin, GPIO_PIN_SET);
				HAL_GPIO_WritePin(GPIOB, LED_GREEN_2_Pin, GPIO_PIN_SET);
			}

//			if(is_button_pressed(0) == 1) {
//				status = MAN_GREEN;
//				setTimer1(3000);
//			}

			break;

		default:
			break;
	}
}

