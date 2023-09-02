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
#ifndef PRELIM_H
    #define PRELIM_H
    #include "project.h"
    
    #include <stdio.h>
    #include <stdlib.h>
    #include <cytypes.h>
    #include <stdbool.h>
    
    #include "locomotion.h"
    #include "ultrasonic.h"
    #include "servo.h"
    #include "color_detection.h"

    
//    #include "color_detection.h"
//    #include "locomotion.h"
//    #include "irsensor.h"
//    #include "servo.h"
//    #include "ultrasonic.h"
    
   
    void moveOutOfBaseFast();
    void moveOutOfBase();
    void detectSlit();
    void moveThroughSlit();
    
    
#endif


/* [] END OF FILE */
