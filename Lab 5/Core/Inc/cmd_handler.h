/*
 * cmd_handler.h
 *
 *  Created on: Nov 8, 2025
 *      Author: Hwng
 */

#ifndef INC_CMD_HANDLER_H_
#define INC_CMD_HANDLER_H_

#include "main.h"

#define ADC_TIMEOUT 		3000 // 3 seconds for timeout duration
#define MAX_BUFFER_SIZE 	30
#define MAX_CMD_LENGTH 		30

// Define the states for command handling
typedef enum {
    CMD_IDLE,	// Waiting for command
    CMD_RST,    // "!RST#"
    CMD_OK      // "!OK#"
} CommandType;

// Define UART FSM states
typedef enum {
    UART_IDLE,
    UART_WAIT_FOR_RST,
    UART_WAIT_FOR_OK,
    UART_TIMEOUT,
    UART_COMM_COMPLETE
} UARTState;

extern ADC_HandleTypeDef hadc1;
extern UART_HandleTypeDef huart2;

extern uint8_t temp;                       // Variable for UART data reception
extern uint8_t buffer[MAX_BUFFER_SIZE];    // Buffer for received data
extern uint8_t index_buffer;               // Index for the received data
extern uint8_t buffer_flag;                // Flag indicating new data in buffer

// Function prototypes
void command_parser_fsm(void);
void uart_communication_fsm(void);
int is_valid_command_char(uint8_t ch);
void send_uart_message(const char *str);

#endif /* INC_CMD_HANDLER_H_ */
