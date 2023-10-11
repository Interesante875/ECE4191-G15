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
#include <math.h>
#include "bluetooth.h"
#include "trialRun.h"
#include "ultrasonic.h"
#include "ultrasonic_control.h"
#include "motor.h"
int main(void)
{
    CyGlobalIntEnable; /* Enable global interrupts. */


    initializeRobot();    
    test_run();

    for(;;)
    {
        
    }
}

/* [] END OF FILE */
