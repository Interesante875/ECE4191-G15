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

#ifndef BLUETOOTH_H
#define BLUETOOTH_H

    #include <stdbool.h>
    #include <stdint.h> // Include standard integer types
    #include <stdarg.h> // Include for variadic arguments
    #include <project.h>
    
    #define MAX_STRING_LENGTH 128

    // Enumerations should use all uppercase with underscores
    typedef enum {
        PinZoneColorBlue,
        PinZoneColorGreen,
        PinZoneColorRed
    } PinZoneColor;

    typedef enum {
        InputStateReady,
        InputStateInstruction,
        InputStateStart,
        InputStateEnd
    } InputState;

    extern bool isStringNotReceived;
    extern int levelOnePinDeckNum;
    extern int currentLevel;
    extern InputState inputState;
    extern PinZoneColor zoneColor;
    extern PinZoneColor levelTwoZoneColor;
    extern int test_val;

    void initializeBluetooth(void);
    void shutdownBluetooth(void);
    void printValue(const char *format, ...);
    void performHandshake(void);
    void waitingHandshake(void);
    void testingValue();
    
    CY_ISR (ISR_Handler_Input);

#endif /* BLUETOOTH_H */

/* [] END OF FILE */
