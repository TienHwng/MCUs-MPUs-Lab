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
#include <stdio.h>
#include <string.h>
#include "scheduler.h"
#include "tasks.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
typedef unsigned char tByte;

typedef struct {
	// Pointer to the task (must be a 'void (void)' function)
	void (*pTask)(void);
	// Delay (ticks) until the function will (next) be run
	uint32_t Delay;
	// Interval (ticks) between subsequent runs
	uint32_t Period;
	// Increment (by scheduler) when task is due to execute
	uint8_t RunMe;
	// This is a hint to solve the question below
	uint32_t TaskID;
} sTask;

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define SCH_MAX_TASKS	40
#define NO_TASK_ID 		0

/* Error codes */
#define ERROR_NONE                       0
#define ERROR_SCH_TOO_MANY_TASKS         1
#define ERROR_SCH_CANNOT_DELETE_TASK     2
#define ERROR_WATCHDOG_MISSED_REFRESH    3

/* Return codes */
#define RETURN_NORMAL  	0
#define RETURN_ERROR   	1

sTask SCH_tasks_G[SCH_MAX_TASKS];
unsigned char Error_code_G = ERROR_NONE;
unsigned char Last_error_code_G = ERROR_NONE;

#define MAX_LED			5
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
TIM_HandleTypeDef htim2;

UART_HandleTypeDef huart1;

/* USER CODE BEGIN PV */
int systime;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_TIM2_Init(void);
static void MX_USART1_UART_Init(void);
/* USER CODE BEGIN PFP */
void SCH_Init(void);
void SCH_Update(void);
void SCH_Dispatch_Tasks(void);
unsigned char SCH_Add_Task(void (*pFunction)(), unsigned int DELAY, unsigned int PERIOD);
unsigned char SCH_Delete_Task(const tByte TASK_INDEX);
void SCH_Report_Status(void);
void SCH_Go_To_Sleep(void);
void Timer_init(void);
void Watchdog_init(void);
void Watchdog_Counting(void);
void Reset_Watchdog_Counting(void);
//void System_Initialization(void);
uint32_t get_time(void);
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
void SCH_Init(void) {
	unsigned char i;
	for (i = 0; i < SCH_MAX_TASKS; i++) {
		SCH_Delete_Task(i);
	}
	// Reset the global error variable
	// - SCH_Delete_Task() will generate an error code
	// (because the task array is empty)
	Error_code_G = ERROR_NONE;
	Last_error_code_G = ERROR_NONE;
	Timer_init();
//	Watchdog_init();
}

void SCH_Update(void) {
	unsigned char Index;

//	Watchdog_Counting();
	// NOTE: calculations are in *TICKS* (not milliseconds)
	for (Index = 0; Index < SCH_MAX_TASKS; Index++) {
		if (SCH_tasks_G[Index].pTask) {
			if (SCH_tasks_G[Index].Delay == 0) {
				// The task is due to run
				// Inc. the 'RunMe' flag
				SCH_tasks_G[Index].RunMe += 1;
				if (SCH_tasks_G[Index].Period) {
					// Schedule periodic tasks to run again
					SCH_tasks_G[Index].Delay = SCH_tasks_G[Index].Period;
				}
			} else {
				// Not yet ready to run: just decrement the delay
				SCH_tasks_G[Index].Delay -= 1;
			}
		}
	}
}

/*-------------------------------------------------------------------------*-
SCH_Add_Task() Causes a task (function) to be executed at regular intervals
or after a user-defined delay
-*-------------------------------------------------------------------------*/

unsigned char SCH_Add_Task(void (*pFunction)(), unsigned int DELAY, unsigned int PERIOD) {
	unsigned char Index = 0;
	// First find a gap in the array (if there is one)
	while ((SCH_tasks_G[Index].pTask != 0) && (Index < SCH_MAX_TASKS)) {
		Index++;
	}
	// Have we reached the end of the lists?
	if (Index == SCH_MAX_TASKS) {
		// Task list is full
		// Set the global error variable
		Error_code_G = ERROR_SCH_TOO_MANY_TASKS;
		// Also return an error code
		return SCH_MAX_TASKS;
	}
	// If we're here, there is a space in the task array
	SCH_tasks_G[Index].pTask = pFunction;
	SCH_tasks_G[Index].Delay = DELAY / 10; // <- 10 is the tick rate
	SCH_tasks_G[Index].Period = PERIOD / 10; // <- 10 is the rick rate
	SCH_tasks_G[Index].RunMe = 0;
	SCH_tasks_G[Index].TaskID = Index;
	// return position of the task (to allow later deletion)
	return Index;
}

void SCH_Dispatch_Tasks(void) {
	unsigned char Index;
	// Dispatches (runs) the next task (if one is ready)
	for (Index = 0; Index < SCH_MAX_TASKS; Index++) {
		if (SCH_tasks_G[Index].RunMe > 0) {
			(*SCH_tasks_G[Index].pTask)(); // Run the task
			SCH_tasks_G[Index].RunMe -= 1; // Reset / reduce RunMe flag
			// Periodic tasks will automatically run again
			// - if this is a 'one shot' task, remove it from the array
			if (SCH_tasks_G[Index].Period == 0) {
				SCH_Delete_Task(Index);
			}
		}
	}
	// Report system status
	SCH_Report_Status();
	// The scheduler enters idle mode at this point
	SCH_Go_To_Sleep();
}

unsigned char SCH_Delete_Task(const tByte TASK_INDEX) {
	unsigned char Return_code;
	if (TASK_INDEX >= SCH_MAX_TASKS) {
		Error_code_G = ERROR_SCH_CANNOT_DELETE_TASK;
		return RETURN_ERROR;
	}
	if (SCH_tasks_G[TASK_INDEX].pTask == 0) {
		// Np task at this location
		//
		// Set the global error variable
		Error_code_G = ERROR_SCH_CANNOT_DELETE_TASK;
		// ...also return an error code
		Return_code = RETURN_ERROR;
	} else {
		Return_code = RETURN_NORMAL;
	}
	SCH_tasks_G[TASK_INDEX].pTask = 0x0000;
	SCH_tasks_G[TASK_INDEX].Delay = 0;
	SCH_tasks_G[TASK_INDEX].Period = 0;
	SCH_tasks_G[TASK_INDEX].RunMe = 0;
	return Return_code;
}

void SCH_Go_To_Sleep() {
	// todo: Optional
}

void SCH_Report_Status(void){

#ifdef SCH_REPORT_ERRORS
	// ONLY APPLIES IF WE ARE REPORTING ERRORS
	// Check for a new error code
	unsigned char Error_port = 0;
	uint32_t Error_tick_count_G = 0;
	if (Error_code_G != Last_error_code_G){
		// Negative logic on LEDs assumed
		Error_port = 255 - Error_code_G;
		Last_error_code_G = Error_code_G;
		if (Error_code_G != 0){
			Error_tick_count_G = 60000;
		} else {
			if (Error_tick_count_G != 0){
				if (--Error_tick_count_G == 0){
					Error_code_G = 0; // Reset error code
				}
			}
		}
	}

#endif
}

void Timer_init(void) {
	HAL_TIM_Base_Start_IT(&htim2);
}

//void Watchdog_init(void){
//	MX_IWDG_Init();
//	Reset_Watchdog_Counting();
//}
//
//void Watchdog_Counting(void){
//	counter_for_watchdog++;
//}
//
//void Watchdog_Refresh(void){
//	HAL_IWDG_Refresh(&hiwdg);
//}
//
//unsigned char Is_Watchdog_Reset(void){
//	if (counter_for_watchdog > 300){
//		return 1;
//	}
//	return 0;
//}
//
//void Reset_Watchdog_Counting(void){
//	counter_for_watchdog = 0;
//}
//
//void Watchdog_Task(void){
//	Watchdog_Refresh();
//}

uint32_t get_time() {
	return systime;
//	return HAL_GetTick();
}

int led_pin[MAX_LED] = { GPIO_PIN_1, GPIO_PIN_2, GPIO_PIN_3, GPIO_PIN_4, GPIO_PIN_5};

void ledDisplay(int pin){
	if (pin < 0 || pin >= MAX_LED) return;
	HAL_GPIO_TogglePin(GPIOA, led_pin[pin]);
}

void ledDisplay_0(void) { ledDisplay(0); }
void ledDisplay_1(void) { ledDisplay(1); }
void ledDisplay_2(void) { ledDisplay(2); }
void ledDisplay_3(void) { ledDisplay(3); }
void ledDisplay_4(void) { ledDisplay(4); }
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

//  	SCH_Add_Task(Watchdog_Task, 0, 100);

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_TIM2_Init();
  MX_USART1_UART_Init();
  /* USER CODE BEGIN 2 */

//  	HAL_TIM_Base_Start_IT(&htim2);

  	SCH_Init(); // <- Timer Interupt start in here

  	SCH_Add_Task(ledDisplay_0, 0, 500);
  	SCH_Add_Task(ledDisplay_1, 0, 1000);
  	SCH_Add_Task(ledDisplay_2, 0, 1500);
  	SCH_Add_Task(ledDisplay_3, 0, 2000);
  	SCH_Add_Task(ledDisplay_4, 0, 2500);

//  	char *test = "UART test OK\r\n";

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
	  SCH_Dispatch_Tasks();
//  	  HAL_UART_Transmit(&huart1, (uint8_t*)test, strlen(test), 100);
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
  * @brief TIM2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM2_Init(void)
{

  /* USER CODE BEGIN TIM2_Init 0 */

  /* USER CODE END TIM2_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM2_Init 1 */

  /* USER CODE END TIM2_Init 1 */
  htim2.Instance = TIM2;
  htim2.Init.Prescaler = 7999;
  htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim2.Init.Period = 9;
  htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim2) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim2, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim2, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM2_Init 2 */

  /* USER CODE END TIM2_Init 2 */

}

/**
  * @brief USART1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART1_UART_Init(void)
{

  /* USER CODE BEGIN USART1_Init 0 */

  /* USER CODE END USART1_Init 0 */

  /* USER CODE BEGIN USART1_Init 1 */

  /* USER CODE END USART1_Init 1 */
  huart1.Instance = USART1;
  huart1.Init.BaudRate = 115200;
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits = UART_STOPBITS_1;
  huart1.Init.Parity = UART_PARITY_NONE;
  huart1.Init.Mode = UART_MODE_TX_RX;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart1.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART1_Init 2 */

  /* USER CODE END USART1_Init 2 */

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

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3|GPIO_PIN_4
                          |GPIO_PIN_5|GPIO_PIN_6, GPIO_PIN_RESET);

  /*Configure GPIO pins : PA1 PA2 PA3 PA4
                           PA5 PA6 */
  GPIO_InitStruct.Pin = GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3|GPIO_PIN_4
                          |GPIO_PIN_5|GPIO_PIN_6;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /* USER CODE BEGIN MX_GPIO_Init_2 */

  /* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {
	if (htim->Instance == TIM2){
		SCH_Update();

		systime += 10;

		char msg[50];
		sprintf(msg, "System Time: %lu ms\r\n", get_time());
		HAL_UART_Transmit(&huart1, (uint8_t*)msg, strlen(msg), 10);
	}
}
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
