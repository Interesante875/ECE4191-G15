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
#include "motor.h"

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


/* [] END OF FILE */
