/*
 * tasks.c
 *
 *  Created on: Oct 26, 2025
 *      Author: Hwng
 */

#include "tasks.h"

uint32_t get_time() {
	return systime;
//	return HAL_GetTick();
}

void ledDisplay(int pin_index) {
	if (pin_index < 0 || pin_index >= MAX_LED) return;
	HAL_GPIO_TogglePin(GPIOA, LED_Pins[pin_index]);
}

void oneShotLED(void) { ledDisplay(0); }
void led_500_ms(void) { ledDisplay(1); }
void led_1000_ms(void) { ledDisplay(2); }
void led_1500_ms(void) { ledDisplay(3); }
void led_2000_ms(void) { ledDisplay(4); }
void led_2500_ms(void) { ledDisplay(5); }

void timeout_10ms(void) {
	char msg[50];
	sprintf(msg, "Current timestamp: %lu ms\r\n", get_time());
	HAL_UART_Transmit(&huart1, (uint8_t*)msg, strlen(msg), HAL_MAX_DELAY);

	systime += 10;
}
