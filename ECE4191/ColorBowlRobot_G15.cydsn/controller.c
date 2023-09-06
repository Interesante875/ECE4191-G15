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

/* [] END OF FILE */
#include "project.h"
#include <stdio.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "controller.h"

static int error = 0;
static int accumulated_error = 0;
static int last_error = 0;
static int derivative_error = 0;
static double kp = P_CONST;
static double ki = 0;
static double kd = 0;
static int correction_signal = 0;
static int wheel_time_period = 1000;

void initializePController(double prop_constant) {
    error = 0;
    kp = prop_constant;
    correction_signal = 0;
}

void initializePIDController(double prop_constant, double int_constant, double de_constant) {
    error = 0;
    accumulated_error = 0;
    last_error = 0;
    derivative_error = 0;
    
    wheel_time_period = Wheel_Timer_ReadPeriod();
    
    kp = prop_constant;
    ki = int_constant;
    kd = de_constant;
    
}

void resetPController() {
    error = 0;
    kp = P_CONST;
    correction_signal = 0;
}

void resetPIDController() {
    error = 0;
    accumulated_error = 0;
    last_error = 0;
    derivative_error = 0;
    correction_signal = 0;
    
    wheel_time_period = Wheel_Timer_ReadPeriod();
    
    kp = P_CONST;
    ki = I_CONST;
    kd = D_CONST;
    
}

void compute_error(int master_tick, int slave_tick) {
    if (master_tick > 0 && slave_tick > 0) error = master_tick - slave_tick;
    if (master_tick < 0 && slave_tick < 0) error = -(master_tick - slave_tick);
    if (master_tick > 0 && slave_tick < 0) error = abs(master_tick) - abs(slave_tick);
    if (master_tick < 0 && slave_tick > 0) error = abs(master_tick) - abs(slave_tick);
    
}

void computePController_correctionSignal() {
    correction_signal = (int) (kp * (double) error);
}

void computePIDController_acc_error() {
    accumulated_error += error; 
    
    if (accumulated_error > PWM_MAX) accumulated_error = PWM_MAX;
    else if (accumulated_error < PWM_MIN) accumulated_error = PWM_MIN; 
}

void computePIDController_dev_error() {
    derivative_error = error - last_error;
    
    if (derivative_error > PWM_MAX) derivative_error = PWM_MAX;
    else if (derivative_error < PWM_MIN) derivative_error = PWM_MIN;
}

void computePIDController_correctionSignal() {
    
    correction_signal = (int) (kp * (double) error + ki * (double)accumulated_error + kd * (double) derivative_error);
}

uint8 computePController_output(uint8 master_pwm) {
    int slave_pwm = master_pwm + correction_signal;
    return (uint8) slave_pwm>PWM_MAX?PWM_MAX:(slave_pwm<(master_pwm - 20)?(master_pwm - 20):slave_pwm);
}

uint8 computePIDController_output(uint8 master_pwm) {
    
    computePIDController_acc_error();
    computePIDController_dev_error();
    int slave_pwm = master_pwm - correction_signal;
    
    last_error = error;
    return (uint8) slave_pwm>PWM_MAX?PWM_MAX:(slave_pwm<PWM_MIN?PWM_MIN:slave_pwm);
}

uint8 computePController(int master_tick, int slave_tick, uint8 master_pwm) {
    compute_error(master_tick, slave_tick);
    computePController_correctionSignal();
    return computePController_output(master_pwm);
}

uint8 computePIDController(int master_tick, int slave_tick, uint8 master_pwm) {
    compute_error(master_tick, slave_tick);
    computePIDController_correctionSignal();
    return computePIDController_output(master_pwm);
}

