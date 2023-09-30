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
#include "servo.h"
#include <cytypes.h>

void turnOn_GripperHand() {
    PWM_Servo_Gripper_Start();
}
void turnOff_GripperHand() {
    PWM_Servo_Gripper_Stop();   
}
void GripperHand_SetCompare(int compareValue) {
    PWM_Servo_Gripper_WriteCompare1(compareValue); 
}
int GripperHand_ReadCompare() {
    return PWM_Servo_Gripper_ReadCompare1(); 
}

void turnOn_GripperArm() {
    PWM_Servo_Gripper_Start();   
}
void turnOff_GripperArm() {
    PWM_Servo_Gripper_Stop();   
}
void GripperArm_SetCompare(int compareValue) {
    PWM_Servo_Gripper_WriteCompare2(compareValue);    
}
int GripperArm_ReadCompare() {
    return PWM_Servo_Gripper_ReadCompare2(); 
}

void turnOn_FlickerRecoil() {
    PWM_Servo_Flicker_Start();   
}
void turnOff_FlickerRecoil() {
    PWM_Servo_Flicker_Stop();
}
void FlickerRecoil_SetCompare(int compareValue) {
    PWM_Servo_Flicker_WriteCompare1(compareValue);
}
int FlickerRecoil_ReadCompare() {
    return PWM_Servo_Flicker_ReadCompare1(); 
}

void turnOn_FlickerLock() {
    PWM_Servo_Flicker_Start();    
}
void turnOff_FlickerLock() {
    PWM_Servo_Flicker_Stop();   
}
void FlickerLock_SetCompare(int compareValue) {
    PWM_Servo_Flicker_WriteCompare2(compareValue);   
}
int FlickerLock_ReadCompare() {
    return PWM_Servo_Flicker_ReadCompare2(); 
}


/* [] END OF FILE */
