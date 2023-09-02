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
#ifndef MOTOR_H
#define MOTOR_H
    
    #define TICKS_PER_REVOLUTION 14375 
    #define WHEEL_RADIUS 0.02695 
    #define WHEEL_DISTANCE 0.24 
    #define HALF_WIDTH 0.122
    #define PWM_MAX 255
    #define PWM_MIN 200
    
    #include "project.h"
    #include "cytypes.h"
    

    void motor_left_start_QuadDec();
    void motor_left_stop_QuadDec();
    void motor_left_reset_QuadDec();
    void motor_left_set_QuadDec(int);
    int motor_left_get_count_QuadDec();
    void motor_left_set_pwm_compare(uint8);
    uint8 motor_left_get_pwm();
    void motor_left_start_pwm();
    void motor_left_stop_pwm();

    void motor_right_start_QuadDec();
    void motor_right_stop_QuadDec();
    void motor_right_reset_QuadDec();
    void motor_right_set_QuadDec(int);
    int motor_right_get_count_QuadDec();
    void motor_right_set_pwm_compare(uint8);
    uint8 motor_right_get_pwm();
    void motor_right_start_pwm();
    void motor_right_stop_pwm();
    
#endif


/* [] END OF FILE */
