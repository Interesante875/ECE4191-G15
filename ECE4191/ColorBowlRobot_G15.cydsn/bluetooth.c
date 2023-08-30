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
#include <stdarg.h>

char rxBuffer[100]; // Buffer to store received characters
char *rxBufferPtr = rxBuffer; // Pointer to the current position in the buffer
char printString[100]; // Buffer to store formatted strings
int rxBufferSize = 0; // Size of the received data buffer
int receivedInt = 0; // Received integer value

/**
 * Initializes the Bluetooth module by starting the UART communication.
 */
void bluetooth_start() {
    UART_1_Start();
    // Implement a mechanism to start bluetooth
}

/**
 * Stops the Bluetooth module by stopping the UART communication.
 */
void bluetooth_stop() {
    UART_1_Stop();   
    // Implement a mechanism to stop bluetooth
}

/**
 * Initiates a handshake protocol with the connected device.
 */
void handshake() {
    UART_1_PutString("Are you ready?!\n");   
    
    // Implement a mechanism to receive input "Ready!"
    
    // Implement a mechanism to receive input Level Details
    
    UART_1_PutString("Received\n"); 
    
    // Implement a mechanism to receive input "Start!"
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

/*
// Simulating UART communication functions for demonstration purposes
void UART_1_PutChar(char c) {
    // Simulate sending a character over UART
    putchar(c);
}

void UART_1_PutString(const char *str) {
    // Simulate sending a string over UART
    while (*str) {
        UART_1_PutChar(*str);
        str++;
    }
}
*/


/**
 * Interrupt Service Routine (ISR) for handling incoming UART data.
 */
CY_ISR(ISR_Handler_Input) {
    *rxBufferPtr = UART_1_GetChar();
       
    if (*rxBufferPtr == '!') {
        *rxBufferPtr = '\0';  

        receivedInt = atoi(rxBuffer);
        
        printValue("INPUT: %d\n", receivedInt);

        rxBufferPtr = &(rxBuffer[0]);
        
        //wheel_move_by_ticks(FORWARD, 240, receivedInt);
        // wheel_turn_by_angle(motion, 240, receivedInt);
    }
    else
    {
        rxBufferPtr++;
    }
    
}
/* [] END OF FILE */
