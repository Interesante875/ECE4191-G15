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
#include "motor_control.h"

MOVE_STATUS moveStatus = ENABLE;
MOTION currentMotion = STOP;

void motor_on(uint8 pwm) {
    
    moveStatus = ENABLE;
    
    motor_left_start_QuadDec();
    motor_right_start_QuadDec();
    
    motor_left_start_pwm();
    motor_right_start_pwm();
    
    motor_left_set_pwm_compare(pwm);
    motor_right_set_pwm_compare(pwm);
    
}

void motor_off() {
    
    moveStatus = DISABLE;
    
    motor_left_stop_pwm();
    motor_right_stop_pwm();
    
    motor_left_reset_QuadDec();
    motor_right_reset_QuadDec();
    
    motor_left_stop_QuadDec();
    motor_right_stop_QuadDec();
    
    motor_motion_set(STOP);
    
}

void motor_start() {
    
    moveStatus = ENABLE;
    
    motor_left_reset_QuadDec();
    motor_right_reset_QuadDec();
    
}


void motor_stop() {
    
    moveStatus = DISABLE;
 
    motor_left_reset_QuadDec();
    motor_right_reset_QuadDec();

    motor_motion_set(STOP);
    
    isr_wheel_controller_Stop();
}


void motor_motion_set(MOTION motion) {
      
    currentMotion = motion;

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
    
    CyDelay(10);
    
}
/* [] END OF FILE */
