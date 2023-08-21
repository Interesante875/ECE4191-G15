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
#include "motor_control.h"
#include "project.h" // Include the necessary project-specific header file
#include <stdio.h>

void motor_status(int mode) {
    if (mode == ENABLE) {
        PWM_1_Start();
    }
    else if (mode == DISABLE) {
        PWM_1_Stop();
    }
}

void motor_pwm_control(int pwm_value) {
    PWM_1_WriteCompare(pwm_value);
}

void quad_decoder_status(int mode) {
    if (mode == ENABLE) {
        QuadDec_1_Start();
        QuadDec_2_Start();
    }
    else if (mode == DISABLE) {
        QuadDec_1_Stop();
        QuadDec_2_Stop();
    }
}

void quad_dec_reset_count(int select) {
    switch (select) {
        case 0:
            QuadDec_1_SetCounter(0);
            QuadDec_2_SetCounter(0);
            break;
        case 1:
            QuadDec_1_SetCounter(0);
            break;
        case 2:
            QuadDec_2_SetCounter(0);
            break;
        default:
            QuadDec_1_SetCounter(0);
            QuadDec_2_SetCounter(0);
            break;
    }
}

int quad_dec_get_count(int select, int count) {
    switch (select) {
        case 1:
            count = QuadDec_1_GetCounter();
            break;
        case 2:
            count = QuadDec_2_GetCounter();
            break;
        default:
            count = QuadDec_1_GetCounter();
            break;
    }
    
    return count;
}

void wheel_timer_status(int mode) {
    if (mode == ENABLE) {
        Wheel_Timer_Start();
    }
    else if (mode == DISABLE) {
        Wheel_Timer_Stop();
    }
}

void wheel_timer_reset_count() {
    Wheel_Timer_WriteCounter(0);
}

void wheel_timer_get_count(int* timer) {
    *timer = Wheel_Timer_ReadCounter();
}

double linear_drive_time(double distance, double angular_wheel_velocity) {
    double d = distance;
    double awv = angular_wheel_velocity;
    double drive_time = d / (awv * WHEEL_RADIUS);
    
    return drive_time;
}

double turn_drive_time(double angle, double angular_wheel_velocity) {
    double ang = angle;
    double awv = angular_wheel_velocity;
    double turn_time = (ang)/(2*awv*WHEEL_RADIUS/WHEEL_DISTANCE);
    
    return turn_time;
    
}

void compute_angular_velocities(double *angular_velocity_left, double *angular_velocity_right) {
    static int16 QuadDec_1_prevCounterValue = 0;
    static int16 QuadDec_2_prevCounterValue = 0;
    static uint32 prevTime = 0;

    // Get the current counter values for both encoders
    int16 QuadDec_1_currentCounterValue = QuadDec_1_GetCounter();
    int16 QuadDec_2_currentCounterValue = QuadDec_2_GetCounter();

    // Calculate time difference
    uint32 currentTime = prevTime;
    uint32 deltaTime = currentTime - prevTime;

    // Calculate angular displacement for both wheels
    double angular_displacement_left = ((double)(QuadDec_1_currentCounterValue - QuadDec_1_prevCounterValue) / TICKS_PER_REVOLUTION) * 2 * PI;
    double angular_displacement_right = ((double)(QuadDec_2_currentCounterValue - QuadDec_2_prevCounterValue) / TICKS_PER_REVOLUTION) * 2 * PI;

    // Calculate angular velocities for left and right wheels
    *angular_velocity_left = angular_displacement_left / ((double)deltaTime / (double)TICKS_PER_SECOND);
    *angular_velocity_right = angular_displacement_right / ((double)deltaTime / (double)TICKS_PER_SECOND);

    // Update previous values
    QuadDec_1_prevCounterValue = QuadDec_1_currentCounterValue;
    QuadDec_2_prevCounterValue = QuadDec_2_currentCounterValue;
    prevTime = currentTime;
}
/*
void compute_metrics(double* displacement, double* velocity) {
    static int16 QuadDec_1_prevCounterValue = 0;
    static int16 QuadDec_1_currentCounterValue = 0;
    static int16 QuadDec_2_prevCounterValue = 0;
    static int16 QuadDec_2_currentCounterValue = 0;
    static uint32 prevTime;
    
    uint32 currentTime = Cy_SysLib_GetTicks(); 
    uint32 deltaTime = currentTime - prevTime;
    
    // Calculate displacement for both wheels
    *displacement_1 = ((double)QuadDec_1_currentCounterValue / TICKS_PER_REVOLUTION) * 2 * PI * WHEEL_RADIUS;
    *displacement_2 = ((double)QuadDec_2_currentCounterValue / TICKS_PER_REVOLUTION) * 2 * PI * WHEEL_RADIUS;
    
    *displacement = ((double)currentCounterValue / TICKS_PER_REVOLUTION) * 2 * PI * WHEEL_RADIUS;
    *velocity = (double)(currentCounterValue - prevCounterValue) / TICKS_PER_REVOLUTION / (deltaTime / (float)TICKS_PER_SECOND);

    // Update previous values
    prevCounterValue = currentCounterValue;
    prevVelocity = *velocity;
    prevTime = currentTime;
}
*/
void motor_move(int mode, int direction, int speed) {
    motor_status(mode);
    
    if (!mode) {
        return;
    }
    
    motor_pwm_control(speed);

    if (direction == FORWARD || direction == BACKWARD) {
        MOTOR_1_IN_LEFT_Write(direction);
        MOTOR_1_IN_RIGHT_Write(!direction);

        MOTOR_2_IN_LEFT_Write(MOTOR_1_IN_LEFT_Read());
        MOTOR_2_IN_RIGHT_Write(MOTOR_1_IN_RIGHT_Read());
    } else {
        direction -= 2;
        MOTOR_1_IN_LEFT_Write(!direction);
        MOTOR_1_IN_RIGHT_Write(direction);

        MOTOR_2_IN_LEFT_Write(!MOTOR_1_IN_LEFT_Read());
        MOTOR_2_IN_RIGHT_Write(!MOTOR_1_IN_RIGHT_Read());
    }
}
