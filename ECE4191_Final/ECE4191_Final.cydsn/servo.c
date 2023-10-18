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
    PWM_Servo_Gripper_Hand_Start();
}
void turnOff_GripperHand() {
    PWM_Servo_Gripper_Hand_Stop();   
}
void GripperHand_SetCompare(int compareValue) {
    PWM_Servo_Gripper_Hand_WriteCompare(compareValue); 
}
int GripperHand_ReadCompare() {
    return PWM_Servo_Gripper_Hand_ReadCompare(); 
}

void turnOn_GripperArm() {
    PWM_Servo_Gripper_Arm_Start();   
}
void turnOff_GripperArm() {
    PWM_Servo_Gripper_Arm_Stop();   
}
void GripperArm_SetCompare(int compareValue) {
    PWM_Servo_Gripper_Arm_WriteCompare(compareValue);    
}
int GripperArm_ReadCompare() {
    return PWM_Servo_Gripper_Arm_ReadCompare(); 
}

void turnOn_FlickerRecoil() {
    PWM_Servo_Flicker_Load_Start();   
}
void turnOff_FlickerRecoil() {
    FlickerRecoil_SetCompare(20);
    PWM_Servo_Flicker_Load_Stop();
}
void FlickerRecoil_SetCompare(int compareValue) {
    PWM_Servo_Flicker_Load_WriteCompare(compareValue);
}
int FlickerRecoil_ReadCompare() {
    return PWM_Servo_Flicker_Load_ReadCompare(); 
}

void turnOn_FlickerLock() {
    PWM_Servo_Flicker_Lock_Start();    
}
void turnOff_FlickerLock() {
    PWM_Servo_Flicker_Lock_Stop();   
}
void FlickerLock_SetCompare(int compareValue) {
    PWM_Servo_Flicker_Lock_WriteCompare(compareValue);   
}
int FlickerLock_ReadCompare() {
    return PWM_Servo_Flicker_Lock_ReadCompare(); 
}


/* [] END OF FILE */
