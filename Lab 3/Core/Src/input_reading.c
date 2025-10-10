/*
 * input_processing.c
 *
 *  Created on: Oct 8, 2025
 *      Author: Hwng
 */

#include "main.h"
#include "input_reading.h"
#include "input_processing.h"

void button_reading(void) {
//	for(char i = 0; i < N0_OF_BUTTONS; i ++) {
	for(int i = 0; i < N0_OF_BUTTONS; i ++) {
		debounceButtonBuffer2[i] = debounceButtonBuffer1[i];
		debounceButtonBuffer1[i] = HAL_GPIO_ReadPin(Mode_GPIO_Port, Mode_Pin);

		if(debounceButtonBuffer1[i] == debounceButtonBuffer2[i]) {
			buttonBuffer[i] = debounceButtonBuffer1[i];
		}

		if(buttonBuffer[i] == BUTTON_IS_PRESSED) {
			//if a button is pressed, we start counting
			if(counterForButtonPress1s[i] < DURATION_FOR_AUTO_INCREASING) {
				counterForButtonPress1s[i]++;
			}
			else {
				//the flag is turned on when 1 second has passed
				//since the button is pressed.
				flagForButtonPress1s[i] = 1;
				//todo
			}
		}
		else {
			counterForButtonPress1s[i] = 0;
			flagForButtonPress1s[i] = 0;
		}
	}
}

unsigned char is_button_pressed(uint8_t index) {
	if(index >= N0_OF_BUTTONS) return 0;
	return (buttonBuffer[index] == BUTTON_IS_PRESSED);
}

unsigned char is_button_pressed_1s(unsigned char index) {
	if(index >= N0_OF_BUTTONS) return 0xff;
	return (flagForButtonPress1s[index] == 1);
}
