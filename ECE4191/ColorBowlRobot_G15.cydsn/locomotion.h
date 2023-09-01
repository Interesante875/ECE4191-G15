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
    
    
    #define TICKS_PER_REVOLUTION 14375 
    //#define TICKS_PER_REVOLUTION 2500
    #define WHEEL_RADIUS 0.02695 // wheel radius value in meters
    #define WHEEL_DISTANCE 0.24 
    
    #include "project.h"
    #include "cytypes.h"
    
    
    typedef enum {
        FORWARD,
        BACKWARD,
        LEFT,
        RIGHT,
        STOP
    } MOTION;
    
    typedef enum {
        ENABLE,
        DISABLE
    } MOVE_STATUS;
    
    MOVE_STATUS moveStatus;
    MOTION currentMotion;

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
    
    
    void motor_start(uint8);
    void motor_stop();
    void motor_off();
    void wheel_motion_set(MOTION motion);
    void wheel_move_by_ticks(MOTION, uint8, int);
    void wheel_move_by_distance(MOTION motion, uint8 pwm, double distance);
    void wheel_turn_by_ticks(MOTION motion, uint8 pwm, int turn_ticks);
    void wheel_turn_by_angle(MOTION motion, uint8 pwm, double angle);
    
    void robot_locomotion_initialization();
    void angle_correction(uint8 pwm, double flu, double fru);
    
    CY_ISR (ISR_Handler_wheel_controller);

#endif /* LOCOMOTION_H */
/* [] END OF FILE */
