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
#include "ultrasonic.h"
#include "ultrasonic_control.h"
#include "locomotion.h"
#include "colorsensor.h"
#include "servo_control.h"
#include "irsensor.h"
#include "gyroscope.h"
#include "navigation.h"
#include "behavior_tree.h"

int main(void)
{
    CyGlobalIntEnable; /* Enable global interrupts. */

    StartingBase base_color = RedBase;
    initializePosition(base_color);
    initializeBluetooth();
    InitalizeUltrasonicSensor();
//    startGyroscope();
//    CyDelay(1000);
//    
//    wheel_move_by_metrics(Forward, 220, 0.5);
    
    // UltrasonicSensor_ChangeState(UdsDetectRight);
    double val;
    for(;;)
    {
        
        for (int i = 0; i < 8; i++) {
            val = UltrasonicSensor_ReadDistanceData(i);
            printValue("%d: %.2lf ", i, val);
            CyDelay(50);
        }

        printValue("\n");
        
        CyDelay(2000);
    }
}

/* [] END OF FILE */
