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
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "ultrasonic.h"
/*
#include "ultrasonic.h"
#include "bluetooth.h"
#include "locomotion.h"
#include "color_detection.h"
#include "servo.h"
*/
#include "servo.h"
#include "locomotion.h"
#include "color_detection.h"

#include "prelim.h"

int main(void)
{
    CyGlobalIntEnable; /* Enable global interrupts. */

//    moveOutOfBase();
//    CyDelay(500);
//    detectSlit();
//    moveThroughSlit();
    trunk_up();
    gripper_close();
    lift_down();
    gripper_open();
    
    wheel_move_by_metrics(FORWARD, 240, 0.25);
    gripper_close();
    
    trunk_middle();
    
    color_sensor_start();
    set_frequency_scaling(1, 1);
    color_detection_run(COLOR_DETECTION_RUNS);
        
    switch(detectedColor) {
        case RED:
            Indicator_RED_Write(1); 
            CyDelay(2000);
            break;
        case BLUE:
            Indicator_BLUE_Write(1);
            CyDelay(2000);
            break;
        case GREEN:
            Indicator_GREEN_Write(1);
            CyDelay(2000);
            break;
        default:
            Indicator_RED_Write(0);
            Indicator_BLUE_Write(0);
            Indicator_GREEN_Write(0);
            CyDelay(1000);
            break;
    }
    
    color_sensor_stop();
    
    trunk_up();
    
    lift_up();
    
    lift_down();
    
    gripper_open();
    
    wheel_move_by_metrics(BACKWARD, 240, 0.1);
    
    gripper_close();
    
    lift_up();
    
    arm_swing_flick();
    
    wheel_move_by_metrics(FORWARD, 240, 0.2);
    
    arm_swing_flat();
    
    gripper_close();
    
    lift_down();
    
    
//    gripper_open();
//    CyDelay(250);
//    gripper_close();
//        CyDelay(1000);
    
    //ultrasonic_on();
    for(;;)
    {    
//        if (kaldist_measure[0] > 75) Indicator_BLUE_Write(1);
//        else Indicator_BLUE_Write(0);
//        
//        if (kaldist_measure[1] > 75) Indicator_RED_Write(1);
//        else Indicator_RED_Write(0);
//        
//        CyDelay(50);
    }
}

/* [] END OF FILE */
