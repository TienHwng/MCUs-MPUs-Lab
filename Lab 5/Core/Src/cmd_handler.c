/*
 * cmd_handler.c
 *
 *  Created on: Nov 8, 2025
 *      Author: Hwng
 */

#include "cmd_handler.h"
#include <string.h>
#include <stdio.h>

// Initialize local variables

static CommandType command_status = CMD_IDLE;    	// Current command flag
static UARTState uart_state = UART_IDLE;       		// UART state machine state

static uint32_t timeout_counter = 0;          	    // Timeout counter
static uint32_t ADC_value = 0;                 		// ADC value
static uint32_t last_ADC_value = 0;            		// Last ADC value (for retransmission)

// Check if the character is valid for the command
int is_valid_command_char(uint8_t ch) {
    return ((ch >= 'A' && ch <= 'Z') || (ch >= 'a' && ch <= 'z') ||
            (ch >= '0' && ch <= '9') || ch == '#');
}

// Send a string over UART
void send_uart_message(const char *str) {
    HAL_UART_Transmit(&huart2, (uint8_t*)str, strlen(str), 100);
}

// Command parsing FSM
void command_parser_fsm(void) {
    static uint8_t command_buffer[MAX_CMD_LENGTH];  // Command buffer
    static uint8_t command_index = 0;
    static uint8_t is_in_command = 0;  // Indicates if we are in a valid command

    if (index_buffer == 0 || !buffer_flag) return;

    for (int i = 0; i < index_buffer; i++) {
        uint8_t received_char = buffer[i];  // The received character

        if (received_char == '\r' || received_char == '\n' || received_char == ' ') continue;  // Ignore unnecessary characters

        if (!is_in_command) {
            if (received_char == '!') {
                is_in_command = 1;
                command_index = 0;
                command_buffer[command_index++] = received_char;
            }
        }
        else {
            // Handle backspace (0x08 or 0x7F)
            if (received_char == 0x08 || received_char == 0x7F) {
                if (command_index > 1) {  // Keep '!' at the beginning
                    command_index--;  // Move the index backward
                }

                // Debug: Print command_buffer and command_index after backspace
//                char debug_str[100];
//                sprintf(debug_str, "BACKSPACE: command_buffer = '%.*s', command_index = %d\r\n", command_index, command_buffer, command_index);
//                send_uart_message(debug_str);  // Send the whole debug string in one go

                continue;  // Skip further processing for backspace
            }

            // Accept only valid characters
            if (is_valid_command_char(received_char)) {

                if (command_index >= MAX_CMD_LENGTH - 1) {
                    // If buffer is full, reset and start over
                    is_in_command = 0;
                    command_index = 0;
                    continue;
                }
                command_buffer[command_index++] = received_char;

                // Debug: Print command_buffer and command_index after adding a character
//                char debug_str[100];
//                sprintf(debug_str, " - CHAR ADDED: command_buffer = '%.*s', command_index = %d\r\n", command_index, command_buffer, command_index);
//                send_uart_message(debug_str);  // Send the whole debug string in one go

                // If the command ends with '#', check it
                if (received_char == '#') {
                    command_buffer[command_index] = '\0';  // Null-terminate the command

                    // Check the command validity
                    if (command_index == 5 && strcmp((char*)command_buffer, "!RST#") == 0) {
                        command_status = CMD_RST;  // ADC request
                    }
                    else if (command_index == 4 && strcmp((char*)command_buffer, "!OK#") == 0) {
                        command_status = CMD_OK;  // End communication
                    }

                    // Reset for the next command
                    is_in_command = 0;
                    command_index = 0;
                    break;  // Break out of the loop once a valid command is processed
                }
            }
            else {
                // Handle invalid characters (if required)
                command_buffer[command_index++] = received_char;
                continue;
            }
        }
    }

    // Reset buffer flag and buffer after processing
    index_buffer = 0;
    buffer_flag = 0;  // Reset the buffer flag
    memset(buffer, 0, MAX_BUFFER_SIZE);  // Clear the buffer
}

// UART communication FSM
void uart_communication_fsm(void) {
    char transmission_buffer[50];  // Buffer for UART transmission
    uint32_t current_tick = HAL_GetTick();

    switch (uart_state) {
        case UART_IDLE:
            send_uart_message("=== System READY ===\r\n");
            send_uart_message("Type !RST# to request ADC value\r\n");
            uart_state = UART_WAIT_FOR_RST;
            break;

        case UART_WAIT_FOR_RST:
            if (command_status == CMD_OK) {
                send_uart_message("\r\n=== Communication COMPLETED! ===\r\n");
                command_status = CMD_IDLE;
                uart_state = UART_WAIT_FOR_RST;
                HAL_GPIO_WritePin(LED_AQUA_GPIO_Port, LED_AQUA_Pin, GPIO_PIN_RESET);
            }
            else if (command_status == CMD_RST) {
                send_uart_message("\r\nProcessing !RST# command...\r\n");

                command_status = CMD_IDLE;

                if (HAL_ADC_PollForConversion(&hadc1, 1000) == HAL_OK) {
                    ADC_value = HAL_ADC_GetValue(&hadc1);
                    last_ADC_value = ADC_value;
                }
                else {
                    send_uart_message("ADC read failed!\r\n");
                    ADC_value = 0;
                }

                // Send ADC value
                sprintf(transmission_buffer, "!ADC=%lu#", ADC_value);
                send_uart_message(transmission_buffer);
                send_uart_message("\r\nType !OK# to stop or !RST# to update new ADC value\r\n");

                uart_state = UART_WAIT_FOR_OK;
                timeout_counter = current_tick;

                HAL_GPIO_WritePin(LED_AQUA_GPIO_Port, LED_AQUA_Pin, GPIO_PIN_RESET);
            }
            break;

        case UART_WAIT_FOR_OK:
            if (command_status == CMD_RST) {
                send_uart_message("\r\nNew !RST# - Updating new ADC value\r\n");
                command_status = CMD_RST;
                uart_state = UART_WAIT_FOR_RST;
            }
            else if (command_status == CMD_OK) {
                send_uart_message("\r\n=== Communication COMPLETED! ===\r\n");
                command_status = CMD_IDLE;
                uart_state = UART_WAIT_FOR_RST;
                HAL_GPIO_WritePin(LED_AQUA_GPIO_Port, LED_AQUA_Pin, GPIO_PIN_SET);
            }
            else if ((current_tick - timeout_counter) >= ADC_TIMEOUT) {
                send_uart_message("\r\n=== TIMEOUT - Retransmitting... ===\r\n");

                sprintf(transmission_buffer, "!ADC=%lu#", last_ADC_value);
                send_uart_message(transmission_buffer);
                send_uart_message("\r\n");

                timeout_counter = current_tick;
            }
            break;

        default:
            break;
    }
}
