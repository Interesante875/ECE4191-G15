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
#include "ultrasonic.h"
#include "bluetooth.h"
#include "locomotion.h"

int main(void)
{
    CyGlobalIntEnable; /* Enable global interrupts. */
    ultrasonic_setup();
    
    
    
    //wheel_turn_by_angle(LEFT, 240, 90);
    /* Place your initialization/startup code here (e.g. MyInst_Start()) */
    for(;;)
    {
        ultrasonic_measuring();
        
        if (kaldist_measure[0] < 5) {
            Indicator_RED_Write(1);        
        } else {
            Indicator_RED_Write(0);
        }
        
        

    }
}

/* [] END OF FILE */
