/*
 * tasks.h
 *
 *  Created on: Oct 26, 2025
 *      Author: Hwng
 */

#ifndef INC_TASKS_H_
#define INC_TASKS_H_

#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "main.h"

#define MAX_LED			6

extern UART_HandleTypeDef huart1;
extern uint32_t systime;
extern int LED_Pins[MAX_LED];

void timeout_10ms(void);

void oneShotLED(void);
void led_500_ms(void);
void led_1000_ms(void);
void led_1500_ms(void);
void led_2000_ms(void);
void led_2500_ms(void);

uint32_t get_time(void);

void ledDisplay(int pin_index);

#endif /* INC_TASKS_H_ */
