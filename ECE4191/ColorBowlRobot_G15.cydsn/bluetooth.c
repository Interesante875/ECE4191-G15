/* ========================================
 *
 * Copyright YOUR COMPANY, THE YEAR
 * All Rights Reserved
 * UNPUBLISHED, LICENSED SOFTWARE.
 *
 * CONFIDENTIAL AND PROPRIETARY INFORMATION
 * WHICH IS THE PROPERTY OF your company.
 *
 * ========================================
*/
#include "project.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <stdbool.h>
#include <stdarg.h>
#include "bluetooth.h"


char rxBuffer[100]; // Buffer to store received characters
char *rxBufferPtr = rxBuffer; // Pointer to the current position in the buffer
char printString[100]; // Buffer to store formatted strings
int rxBufferSize = 0; // Size of the received data buffer
int receivedInt = 0; // Received integer value
bool stringNotReceived = true;

int level_1_pin_deck_num = 0;
int level_current_level = 0;
PIN_ZONE_COLOR pin_deck_zone_color;
INPUT_STATE inputState = READY;


CY_ISR(ISR_Handler_Input) {
    *rxBufferPtr = UART_1_GetChar();
       
    if (*rxBufferPtr == '!') {
        *rxBufferPtr = '\0';  

        printValue("INPUT: %s\n", rxBuffer);

        rxBufferPtr = &(rxBuffer[0]);
        
        stringNotReceived = false;
        
    }
    else
    {
        rxBufferPtr++;
    }
    
}
/**
 * Initializes the Bluetooth module by starting the UART communication.
 */
void bluetooth_start() {
    UART_1_Start();
    //isr_input_StartEx(ISR_Handler_Input);
    // Implement a mechanism to start bluetooth
}

/**
 * Stops the Bluetooth module by stopping the UART communication.
 */
void bluetooth_stop() {
    UART_1_Stop();   
    isr_input_Stop();
    // Implement a mechanism to stop bluetooth
}

/**
 * Initiates a handshake protocol with the connected device.
 */
void handshake() {
    
    static int state = 0;
    
    char* token;
    
    if (stringNotReceived) return;

    if (!strcmp(rxBuffer, "Are you ready?") && inputState == READY) {
        printValue("Ready\n  ");    
        inputState = INSTRUCTION;
    }
    else if (!strcmp(rxBuffer, "Start") && state == 2) {
        printValue("OK to start\n  ");
        inputState = READY;
    }
    else if (state == INSTRUCTION){
        token = strtok(rxBuffer, "\n");
        while (token != NULL) {
            int first_number, second_number;
            char color[20];
            
            // Parse each line into the specified format
            if (sscanf(token, "%d %19s %d", &first_number, color, &second_number) == 3) {
                // Print the parsed values
                printValue("First Number: %d, Color: %s, Second Number: %d\n", first_number, color, second_number);
                inputState = START;
                
                level_1_pin_deck_num = first_number;
                level_current_level = second_number;
                
            } else {
                // Print an error message for incorrect format
                printValue("Error: Invalid format in line: %s\n", token);
                state = 1;
            }
            
            // Get the next line
            token = strtok(NULL, "\n");
        }
    }
}

void wait_for_handshake() {
    
    
}

/**
 * Prints formatted data over UART communication.
 * @param format The format string for the data to be printed.
 * @param ... The variable number of arguments to be formatted and printed.
 */
void printValue(const char *format, ...) {
    va_list args;
    va_start(args, format);
    vsprintf(printString, format, args);
    va_end(args);
    
    UART_1_PutString(printString); // Sending the formatted string over UART
    
}


/* [] END OF FILE */
