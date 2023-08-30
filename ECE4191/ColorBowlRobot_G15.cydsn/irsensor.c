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
#include "irsensor.h"
#include "locomotion.h"

void ir_start() {
    isr_IR_Sensor_LEFT_StartEx(ISR_Handler_IR_LEFT);
    isr_IR_Sensor_RIGHT_StartEx(ISR_Handler_IR_RIGHT);
    isr_puck_StartEx(ISR_Handler_puck);
}

void ir_stop() {
    isr_IR_Sensor_LEFT_Stop();
    isr_IR_Sensor_RIGHT_Stop();
    isr_puck_Stop();
}

CY_ISR (ISR_Handler_IR_LEFT) {
    
    if (ir_LEFT == PRESENCE) ir_LEFT = ABSENCE;
    else {
        ir_LEFT = PRESENCE;  
    }    
}

CY_ISR (ISR_Handler_IR_RIGHT) {
    if (ir_RIGHT == PRESENCE) ir_RIGHT = ABSENCE;
    else ir_RIGHT = PRESENCE;    
}

CY_ISR (ISR_Handler_puck) {
    
    if (moveStatus == DISABLE) {
        wheel_motion_set(STOP);
    }
    
    // Not implemented
}
/* [] END OF FILE */
