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
#include "servo_control.h"

int main(void)
{
    CyGlobalIntEnable; /* Enable global interrupts. */

    /* Place your initialization/startup code here (e.g. MyInst_Start()) */
    
    initializeBluetooth();
    CyDelay(1000);
    FlickerRecoil_Unload();
    for(;;)
    {
      
    }
}

/* [] END OF FILE */
