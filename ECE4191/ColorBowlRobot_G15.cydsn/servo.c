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

void gripper_start() {
    PWM_Servo_Gripper_Start();
}
void arm_start() {
    PWM_Servo_Gripper_Arm_Start();   
}
void lift_start() {
    PWM_Servo_Lifter_Start();
}
void trunk_start() {
    PWM_Servo_Trunk_Start();   
}

void gripper_off() {
    PWM_Servo_Gripper_Stop();
}
void arm_off() {
    PWM_Servo_Gripper_Arm_Stop();  
}
void lift_off() {
    PWM_Servo_Lifter_Stop();
}
void trunk_off() {
    PWM_Servo_Trunk_Stop(); 
}

void gripper_writeCompare(int compareValue) {
    PWM_Servo_Gripper_WriteCompare(compareValue);   
}
void arm_writeCompare(int compareValue) {
    PWM_Servo_Gripper_Arm_WriteCompare(compareValue);   
}
void lift_writeCompare(int compareValue) {
    PWM_Servo_Lifter_WriteCompare(compareValue); 
}
void trunk_writeCompare(int compareValue) {
    PWM_Servo_Trunk_WriteCompare(compareValue); 
}

void gripper_open() {
    /*
        There are a few states that prohibits this actions
        1. Gripper forearm motor is OFF
        2. Lift is at MIDDLE or HIGH position
        3. Puck is ON (except when pin landing zone reached)
        4. Arm is at angles other than 0
    
        There are only two states that allows the opening of gripper forearm
        0. Gripper forearm motor is ON
        1. Lift is at LOW
        2. Trunk is at MIDDLE or HIGH
    */
    /* Need to test the value
    gripper_writeCompare(compareValue); 
    */
    
    gripper_start();
    gripper_writeCompare(980);
    CyDelay(250);
    //gripper_off();
}

void gripper_close() {
    /* Need to test the value
    gripper_writeCompare(compareValue); 
    */
    
    gripper_start();
    gripper_writeCompare(880);
    CyDelay(250);
    //gripper_off();
}

void arm_swing(int angle) {
    /*
    Translate angle to compareValue, requires mapping
    arm_writeCompare(compareValue);
    */
    
    
}

void lift_down() {
    // lift_writeCompare(int compareValue)
    
    lift_start();
    lift_writeCompare(880);
    CyDelay(2500);
    lift_off();
}
void lift_up() {
    // lift_writeCompare(int compareValue)   
    lift_start();
    lift_writeCompare(980);
    CyDelay(2500);
    lift_off();
}
void trunk_up() {
    //trunk_writeCompare(int compareValue)
    trunk_start();
    trunk_writeCompare(920);
    CyDelay(500);
    //trunk_off();
}
void trunk_middle() {
    //trunk_writeCompare(int compareValue)
    trunk_start();
    trunk_writeCompare(880);
    CyDelay(250);
    //trunk_off();
}
void trunk_ground() {
    //trunk_writeCompare(int compareValue)   
    trunk_start();
    trunk_writeCompare(875);
    CyDelay(250);
    //trunk_off();
}

void reset_to_initial_state() {
    //gripper_close()
    //arm_swing(up)
    //lift_up()
    //trunk_ground()
}

void set_to_state() {
    
}

void grab_puck() {
    //trunk_up()
    //gripper_open()
    //arm_swing(horizontal)
    //lift_down()
    //gripper_close()
    //lift_up()
}

void release_puck() {
    //trunk_up()
    //arm_swing(horizontal)
    //lift_down()
    //gripper_open()
    //lift_up()
    //gripper_close()
}

void detect_ground_color() {
    //trunk_ground()
}

void detect_puck_color() {
    //trunk_middle()
}


/* [] END OF FILE */
