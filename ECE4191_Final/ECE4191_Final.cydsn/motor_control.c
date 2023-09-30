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
#include "motor.h"

MoveStatus moveStatus;
MotionDirection currentMotionDirection;

    
void turnMotorOn(uint8 pwm) {
    
    moveStatus = Enable;
    
    MotorController_StartLeftQuadDec();
    MotorController_StartRightQuadDec();
    
    MotorController_StartPwm();
    
    MotorController_SetLeftPwmCompare(pwm);
    MotorController_SetRightPwmCompare(pwm);
    
}

void turnMotorOff() {
    
    moveStatus = Disable;
    
    MotorController_StopPwm();

    MotorController_ResetLeftQuadDec();
    MotorController_ResetRightQuadDec();
    
    MotorController_StopLeftQuadDec();
    MotorController_StopRightQuadDec();
    
    setMotionDirection(StopMotion);
    
}

void startMotor() {
    
    moveStatus = Enable;
    
    MotorController_ResetLeftQuadDec();
    MotorController_ResetRightQuadDec();
    
}


void stopMotor() {
    
    moveStatus = Disable;
 
    MotorController_ResetLeftQuadDec();
    MotorController_ResetRightQuadDec();

    setMotionDirection(StopMotion);
    
}


void setMotionDirection(MotionDirection motionDirection) {
      
    currentMotionDirection = motionDirection;

    switch (motionDirection) {
        case Forward:
            MOTOR_LEFT_IN_1_Write(1);
            MOTOR_LEFT_IN_2_Write(0);
            
            MOTOR_RIGHT_IN_3_Write(1);
            MOTOR_RIGHT_IN_4_Write(0);
            break;
        case Backward:
            MOTOR_LEFT_IN_1_Write(0);
            MOTOR_LEFT_IN_2_Write(1);
            
            MOTOR_RIGHT_IN_3_Write(0);
            MOTOR_RIGHT_IN_4_Write(1);
            break;
        case Left:
            
            MOTOR_LEFT_IN_1_Write(1);
            MOTOR_LEFT_IN_2_Write(0);
            
            MOTOR_RIGHT_IN_3_Write(0);
            MOTOR_RIGHT_IN_4_Write(1);

            break;
        case Right:
            
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
