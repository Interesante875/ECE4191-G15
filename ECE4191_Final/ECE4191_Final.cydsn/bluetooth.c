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
#include "bluetooth.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

/* Can we manually readjust everything after each level? */

/* Global Variables */
bool isStringNotReceived;
int levelOnePinDeckNum;
int currentLevel;
InputState inputState;
PinZoneColor zoneColor;
PinZoneColor levelTwoZoneColor;
int levelThreePinDeckNum;
int levelFourPinDeckNum;
PinZoneColor levelThreeZoneColor;
PinZoneColor levelFourZoneColor;
int test_val;

/* Local Variables */
char rxBuffer[100]; // Buffer to store received characters
char *rxBufferPtr = rxBuffer; // Pointer to the current position in the buffer
char printString[100]; // Buffer to store formatted strings
int rxBufferSize = 0; // Size of the received data buffer
char color[20];

/* Initializes the Bluetooth module by starting the UART communication. */
void initializeBluetooth() {
    UART_1_Start();
    
    isStringNotReceived = true;
    
    isr_input_StartEx(ISR_Handler_Input);
    // Implement a mechanism to start bluetooth
}

/**
 * Stops the Bluetooth module by stopping the UART communication.
 */
void shutdownBluetooth() {
    UART_1_Stop();   
    isr_input_Stop();
    // Implement a mechanism to stop bluetooth
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

void performHandshake(void) {
    static int state = 0;
    
    char* token;
    
    if (isStringNotReceived) return;

    if (!strcmp(rxBuffer, "Are you ready?") && inputState == InputStateReady) {
        printValue("Ready\n  ");    
        inputState = InputStateInstruction;
    }
    else if (!strcmp(rxBuffer, "Start") && inputState == InputStateStart) {
        printValue("OK to start\n  ");
        inputState = InputStateEnd;
    }
    else if (inputState == InputStateInstruction){
        token = strtok(rxBuffer, "\n");
        while (token != NULL) {
            int first_number, second_number;
            
            
            // Parse each line into the specified format
            if (sscanf(token, "%d %19s %d", &first_number, color, &second_number) == 3) {
                // Print the parsed values
                printValue("Pin Deck Number: %d, Pin Deck Color: %s, Current Level: %d\n", first_number, color, second_number);
                
                levelOnePinDeckNum = first_number;
                currentLevel = second_number;

                if (!strcasecmp(color, "blue")) levelTwoZoneColor = PinZoneColorBlue;
                else if (!strcasecmp(color, "green")) levelTwoZoneColor = PinZoneColorGreen;
                else if (!strcasecmp(color, "red")) levelTwoZoneColor = PinZoneColorRed;
                else {
                    printValue("Error: Invalid color in instruction: %s\n", color);
                    
                    inputState = InputStateInstruction;
                    
                    return;
                }
                
                switch (currentLevel) {
                    case 1:
                        if (levelOnePinDeckNum == 1 || levelOnePinDeckNum == 3) {
                            zoneColor = PinZoneColorBlue;
                        } else if (levelOnePinDeckNum == 2 || levelOnePinDeckNum == 4) {
                            zoneColor = PinZoneColorGreen;
                        } else {
                            zoneColor = PinZoneColorRed;
                        }
                        break;
                    case 2:
                        zoneColor = levelTwoZoneColor;
                        break;
                    default:
                        break;
                }
                
                inputState = InputStateStart;
                
            } else {
                // Print an error message for incorrect format
                printValue("Error: Invalid format in line: %s\n", token);
                state = InputStateInstruction;
            }
            
            // Get the next line
            token = strtok(NULL, "\n");
        }
    }   
}

void waitingHandshake(void) {
    
    printValue("Waiting...\n");
    while (inputState != InputStateEnd) {
        while (isStringNotReceived);
        performHandshake();
        isStringNotReceived = true;
    }
    
}

void testingValue() {
    printValue("Waiting...\n");
    while (isStringNotReceived);
    test_val = 0;
    char* token;
    token = strtok(rxBuffer, "\n");
    sscanf(token, "%d", &test_val);
    printValue("Received Input Test Val %s %d\n", token, test_val);
    isStringNotReceived = true;
}

CY_ISR (ISR_Handler_Input) {
    *rxBufferPtr = UART_1_GetChar();
    
    if (*rxBufferPtr == '\r' || *rxBufferPtr == '\n') {
        return;
    }
       
    if (*rxBufferPtr == '!') {
        *rxBufferPtr = '\0';  

        printValue("INPUT: %s\n  ", rxBuffer);

        rxBufferPtr = &(rxBuffer[0]);
        
        isStringNotReceived = false;
        
    }
    else
    {
        rxBufferPtr++;

    }
    
}

/* [] END OF FILE */
