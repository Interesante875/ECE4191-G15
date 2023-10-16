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
#ifndef LEVELS_H
    #define LEVELS_H
    
    #include "project.h"
    #include "navigation.h"
    #include "task.h"
    #include "p_ctrl.h"
    #include "pid_ctrl.h"
    #include "locomotion.h"
    #include "navigation.h"
    #include "ultrasonic.h"
    #include "ultrasonic_control.h"
    #include "bluetooth.h"
    #include "colorsensor.h"
    #include "gyroscope.h"
    #include "irsensor.h"
    #include "servo_control.h"
    #include "behavior_tree.h"
    #include "behavior.h"

    
    extern StartingBase base_color;
    extern Color requiredColor_L1;
    extern Color requiredColor_L2;
    extern Color requiredColor_L3;
    extern Color requiredColor_L4;

    extern int STATE_CURR_LEVEL;
    
    void initializeRobotBase();
    
#endif
/* [] END OF FILE */
