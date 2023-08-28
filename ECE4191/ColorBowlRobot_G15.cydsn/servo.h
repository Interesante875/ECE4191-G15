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
#ifndef SERVO_H
#define SERVO_H
    #include "project.h"
    #include <stdio.h>  
    
    void gripper_start();
    void arm_start();
    void lift_start();
    void trunk_start();
    
    void gripper_off();
    void arm_off();
    void lift_off();
    void trunk_off();
    
    // Simple manuevers
    void gripper_open();
    void gripper_close();
    void arm_swing(int angle);
    void lift_up();
    void lift_down();
    void trunk_up();
    void trunk_puck();
    void trunk_ground();
    
    // Set manuevers
    void grab_puck();
    void detect_ground_color();
    void detect_puck_color();
    
#endif 
/* [] END OF FILE */
