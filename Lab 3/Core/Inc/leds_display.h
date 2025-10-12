/*
 * display7seg.h
 *
 *  Created on: Oct 10, 2025
 *      Author: Hwng
 */

#ifndef INC_LEDS_DISPLAY_H_
#define INC_LEDS_DISPLAY_H_

#include "main.h"
#include "global.h"
#include <stdint.h>

#define MAX_LED 4

extern int index_led;
extern int led_buffer[4];

void display7SEG1(int num);
void display7SEG2(int num);

void update7SEG1(int index);
void update7SEG2(int index);

void updateModeToBuffer(int mode);
void updateSetDuration(int led_duration);
void updateDurToBuffer(int main_dur, int sub_dur);

void setTrafficLights(GPIO_PinState red1, GPIO_PinState yellow1, GPIO_PinState green1,
                      GPIO_PinState red2, GPIO_PinState yellow2, GPIO_PinState green2);
void clearTrafficLEDs();

#endif /* INC_LEDS_DISPLAY_H_ */
