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
    
    #include "project.h"
    #include "cytypes.h"
    #include <stdbool.h>
    
    typedef enum {
        BLUE_ZONE,
        GREEN_ZONE,
        RED_ZONE

    } PIN_ZONE_COLOR;
    
    typedef enum {
        READY,
        INSTRUCTION,
        START

    } INPUT_STATE;
    
    
    extern bool stringNotReceived;
    extern int level_1_pin_deck_num;
    extern int level_current_level;
    extern INPUT_STATE inputState;
    extern PIN_ZONE_COLOR pin_deck_zone_color;
    
    CY_ISR(ISR_Handler_Input);
    void bluetooth_start();
    void bluetooth_stop();
    void printValue(const char *format, ...);
    void handshake();
    
    
#endif /* BLUETOOTH_H */
/* [] END OF FILE */
