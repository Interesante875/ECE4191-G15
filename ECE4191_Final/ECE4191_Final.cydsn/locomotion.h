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
#ifndef LOCOMOTION_H
    #define LOCOMOTION_H
    
    #define TICKS_PER_REVOLUTION 13750
    #define WHEEL_RADIUS 0.02695
    #define WHEEL_DISTANCE 0.1475
    #define HALF_WIDTH 0.122
    
    #define EMF_BUFFER_DELAY 50
    
    #include "motor_control.h"
    
    void wheel_move_by_ticks(MotionDirection motion, int pwm, int target_ticks);
    
    
#endif

/* [] END OF FILE */
