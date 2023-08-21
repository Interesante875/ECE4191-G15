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
#include <stdint.h>
#include "project.h"

#ifndef MOTOR_CONTROL_H
#define MOTOR_CONTROL_H

#define ENABLE 1
#define DISABLE 0
#define FORWARD 0
#define BACKWARD 1
#define LEFT 2
#define RIGHT 3
#define WHEEL_FULL_SPEED 255
#define WHEEL_HALF_SPEED 160
#define WHEEL_SLOW 127
#define TICKS_PER_REVOLUTION 1000 // Example value, adjust as needed
#define TICKS_PER_SECOND 1000000 
#define WHEEL_RADIUS 0.02695 // Example value in meters
#define WHEEL_DISTANCE 0.25 // Example value in centimeters (distance between the wheels)
#define PI 3.1415926

void motor_status(int mode);

void motor_pwm_control(int pwm_value);

void quad_decoder_status(int mode);

void quad_dec_reset_count(int select);

int quad_dec_get_count(int select, int count);

void wheel_timer_status(int mode);

void wheel_timer_reset_count();

void wheel_timer_get_count(int* timer);

double linear_drive_time(double distance, double angular_wheel_velocity);

double turn_drive_time(double angle, double angular_wheel_velocity);

void drive_control(double* differential_command, double drive_time);
    
void motor_move(int mode, int direction, int speed);

void control_wheel(double velocity, double angular_velocity);



void compute_angular_velocities(double *angular_velocity_left, double *angular_velocity_right);

void compute_metrics(double* displacement, double* velocity, double* acceleration);


#endif
