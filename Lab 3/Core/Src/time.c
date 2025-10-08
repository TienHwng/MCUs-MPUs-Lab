/*
 * time.c
 *
 *  Created on: Oct 8, 2025
 *      Author: Hwng
 */

#include "input_reading.h"

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	if(htim->Instance == TIM2) {
		button_reading();
	}
}
