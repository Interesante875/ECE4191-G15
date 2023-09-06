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
    
    #define PWM_VAL 240
    
   
    void moveOutOfBaseFast();
    void moveOutOfBase();
    void detectSlit();
    void moveThroughSlit();
    
    
    void initBase();
    void findDeckPrelim();
    void adjustDistance();
    void prepMoveThroughSlit();
    
    void grabPuck();
    void placePuck();
    void flickPuck();
    void returnToBase();
    
    void test();
    
    
#endif


/* [] END OF FILE */
