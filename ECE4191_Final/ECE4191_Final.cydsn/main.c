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
    UltrasonicSensor_ChangeState(UdsDetectLeft);
    CyDelay(200);
    
    double LF, LB;
    LF = UltrasonicSensor_ReadDistanceData(4);
    LB = UltrasonicSensor_ReadDistanceData(5);
    
    wheel_move(Forward, 225);
    CyDelay(500);
    bool notSeen = true;
    
    while (notSeen) {
        LF = UltrasonicSensor_ReadDistanceData(4);
        LB = UltrasonicSensor_ReadDistanceData(5);
        
//        printValue("%lf, %lf\n", LF, LB);
        notSeen = fabs(LF - LB) <= 10 && LB < LF;
    }
    
    wheel_move(StopMotion, 200);
    
    wheel_move_by_metrics(Left, 250, 90);

    for(;;)
    {

        
    }
}

/* [] END OF FILE */
