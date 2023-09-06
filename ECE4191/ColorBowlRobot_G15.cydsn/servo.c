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

uint16 trunk_readCompare() {
    uint16 compareValue = PWM_Servo_Trunk_ReadCompare(); 
    return compareValue;
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
    CyDelay(500);
    gripper_off();
}

void gripper_full_close() {
    /* Need to test the value
    gripper_writeCompare(compareValue); 
    */
    
    gripper_start();
    gripper_writeCompare(879);
    CyDelay(500);
    // gripper_off();
}

void gripper_close() {
    /* Need to test the value
    gripper_writeCompare(compareValue); 
    */
    
    gripper_start();
    gripper_writeCompare(881);
    CyDelay(1000);
    //gripper_off();
}

void arm_swing_flat() {
    /*
    Translate angle to compareValue, requires mapping
    arm_writeCompare(compareValue);
    */
    
    arm_start();
    arm_writeCompare(875);
    CyDelay(1000);
    arm_off();

}

void arm_swing_down() {
    /*
    Translate angle to compareValue, requires mapping
    arm_writeCompare(compareValue);
    */
    
    arm_start();
    arm_writeCompare(876);
    CyDelay(1000);
    arm_off();

}

void arm_swing_flick() {
    /*
    Translate angle to compareValue, requires mapping
    arm_writeCompare(compareValue);
    */
    
    arm_start();
    arm_writeCompare(935);
    CyDelay(1000);
    arm_off();

}

void arm_shoot() {
    arm_start();
    arm_swing_flick();
    CyDelay(200);
    arm_swing_flat();
    arm_off();
}

void lift_down() {
    // lift_writeCompare(int compareValue)
    
    lift_start();
    lift_writeCompare(875);
    CyDelay(1600);
    lift_off();
}
void lift_up() {
    // lift_writeCompare(int compareValue)   
    lift_start();
    lift_writeCompare(980);
    CyDelay(1200);
    lift_off();
}

void lift_ctrl() {
    
    
}
void trunk_up() {
    //trunk_writeCompare(int compareValue)
    trunk_start();
    uint16 target = 920;
    trunk_writeCompare(target);
    CyDelay(500);
    trunk_off();
}
void trunk_middle() {
    //trunk_writeCompare(int compareValue)
    trunk_start();
    uint16 target = 875;
    trunk_writeCompare(target);
    CyDelay(500);
    //trunk_off();
}
void trunk_ground() {
    //trunk_writeCompare(int compareValue)   
    trunk_start();
    uint16 target = 870;
    trunk_writeCompare(target);
    CyDelay(500);
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
