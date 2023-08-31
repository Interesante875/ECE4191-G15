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
    #include "median_filter.h"
    
    #define NUMBER_OF_UDS 5
    #define ULTRASONIC_BURSTS 3

    /*
    
    bit 0 - LEFT 
    bit 1 - RIGHT 
    bit 2 - FLEFT
    bit 3 - FRIGHT
    bit 4 - BACK 
    bit 5 - NOT IMPLEMENTED   
    bit 6 - NOT IMPLEMENTED  
    bit 7 - NOT IMPLEMENTED  
    */ 
    int udsState;
    double kaldist_measure[NUMBER_OF_UDS];
    double uds_LEFT[ARRAY_SIZE];
    double uds_RIGHT[ARRAY_SIZE];
    double uds_FLEFT[ARRAY_SIZE];
    double uds_FRIGHT[ARRAY_SIZE];
    double uds_BACK[ARRAY_SIZE];
    
    // Function prototypes
    void ultrasonic_setup();
    void ultrasonic_off();
    void ultrasonic_transmit();
    void ultrasonic_get_distance();
    void ultrasonic_select(int idx);
    void ultrasonic_measuring();
    double kalman_filter(double U, int idx);

    CY_ISR (ISR_Handler_Ultrasonic_Burst_Timer);
    CY_ISR (ISR_Handler_Ultrasonic_Timer);


#endif // ULTRASONIC_H

/* [] END OF FILE */
