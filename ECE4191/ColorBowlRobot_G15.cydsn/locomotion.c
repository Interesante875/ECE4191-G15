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
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "locomotion.h"
#include "controller.h"

void uart_initialization() {
    UART_1_Start();
}

void uart_stop() {
    UART_1_Stop();
}

void motor_left_start_QuadDec() {
    QuadDec_LEFT_Start();
}

void motor_right_start_QuadDec() {
    QuadDec_RIGHT_Start();   
}

void motor_left_stop_QuadDec() {
    QuadDec_LEFT_Start();
}

void motor_right_stop_QuadDec() {
    QuadDec_RIGHT_Start();
}

void motor_left_reset_QuadDec() {
    QuadDec_LEFT_SetCounter(0);
}

void motor_right_reset_QuadDec() {
    QuadDec_RIGHT_SetCounter(0);
}

void motor_left_set_QuadDec(int target_ticks) {
    QuadDec_LEFT_SetCounter(target_ticks);
}

void motor_right_set_QuadDec(int target_ticks) {
    QuadDec_RIGHT_SetCounter(target_ticks);
}

int motor_left_get_count_QuadDec() {
    int count = QuadDec_LEFT_GetCounter();
    return count;
}

int motor_right_get_count_QuadDec() {
    int count = QuadDec_RIGHT_GetCounter();
    return count;
}

void motor_left_set_pwm_compare(uint8 compareValue) {
    if (compareValue > PWM_MAX) {
        PWM_LEFT_WriteCompare(PWM_MAX);
        return;
    }
    if (compareValue < PWM_MIN) {
        PWM_LEFT_WriteCompare(PWM_MIN);
        return;
    }
    PWM_LEFT_WriteCompare(compareValue);
}

void motor_right_set_pwm_compare(uint8 compareValue) {
    if (compareValue > PWM_MAX) {
        PWM_RIGHT_WriteCompare(PWM_MAX);
        return;
    }
    if (compareValue < PWM_MIN) {
        PWM_RIGHT_WriteCompare(PWM_MIN);
        return;
    }
    PWM_RIGHT_WriteCompare(compareValue);
}

uint8 motor_left_get_pwm() {
    uint8 curr_left_pwm = PWM_LEFT_ReadCompare();
    return curr_left_pwm;
}

uint8 motor_right_get_pwm() {
    uint8 curr_right_pwm = PWM_RIGHT_ReadCompare();
    return curr_right_pwm;
}

void motor_left_start_pwm() {
    PWM_LEFT_Start();
}

void motor_left_stop_pwm() {
    PWM_LEFT_Stop();   
}

void motor_right_start_pwm() {
    PWM_RIGHT_Start();
}

void motor_right_stop_pwm() {
    PWM_RIGHT_Stop();   
}

void motor_start(uint8 pwm) {
    motor_left_start_QuadDec();
    motor_right_start_QuadDec();
    
    motor_left_start_pwm();
    motor_right_start_pwm();
    
    motor_left_set_pwm_compare(pwm);
    motor_right_set_pwm_compare(pwm);
}


void motor_stop() {
    motor_left_stop_pwm();
    motor_right_stop_pwm();
    
    motor_left_reset_QuadDec();
    motor_right_reset_QuadDec();
    
    motor_left_stop_QuadDec();
    motor_right_stop_QuadDec();
    wheel_motion_set(STOP);
}

void wheel_motion_set(MOTION motion) {
    

    switch (motion) {
        case FORWARD:
            MOTOR_LEFT_IN_1_Write(0);
            MOTOR_LEFT_IN_2_Write(1);
            
            MOTOR_RIGHT_IN_3_Write(0);
            MOTOR_RIGHT_IN_4_Write(1);
            break;
        case BACKWARD:
            MOTOR_LEFT_IN_1_Write(1);
            MOTOR_LEFT_IN_2_Write(0);
            
            MOTOR_RIGHT_IN_3_Write(1);
            MOTOR_RIGHT_IN_4_Write(0);
            break;
        case LEFT:
            MOTOR_LEFT_IN_1_Write(1);
            MOTOR_LEFT_IN_2_Write(0);
            
            MOTOR_RIGHT_IN_3_Write(0);
            MOTOR_RIGHT_IN_4_Write(1);
            break;
        case RIGHT:
            MOTOR_LEFT_IN_1_Write(0);
            MOTOR_LEFT_IN_2_Write(1);
            
            MOTOR_RIGHT_IN_3_Write(1);
            MOTOR_RIGHT_IN_4_Write(0);
            break;
        default:
            MOTOR_LEFT_IN_1_Write(0);
            MOTOR_LEFT_IN_2_Write(0);
            
            MOTOR_RIGHT_IN_3_Write(0);
            MOTOR_RIGHT_IN_4_Write(0);
            break;
            
    }
    
}

void wheel_move_by_ticks(MOTION motion, uint8 pwm, int target_ticks) {
     /**
     * Move the wheels of the robot by a specified number of quad decoder ticks using a closed-loop control approach.
     *
     * This function moves the robot's wheels by a specified number of encoder ticks using a closed-loop control approach.
     * It adjusts the slave wheel's PWM signal based on the difference in encoder ticks between the master and slave wheels,
     * applying a proportional correction to maintain synchronization.
     *
     * @param motion        The desired motion of the robot wheels (FORWARD, BACKWARD, LEFT, RIGHT, or STOP).
     * @param pwm           The PWM signal value for controlling both motors.
     * @param target_ticks  The target number of encoder ticks to move the wheels.
     */
    
    char string_1[30];
    char string_2[30];
    
    motor_start(pwm);
    
    int master_pwm = motor_left_get_pwm();
    int slave_pwm = motor_right_get_pwm();
    
    int master_motor_left_ticks = -motor_left_get_count_QuadDec();
    int slave_motor_right_ticks = -motor_right_get_count_QuadDec();
    
    wheel_motion_set(motion);
    
    while (abs(master_motor_left_ticks) < target_ticks) {
        
        master_pwm = motor_left_get_pwm();
        
        uint8 updated_slave_pwm = computePController(master_motor_left_ticks, 
            slave_motor_right_ticks, master_pwm);
        
        
        motor_right_set_pwm_compare((uint8) updated_slave_pwm);
        master_motor_left_ticks = -motor_left_get_count_QuadDec();
        slave_motor_right_ticks = -motor_right_get_count_QuadDec();
        
    }
    
    motor_stop();
}


void wheel_move_by_distance(MOTION motion, uint8 pwm, double distance) {
    motor_start(pwm);
    
    double ticks = (distance * TICKS_PER_REVOLUTION)/(2 * CY_M_PI * WHEEL_RADIUS);
    int travel_ticks = (int) ticks;
    
    int master_pwm = motor_left_get_pwm();
    int slave_pwm = motor_right_get_pwm();
    
    int master_motor_left_ticks = -motor_left_get_count_QuadDec();
    int slave_motor_right_ticks = -motor_right_get_count_QuadDec();
    
    wheel_motion_set(motion);
    
    while (abs(master_motor_left_ticks) < travel_ticks) {
        
        master_pwm = motor_left_get_pwm();
        
        uint8 updated_slave_pwm = computePController(master_motor_left_ticks, 
            slave_motor_right_ticks, master_pwm);
        
        motor_right_set_pwm_compare((uint8) updated_slave_pwm);
        master_motor_left_ticks = -motor_left_get_count_QuadDec();
        slave_motor_right_ticks = -motor_right_get_count_QuadDec();
        
    }

    motor_stop();
}

void wheel_turn_by_ticks(MOTION motion, uint8 pwm, int turn_ticks) {
    
    char string_1[30];
    char string_2[30];
    motor_start(pwm);
    
    int master_pwm = motor_left_get_pwm();
    int slave_pwm = motor_right_get_pwm();
    
    int master_motor_left_ticks = -motor_left_get_count_QuadDec();
    int slave_motor_right_ticks = -motor_right_get_count_QuadDec();
    
    wheel_motion_set(motion);
    
    while (abs(master_motor_left_ticks) < turn_ticks) {
        
        master_pwm = motor_left_get_pwm();
        
        uint8 updated_slave_pwm = computePController(master_motor_left_ticks, 
            slave_motor_right_ticks, master_pwm);
        
        
        motor_right_set_pwm_compare((uint8) updated_slave_pwm);
        master_motor_left_ticks = -motor_left_get_count_QuadDec();
        slave_motor_right_ticks = -motor_right_get_count_QuadDec();
        
    }

    motor_stop();
    
}


void wheel_turn_by_angle(MOTION motion, uint8 pwm, double angle) {
    
    double circum_distance = angle * CY_M_PI / 360 * WHEEL_DISTANCE;
    double n_revs = circum_distance/(2 * CY_M_PI * WHEEL_RADIUS);
    double ticks = TICKS_PER_REVOLUTION * n_revs;
    int turn_ticks = (int) ticks;
    
    motor_start(pwm);
    
    int master_pwm = motor_left_get_pwm();
    int slave_pwm = motor_right_get_pwm();
    
    int master_motor_left_ticks = -motor_left_get_count_QuadDec();
    int slave_motor_right_ticks = -motor_right_get_count_QuadDec();
    
    wheel_motion_set(motion);
    
    while (abs(master_motor_left_ticks) < turn_ticks) {
        
        master_pwm = motor_left_get_pwm();
        
        uint8 updated_slave_pwm = computePController(master_motor_left_ticks, 
            slave_motor_right_ticks, master_pwm);
        
        
        motor_right_set_pwm_compare((uint8) updated_slave_pwm);
        master_motor_left_ticks = -motor_left_get_count_QuadDec();
        slave_motor_right_ticks = -motor_right_get_count_QuadDec();
        
    }
    
    motor_stop();
    
}
/* [] END OF FILE */
