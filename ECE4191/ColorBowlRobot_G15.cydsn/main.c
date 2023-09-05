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
    

    initBase();

    moveOutOfBaseFast();
////    moveOutOfBase();
//    CyDelay(500);
    detectSlit();
    prepMoveThroughSlit();
    moveThroughSlit();
    grabPuck();
    findDeckPrelim();
    placePuck();
    flickPuck();
    returnToBase();
   
    for(;;)
    {    

    }
}

/* [] END OF FILE */
