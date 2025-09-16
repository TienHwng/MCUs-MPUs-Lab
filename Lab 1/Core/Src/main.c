/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
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
/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

uint8_t segmentMap[10] = {
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

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

void display7SEG1(int num)
{
	uint8_t bitmask1 = segmentMap[num];

	HAL_GPIO_WritePin(GPIOA, A1_Pin, (bitmask1 & (1 << 6)) ? RESET : SET);
	HAL_GPIO_WritePin(GPIOA, B1_Pin, (bitmask1 & (1 << 5)) ? RESET : SET);
	HAL_GPIO_WritePin(GPIOA, C1_Pin, (bitmask1 & (1 << 4)) ? RESET : SET);
	HAL_GPIO_WritePin(GPIOA, D1_Pin, (bitmask1 & (1 << 3)) ? RESET : SET);
	HAL_GPIO_WritePin(GPIOA, E1_Pin, (bitmask1 & (1 << 2)) ? RESET : SET);
	HAL_GPIO_WritePin(GPIOA, F1_Pin, (bitmask1 & (1 << 1)) ? RESET : SET);
	HAL_GPIO_WritePin(GPIOA, G1_Pin, (bitmask1 & (1 << 0)) ? RESET : SET);
}

void display7SEG2(int num)
{
	uint8_t bitmask2 = segmentMap[num];

	HAL_GPIO_WritePin(GPIOB, A2_Pin, (bitmask2 & (1 << 6)) ? RESET : SET);
	HAL_GPIO_WritePin(GPIOB, B2_Pin, (bitmask2 & (1 << 5)) ? RESET : SET);
	HAL_GPIO_WritePin(GPIOB, C2_Pin, (bitmask2 & (1 << 4)) ? RESET : SET);
	HAL_GPIO_WritePin(GPIOB, D2_Pin, (bitmask2 & (1 << 3)) ? RESET : SET);
	HAL_GPIO_WritePin(GPIOB, E2_Pin, (bitmask2 & (1 << 2)) ? RESET : SET);
	HAL_GPIO_WritePin(GPIOB, F2_Pin, (bitmask2 & (1 << 1)) ? RESET : SET);
	HAL_GPIO_WritePin(GPIOB, G2_Pin, (bitmask2 & (1 << 0)) ? RESET : SET);
}

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  /* USER CODE BEGIN 2 */

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */

  int countdown = 10;
  int pedestrianCount = 5;
  int carCount = 3;
  int redLightA = 0, yellowLightA = 0, greenLightA = 0, redLightB = 0, yellowLightB = 0, greenLightB = 0;

  while(1)
  {
      /* USER CODE END WHILE */

      /* USER CODE BEGIN 3 */
      if(countdown == 10)
      {
          redLightA = greenLightB = 1;
          redLightB = yellowLightA = greenLightA = yellowLightB = 0;

          HAL_GPIO_WritePin(GPIOA, LED_RED_1_Pin, RESET);
          HAL_GPIO_WritePin(GPIOA, LED_YELLOW_1_Pin, SET);
          HAL_GPIO_WritePin(GPIOA, LED_GREEN_1_Pin, SET);

          HAL_GPIO_WritePin(GPIOB, LED_RED_2_Pin, SET);
          HAL_GPIO_WritePin(GPIOB, LED_YELLOW_2_Pin, SET);
          HAL_GPIO_WritePin(GPIOB, LED_GREEN_2_Pin, RESET);
      }
      else if(countdown == 7)
      {
          yellowLightB = 1;
          redLightB = greenLightB = 0;

          HAL_GPIO_WritePin(GPIOB, LED_RED_2_Pin, SET);
          HAL_GPIO_WritePin(GPIOB, LED_YELLOW_2_Pin, RESET);
          HAL_GPIO_WritePin(GPIOB, LED_GREEN_2_Pin, SET);
      }
      else if(countdown == 5)
      {
          greenLightA = redLightB = 1;
          redLightA = yellowLightA = yellowLightB = greenLightB = 0;

          HAL_GPIO_WritePin(GPIOA, LED_YELLOW_1_Pin, SET);
          HAL_GPIO_WritePin(GPIOA, LED_RED_1_Pin, SET);
          HAL_GPIO_WritePin(GPIOA, LED_GREEN_1_Pin, RESET);

          HAL_GPIO_WritePin(GPIOB, LED_YELLOW_2_Pin, SET);
          HAL_GPIO_WritePin(GPIOB, LED_RED_2_Pin, RESET);
          HAL_GPIO_WritePin(GPIOB, LED_GREEN_2_Pin, SET);
      }
      else if(countdown == 2)
      {
          yellowLightA = 1;
          redLightA = greenLightA = 0;

          HAL_GPIO_WritePin(GPIOA, LED_YELLOW_1_Pin, RESET);
          HAL_GPIO_WritePin(GPIOA, LED_RED_1_Pin, SET);
          HAL_GPIO_WritePin(GPIOA, LED_GREEN_1_Pin, SET);
      }

      countdown--;
      display7SEG1(pedestrianCount--);
      display7SEG2(carCount--);

      if(pedestrianCount == 0)
      {
          if(redLightA) pedestrianCount = 3;
          if(greenLightA) pedestrianCount = 2;
          if(yellowLightA) pedestrianCount = 5;
      }

      if(carCount == 0)
      {
          if(redLightB) carCount = 3;
          if(greenLightB) carCount = 2;
          if(yellowLightB) carCount = 5;
      }
      if(countdown == 0) countdown = 10;

      HAL_Delay(1000);
  }

  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  /* USER CODE BEGIN MX_GPIO_Init_1 */

  /* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, A1_Pin|B1_Pin|C1_Pin|D1_Pin
                          |E1_Pin|F1_Pin|G1_Pin|LED_RED_1_Pin
                          |LED_YELLOW_1_Pin|LED_GREEN_1_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, A2_Pin|B2_Pin|C2_Pin|LED_RED_2_Pin
                          |LED_YELLOW_2_Pin|LED_GREEN_2_Pin|D2_Pin|E2_Pin
                          |F2_Pin|G2_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pins : A1_Pin B1_Pin C1_Pin D1_Pin
                           E1_Pin F1_Pin G1_Pin LED_RED_1_Pin
                           LED_YELLOW_1_Pin LED_GREEN_1_Pin */
  GPIO_InitStruct.Pin = A1_Pin|B1_Pin|C1_Pin|D1_Pin
                          |E1_Pin|F1_Pin|G1_Pin|LED_RED_1_Pin
                          |LED_YELLOW_1_Pin|LED_GREEN_1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : A2_Pin B2_Pin C2_Pin LED_RED_2_Pin
                           LED_YELLOW_2_Pin LED_GREEN_2_Pin D2_Pin E2_Pin
                           F2_Pin G2_Pin */
  GPIO_InitStruct.Pin = A2_Pin|B2_Pin|C2_Pin|LED_RED_2_Pin
                          |LED_YELLOW_2_Pin|LED_GREEN_2_Pin|D2_Pin|E2_Pin
                          |F2_Pin|G2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /* USER CODE BEGIN MX_GPIO_Init_2 */

  /* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}
#ifdef USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
