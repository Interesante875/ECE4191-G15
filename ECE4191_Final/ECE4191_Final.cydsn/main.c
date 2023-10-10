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

int main(void)
{
    CyGlobalIntEnable; /* Enable global interrupts. */
    initializeBluetooth();
    InitalizeUltrasonicSensor();
    
    // test_run();
    
    for(;;)
    {
        
        for (int i = 0; i < 8; i++) {
            read_U();
            print_U();
            printValue("%.2lf: %.2lf ", BLU, BRU);
            printValue("%.2lf: %.2lf\n", RBU, LBU);
            CyDelay(50);
        }
        // printValue("\n");
        CyDelay(3000);
        
    }
}

/* [] END OF FILE */
