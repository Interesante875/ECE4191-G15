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
#include "bluetooth.h"
#include "ultrasonic.h"
#include "locomotion.h"
#include "colorsensor.h"

int main(void)
{
    CyGlobalIntEnable; /* Enable global interrupts. */

    /* Place your initialization/startup code here (e.g. MyInst_Start()) */
    
    initializeBluetooth();
    
    // InitalizeUltrasonicSensor();
    
    CyDelay(1000);
    wheel_move_by_ticks(Forward, 250, 2000);
    
    for(;;)
    {
        /* Place your application code here. */
//        double val = UltrasonicSensor_ReadDistanceData(0);
//        printValue("VAL: %d\n", (int) val);
    }
}

/* [] END OF FILE */
