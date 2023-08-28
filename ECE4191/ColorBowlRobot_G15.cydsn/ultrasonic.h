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

#ifndef ULTRASONIC_H
#define ULTRASONIC_H

#include <stdint.h>
#include "project.h"
#include "cytypes.h"
    
    #define N 4

    /*
    
    bit 0 - LEFT 
    bit 1 - RIGHT 
    bit 2 - FLEFT
    bit 3 - FRIGHT
    bit 4 - NOT IMPLEMENTED  
    bit 5 - NOT IMPLEMENTED   
    bit 6 - NOT IMPLEMENTED  
    bit 7 - NOT IMPLEMENTED  
    */ 
    int udsState = 0;
    double kaldist_measure[N] = {0};
    
    // Function prototypes
    void ultrasonic_setup();
    void ultrasonic_off();
    void ultrasonic_transmit();
    void ultrasonic_get_distance();
    void ultrasonic_select(int idx);
    double kalman_filter(double U, int idx);

    CY_ISR (ISR_Handler_Timer);


#endif // ULTRASONIC_H

/* [] END OF FILE */
