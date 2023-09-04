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

#include "prelim.h"

int main(void)
{
    CyGlobalIntEnable; /* Enable global interrupts. */

    moveOutOfBase();
    CyDelay(500);
    detectSlit();
    moveThroughSlit();
 
    
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
