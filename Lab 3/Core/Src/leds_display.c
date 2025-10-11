/*
 * display7seg.c
 *
 *  Created on: Oct 10, 2025
 *      Author: Hwng
 */

#include <leds_display.h>

int index_led = 0;
int led_buffer[4] = {0, 0, 0, 0};

int segmentMap[10] = {
	0b1111110, // 0
	0b0110000, // 1
	0b1101101, // 2
	0b1111001, // 3
	0b0110011, // 4
	0b1011011, // 5
	0b1011111, // 6
	0b1110000, // 7
	0b1111111, // 8
	0b1111011  // 9
};

int SEG_Pins[7] = {
	SEG0_Pin, SEG1_Pin, SEG2_Pin, SEG3_Pin, SEG4_Pin, SEG5_Pin, SEG6_Pin
};

int EN_Pins[4] = {
	EN0_Pin, EN1_Pin, EN2_Pin, EN3_Pin
};

int all_LEDs[6] = {
	LED_RED_1_Pin, LED_YELLOW_1_Pin, LED_GREEN_1_Pin,
	LED_RED_2_Pin, LED_YELLOW_2_Pin, LED_GREEN_2_Pin
};

void updateModeToBuffer(int mode)
{
	led_buffer[2] = mode / 10;
	led_buffer[3] = mode % 10;
}

void updateDurToBuffer(int led_duration)
{
	led_buffer[0] = led_duration / 10;
	led_buffer[1] = led_duration % 10;
}

void display7SEG(int num)
{
	int bitmask = segmentMap[num];

	for (int i = 0; i < 7; i++)
	{
		HAL_GPIO_WritePin(GPIOA, SEG_Pins[i], (bitmask & (1 << (6 - i))) ? RESET : SET);
	}
}

void update7SEG(int index)
{
	for (int i = 0; i < 4; i++)
	{
		HAL_GPIO_WritePin(GPIOA, EN_Pins[i], GPIO_PIN_SET);
	}

	if (0 <= index && index < MAX_LED) {
		HAL_GPIO_WritePin(GPIOA, EN_Pins[index], GPIO_PIN_RESET);
		display7SEG(led_buffer[index]);
	}
}

void setLEDs(int pin1, int pin2, int pin3, int pin4, int pin5, int pin6)
{
    for (int i = 0; i < 6; i++) {
        HAL_GPIO_WritePin(GPIOB, all_LEDs[i], GPIO_PIN_RESET);
    }

    int pins[] = {pin1, pin2, pin3, pin4, pin5, pin6};
    for (int i = 0; i < 6; i++) {
        if (pins[i] == -1 || pins[i] == 0) break;
        HAL_GPIO_WritePin(GPIOB, pins[i], GPIO_PIN_SET);
    }
}
