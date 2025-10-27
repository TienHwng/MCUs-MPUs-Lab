/*
 * scheduler.h
 *
 *  Created on: Oct 26, 2025
 *      Author: Hwng
 */

#ifndef INC_SCHEDULER_H_
#define INC_SCHEDULER_H_

#include <stdint.h>
#include "main.h"
#include "tasks.h"

/* ms per tick */
#define TICK_MS							10

/* Tasks define */
#define SCH_MAX_TASKS 					10
#define NO_TASK_ID 						0

/* Error codes */
#define ERROR_NONE                      0
#define ERROR_SCH_TOO_MANY_TASKS        1
#define ERROR_SCH_CANNOT_DELETE_TASK    2
#define ERROR_WATCHDOG_MISSED_REFRESH 	3
#define ERROR_SCH_WAITING_FOR_SLAVE_TO_ACK 4
#define ERROR_SCH_WAITING_FOR_START_COMMAND_FROM_MASTER 5
#define ERROR_SCH_ONE_OR_MORE_SLAVES_DID_NOT_START 6
#define ERROR_SCH_LOST_SLAVE 7
#define ERROR_SCH_CAN_BUS_ERROR 8
#define ERROR_I2C_WRITE_BYTE_AT24C64 9
#define ERROR_SCH_TASK_ADD_FAIL 10

/* Return codes */
#define RETURN_NORMAL  					0
#define RETURN_ERROR   					1

extern IWDG_HandleTypeDef hiwdg;
extern TIM_HandleTypeDef htim2;

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
void Watchdog_Task(void);

void deleteAllTasks(void);

#endif /* INC_SCHEDULER_H_ */
