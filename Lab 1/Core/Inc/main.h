/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f1xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define A1_Pin GPIO_PIN_1
#define A1_GPIO_Port GPIOA
#define B1_Pin GPIO_PIN_2
#define B1_GPIO_Port GPIOA
#define C1_Pin GPIO_PIN_3
#define C1_GPIO_Port GPIOA
#define D1_Pin GPIO_PIN_4
#define D1_GPIO_Port GPIOA
#define E1_Pin GPIO_PIN_5
#define E1_GPIO_Port GPIOA
#define F1_Pin GPIO_PIN_6
#define F1_GPIO_Port GPIOA
#define G1_Pin GPIO_PIN_7
#define G1_GPIO_Port GPIOA
#define A2_Pin GPIO_PIN_0
#define A2_GPIO_Port GPIOB
#define B2_Pin GPIO_PIN_1
#define B2_GPIO_Port GPIOB
#define C2_Pin GPIO_PIN_2
#define C2_GPIO_Port GPIOB
#define LED_RED_2_Pin GPIO_PIN_10
#define LED_RED_2_GPIO_Port GPIOB
#define LED_YELLOW_2_Pin GPIO_PIN_11
#define LED_YELLOW_2_GPIO_Port GPIOB
#define LED_GREEN_2_Pin GPIO_PIN_12
#define LED_GREEN_2_GPIO_Port GPIOB
#define LED_RED_1_Pin GPIO_PIN_10
#define LED_RED_1_GPIO_Port GPIOA
#define LED_YELLOW_1_Pin GPIO_PIN_11
#define LED_YELLOW_1_GPIO_Port GPIOA
#define LED_GREEN_1_Pin GPIO_PIN_12
#define LED_GREEN_1_GPIO_Port GPIOA
#define D2_Pin GPIO_PIN_3
#define D2_GPIO_Port GPIOB
#define E2_Pin GPIO_PIN_4
#define E2_GPIO_Port GPIOB
#define F2_Pin GPIO_PIN_5
#define F2_GPIO_Port GPIOB
#define G2_Pin GPIO_PIN_6
#define G2_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
