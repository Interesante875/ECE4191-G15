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
    #define ULTRASONIC_BURSTS 1

    /*
    
    bit 0 - FRONT_LEFT
    bit 1 - FRONT_RIGHT 
    bit 2 - LEFT_FRONT
    bit 3 - LEFT_BACK
    bit 4 - RIGHT_FRONT 
    bit 5 - RIGHT_BACK   
    bit 6 - BACK_LEFT  
    bit 7 - BACK_RIGHT  
    */ 
    extern int udsState;
    extern double kaldist_measure[NUMBER_OF_UDS];
    extern double uds_LEFT[ARRAY_SIZE];
    extern double uds_RIGHT[ARRAY_SIZE];
    extern double uds_FLEFT[ARRAY_SIZE];
    extern double uds_FRIGHT[ARRAY_SIZE];
    extern double uds_BACK[ARRAY_SIZE];
    
    // Function prototypes
    void ultrasonic_on();
    void ultrasonic_off();
    void ultrasonic_start();
    void ultrasonic_stop();
    
    void ultrasonic_trigger_burst();
    
    ///void ultrasonic_setup();
    
    void ultrasonic_transmit();
    void ultrasonic_get_distance();
    void ultrasonic_select(int idx);
    void ultrasonic_measuring();
    double kalman_filter(double U, int idx);
    double median(int idx, double distance);

    CY_ISR (ISR_Handler_ultrasonic_burst);
    CY_ISR (ISR_Handler_ultrasonic_echo);


#endif // ULTRASONIC_H

/* [] END OF FILE */
