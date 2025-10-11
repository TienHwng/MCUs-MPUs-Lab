/*
 * input_reading.c
 *
 *  Created on: Oct 10, 2025
 *      Author: Hwng
 */

#include "main.h"
#include "input_reading.h"
#include "global.h"

// The buffer that the final result is stored after debouncing
static GPIO_PinState buttonBuffer[N0_OF_BUTTONS] 			= {BUTTON_IS_RELEASED, BUTTON_IS_RELEASED, BUTTON_IS_RELEASED};

// We define two buffers for debouncing
static GPIO_PinState debounceButtonBuffer1[N0_OF_BUTTONS]	= {BUTTON_IS_RELEASED, BUTTON_IS_RELEASED, BUTTON_IS_RELEASED};
static GPIO_PinState debounceButtonBuffer2[N0_OF_BUTTONS]	= {BUTTON_IS_RELEASED, BUTTON_IS_RELEASED, BUTTON_IS_RELEASED};

// We define a flag for a button pressed more than 1 second.
static uint8_t flagForButtonPress1s[N0_OF_BUTTONS]			= {0, 0, 0};

// We define counter for automatically increasing the value after the button is pressed more than 1 second.
static uint16_t counterForButtonPress1s[N0_OF_BUTTONS]		= {0, 0, 0};

static uint16_t counterAfterLongPress[N0_OF_BUTTONS] 		= {0, 0, 0};

uint8_t flagForLongPress[N0_OF_BUTTONS] 					= {0, 0, 0};


int BUTTON_Pins[N0_OF_BUTTONS] = {
		BUTTON_1_Pin, BUTTON_2_Pin, BUTTON_3_Pin
};

void button_reading(void) {
//	for(char i = 0; i < N0_OF_BUTTONS; i++) {
	for(int i = 0; i < N0_OF_BUTTONS; i++) {
		debounceButtonBuffer2[i] = debounceButtonBuffer1[i];
		debounceButtonBuffer1[i] = HAL_GPIO_ReadPin(GPIOA, BUTTON_Pins[i]);

		if(debounceButtonBuffer1[i] == debounceButtonBuffer2[i]) {
			buttonBuffer[i] = debounceButtonBuffer1[i];
		}

		if(buttonBuffer[i] == BUTTON_IS_PRESSED) {
			// If a button is pressed, we start counting

			if(counterForButtonPress1s[i] < DURATION_FOR_AUTO_INCREASING) {
				counterForButtonPress1s[i] += TIMER_CYCLE;
			}
			else {
				// The flag is turned on when 1 second has passed since the button is pressed.

				flagForButtonPress1s[i] = 1;
				counterAfterLongPress[i] += TIMER_CYCLE;

				if (counterAfterLongPress[i] >= 500) {
					counterAfterLongPress[i] = 0;
					flagForLongPress[i] = 1;
				}
			}
		}
		else {
			flagForButtonPress1s[i] = 0;
			counterForButtonPress1s[i] = 0;

			flagForLongPress[i] = 0;
			counterAfterLongPress[i] = 0;
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
