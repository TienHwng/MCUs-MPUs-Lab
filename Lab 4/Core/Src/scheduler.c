/*
 * scheduler.c
 *
 *  Created on: Oct 26, 2025
 *      Author: Hwng
 */

#include "scheduler.h"

sTask SCH_tasks_G[SCH_MAX_TASKS];
unsigned char Error_code_G = ERROR_NONE;
unsigned char Last_error_code_G = ERROR_NONE;
static uint32_t counter_for_watchdog = 0;
static int task_counter = 0;

void SCH_Init(void) {
	deleteAllTasks();
	// Reset the global error variable
	// - SCH_Delete_Task() will generate an error code
	// (because the task array is empty)
	Error_code_G = ERROR_NONE;
	Last_error_code_G = ERROR_NONE;
	Timer_init();
	Watchdog_init();
}

/*-------------------------------------------------------------------------*-
SCH_Add_Task() Causes a task (function) to be executed at regular intervals
or after a user-defined delay
-*-------------------------------------------------------------------------*/

unsigned char SCH_Add_Task(void (*pFunction)(), unsigned int DELAY, unsigned int PERIOD) {
	unsigned char Index = 0;
	// First find a gap in the array (if there is one)
	while ((SCH_tasks_G[Index].pTask != NULL) && (Index < SCH_MAX_TASKS)) {
		Index++;
	}
	// Have we reached the end of the lists?
	if (Index == SCH_MAX_TASKS) {
		// Task list is full
		// Set the global error variable
		Error_code_G = ERROR_SCH_TOO_MANY_TASKS;
		// Also return an error code


		char msg[100];
		sprintf(msg, "Task add fail. Too many tasks! \r\nError code: %d \r\n", ERROR_SCH_TOO_MANY_TASKS);
		HAL_UART_Transmit(&huart1, (uint8_t*)msg, strlen(msg), HAL_MAX_DELAY);

		return SCH_MAX_TASKS;
	}

	// Case < 0
	if ((int)DELAY < 0 || (int)PERIOD < 0) {
		Error_code_G = ERROR_SCH_TASK_ADD_FAIL;

		char msg1[100];
		sprintf(msg1, "Task add fail. Period or Delay is negative! \r\nError code: %d \r\n", ERROR_SCH_TASK_ADD_FAIL);
		HAL_UART_Transmit(&huart1, (uint8_t*)msg1, strlen(msg1), HAL_MAX_DELAY);

		return RETURN_ERROR;
	}

	// If we're here, there is a space in the task array
	SCH_tasks_G[Index].pTask = pFunction;
	SCH_tasks_G[Index].Delay = DELAY / TICK_MS; // <- 10 is the tick rate
	SCH_tasks_G[Index].Period = PERIOD / TICK_MS; // <- 10 is the tick rate
	SCH_tasks_G[Index].RunMe = 0;
	SCH_tasks_G[Index].TaskID = task_counter++;
	// return position of the task (to allow later deletion)
	return Index;
}

void SCH_Update(void) {
	unsigned char Index;

	Watchdog_Counting();
	// NOTE: calculations are in *TICKS* (not milliseconds)
	for (Index = 0; Index < SCH_MAX_TASKS; Index++) {
		if (SCH_tasks_G[Index].pTask != NULL) {
			if (SCH_tasks_G[Index].Delay == 0) {
				// The task is due to run
				// Inc. the 'RunMe' flag
				SCH_tasks_G[Index].RunMe += 1;
				if (SCH_tasks_G[Index].Period) {
					// Schedule periodic tasks to run again
					SCH_tasks_G[Index].Delay = SCH_tasks_G[Index].Period - 1;

					// debug
//					char msg[50];
//					sprintf(msg, "Delay: %lu tick\r\n", SCH_tasks_G[Index].Delay);
//					HAL_UART_Transmit(&huart1, (uint8_t*)msg, strlen(msg), HAL_MAX_DELAY);
				}
			} else {
				// Not yet ready to run: just decrement the delay
				SCH_tasks_G[Index].Delay -= 1;
			}
		}
	}
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

			// in timestamp khi task hoàn thành
			if (SCH_tasks_G[Index].pTask != timeout_10ms && SCH_tasks_G[Index].pTask != Watchdog_Task) {
				char msg[60];
				sprintf(msg, "[%lu ms] Task %u finished\r\n", get_time(), Index);
				HAL_UART_Transmit(&huart1, (uint8_t*)msg, strlen(msg), HAL_MAX_DELAY);
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
	if (SCH_tasks_G[TASK_INDEX].pTask == NULL) {
		// Np task at this location
		//
		// Set the global error variable
		Error_code_G = ERROR_SCH_CANNOT_DELETE_TASK;
		// ...also return an error code
		Return_code = RETURN_ERROR;
	} else {
		Return_code = RETURN_NORMAL;
	}
	SCH_tasks_G[TASK_INDEX].pTask = NULL;
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

void Watchdog_init(void){
//	MX_IWDG_Init();
	Reset_Watchdog_Counting();
}

void Watchdog_Counting(void){
	counter_for_watchdog++;
}

void Watchdog_Refresh(void){
	HAL_IWDG_Refresh(&hiwdg);
}

unsigned char Is_Watchdog_Reset(void){
	if (counter_for_watchdog > 300){
		return 1;
	}
	return 0;
}

void Reset_Watchdog_Counting(void){
	counter_for_watchdog = 0;
}

void Watchdog_Task(void){
	Watchdog_Refresh();
}

void deleteAllTasks() {
	for (int i = 0; i < SCH_MAX_TASKS; i++) {
		SCH_Delete_Task(i);
	}
}
