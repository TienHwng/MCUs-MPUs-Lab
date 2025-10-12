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

int SEG_Pins_1[7] = {
	SEG0_Pin, SEG1_Pin, SEG2_Pin, SEG3_Pin, SEG4_Pin, SEG5_Pin, SEG6_Pin
};

int SEG_Pins_2[7] = {
	SEGa_Pin, SEGb_Pin, SEGc_Pin, SEGd_Pin, SEGe_Pin, SEGf_Pin, SEGg_Pin
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

void updateSetDuration(int led_duration)
{
	led_buffer[0] = led_duration / 10;
	led_buffer[1] = led_duration % 10;
}

void updateDurToBuffer(int main_dur, int sub_dur)
{
	led_buffer[0] = main_dur / 10;
	led_buffer[1] = main_dur % 10;
	led_buffer[2] = sub_dur / 10;
	led_buffer[3] = sub_dur % 10;
}

void display7SEG1(int num)
{
	int bitmask1 = segmentMap[num];

	for (int i = 0; i < 7; i++)
	{
		HAL_GPIO_WritePin(GPIOA, SEG_Pins_1[i], (bitmask1 & (1 << (6 - i))) ? RESET : SET);
	}
}

void display7SEG2(int num)
{
	int bitmask2 = segmentMap[num];

	for (int i = 0; i < 7; i++)
	{
		HAL_GPIO_WritePin(GPIOB, SEG_Pins_2[i], (bitmask2 & (1 << (6 - i))) ? RESET : SET);
	}
}

void update7SEG1(int index)
{
	for (int i = 0; i < 2; i++)
	{
		HAL_GPIO_WritePin(GPIOA, EN_Pins[i], GPIO_PIN_SET);
	}

	if (0 <= index && index < 2) {
		HAL_GPIO_WritePin(GPIOA, EN_Pins[index], GPIO_PIN_RESET);
		display7SEG1(led_buffer[index]);
	}
}

void update7SEG2(int index)
{
	for (int i = 2; i < 4; i++)
	{
		HAL_GPIO_WritePin(GPIOA, EN_Pins[i], GPIO_PIN_SET);
	}

	if (2 <= index && index < 4) {
		HAL_GPIO_WritePin(GPIOA, EN_Pins[index], GPIO_PIN_RESET);
		display7SEG2(led_buffer[index]);
	}
}

// Need to be improved
void setTrafficLights(GPIO_PinState red1, GPIO_PinState yellow1, GPIO_PinState green1,
                      GPIO_PinState red2, GPIO_PinState yellow2, GPIO_PinState green2)
{
    HAL_GPIO_WritePin(GPIOB, LED_RED_1_Pin, red1);
    HAL_GPIO_WritePin(GPIOB, LED_YELLOW_1_Pin, yellow1);
    HAL_GPIO_WritePin(GPIOB, LED_GREEN_1_Pin, green1);

    HAL_GPIO_WritePin(GPIOB, LED_RED_2_Pin, red2);
    HAL_GPIO_WritePin(GPIOB, LED_YELLOW_2_Pin, yellow2);
    HAL_GPIO_WritePin(GPIOB, LED_GREEN_2_Pin, green2);
}

void clearTrafficLEDs() {
	for (int i = 0; i < 6; i++) {
		HAL_GPIO_WritePin(GPIOB, all_LEDs[i], RESET);
	}
}


