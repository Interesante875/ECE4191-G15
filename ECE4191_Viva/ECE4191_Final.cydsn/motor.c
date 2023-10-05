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
#include <project.h>
#include "motor.h"

/* Left Quadrature Decoder */
void MotorController_StartLeftQuadDec() {
    QuadDec_LEFT_Start();
}
void MotorController_StopLeftQuadDec() {
    QuadDec_LEFT_Stop();
}
void MotorController_ResetLeftQuadDec() {
    QuadDec_LEFT_SetCounter(0);   
}
int MotorController_GetLeftQuadDecCount() {
    int counter = QuadDec_LEFT_GetCounter();   
    return counter;
}
void MotorController_SetLeftQuadDecCount(int count) {
    QuadDec_LEFT_SetCounter(count); 
}

/* Right Quadrature Decoder */
void MotorController_StartRightQuadDec() {
    QuadDec_RIGHT_Start();
}
void MotorController_StopRightQuadDec() {
    QuadDec_RIGHT_Stop();
}
void MotorController_ResetRightQuadDec() {
    QuadDec_RIGHT_SetCounter(0);
}
int MotorController_GetRightQuadDecCount() {
    int counter = QuadDec_RIGHT_GetCounter();   
    return counter;
}
void MotorController_SetRightQuadDecCount(int count) {
    QuadDec_RIGHT_SetCounter(count); 
}

/* PWM Wheel Control */

void MotorController_StartPwm() {
    PWM_Wheel_Start();   
}
void MotorController_StopPwm() {
    PWM_Wheel_Stop();
}

void MotorController_SetLeftPwmCompare(int compareValue) {
    
    if (compareValue > PWM_MAX) compareValue = PWM_MAX;
    if (compareValue < PWM_MIN) compareValue = PWM_MIN;
    
    PWM_Wheel_WriteCompare1(compareValue);   
}
int MotorController_GetLeftPwm() {
    
    int pwmValue = PWM_Wheel_ReadCompare1();
    return pwmValue;
}
void MotorController_SetRightPwmCompare(int compareValue) {
    if (compareValue > PWM_MAX) compareValue = PWM_MAX;
    if (compareValue < PWM_MIN) compareValue = PWM_MIN;
    
    PWM_Wheel_WriteCompare2(compareValue);     
}
int MotorController_GetRightPwm() {
    int pwmValue = PWM_Wheel_ReadCompare2();
    return pwmValue;   
}
/* [] END OF FILE */
