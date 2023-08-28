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
#ifndef IRSENSOR_H
#define IRSENSOR_H
    
    #include "project.h"
    #include <stdio.h>

    typedef enum {
        PRESENCE,
        ABSENCE
    } DETECTED;
    
    DETECTED ir_LEFT = ABSENCE;
    DETECTED ir_RIGHT = ABSENCE;
    
    void ir_start();
    void ir_stop();
    
    CY_ISR (ISR_Handler_IR_LEFT);
    CY_ISR (ISR_Handler_IR_RIGHT);
    CY_ISR (ISR_Handler_puck);
    
#endif /* IRSENSOR_H */
/* [] END OF FILE */
