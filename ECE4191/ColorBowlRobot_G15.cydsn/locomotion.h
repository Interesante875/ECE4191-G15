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

    #include "project.h"
    #include "cytypes.h"
    #include "motor_control.h"
    
    #define EMF_BUFFER_DELAY 200

    extern int flag_distance_moving;
    
    
    CY_ISR (ISR_Handler_wheel_controller);
    void wheel_controller_start();
    void wheel_controller_stop();
    
    
    void wheel_move_by_ticks(MOTION, uint8, int);
    void wheel_move_by_metrics(MOTION motion, uint8 pwm, double metrics);
    void wheel_move(MOTION motion, uint8 pwm);
    
    void robot_locomotion_initialization();
    void angle_correction(uint8 pwm, double flu, double fru);
    void angle_correction_with_ticks (MOTION motion, uint8 pwm);
    
    CY_ISR (ISR_Handler_wheel_controller);

#endif /* LOCOMOTION_H */
/* [] END OF FILE */
