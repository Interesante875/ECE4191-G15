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
#include "color_detection.h"

int main(void)
{
    CyGlobalIntEnable; /* Enable global interrupts. */
    
    
    for(;;)
    {
        color_sensor_start();
        color_detection_run(COLOR_DETECTION_RUNS);
        color_sensor_stop();
        
        switch(detectedColor) {
            case RED:
                Indicator_RED_Write(1); 
                break;
            case BLUE:
                Indicator_BLUE_Write(1);
                break;
            case GREEN:
                Indicator_GREEN_Write(1);
                break;
            default:
                Indicator_RED_Write(1);
                CyDelay(500);
                Indicator_RED_Write(0);
                Indicator_BLUE_Write(1);
                CyDelay(500);
                Indicator_BLUE_Write(0);
                Indicator_GREEN_Write(1);
                CyDelay(500);
                break;
        }
        
 
  
        Indicator_RED_Write(0);
        Indicator_BLUE_Write(0);
        Indicator_GREEN_Write(0);
        
        CyDelay(1000);
        
    }
}

/* [] END OF FILE */
