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
#include "ultrasonic_control.h"
#include "ultrasonic.h"
#include "navigation.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

extern int pinInWhichZone;
extern TargetPuckColor targetPuckColor;

void bowlingPinWhichStripe() {
    
    int zone = 0;
    
    if (start_base_color == YellowBase || start_base_color == BlueBase) {
        UltrasonicSensor_ChangeState(UdsDetectLeft);
    } else {
        UltrasonicSensor_ChangeState(UdsDetectRight);
    }
    
    double BU;
    
    if (start_base_color == YellowBase || start_base_color == BlueBase) {
        BU = UltrasonicSensor_ReadDistanceData(5);
    } else {
        BU = UltrasonicSensor_ReadDistanceData(7);
    }
    
    if (BU >= 7.5 && BU < 12.5) // Zone 1
    {
        pinInWhichZone = 1;
        targetPuckColor = BluePuck;
    }
    else if (BU >= 12.5 && BU < 17.5) // Zone 2
    {
        pinInWhichZone = 2;
        targetPuckColor = GreenPuck;   
    }
    else if (BU >= 17.5 && BU < 22.5) // Zone 3
    {
        pinInWhichZone = 3;
        targetPuckColor = RedPuck;   
    }
    else if (BU >= 22.5 && BU < 27.5) // Zone 4
    {
        pinInWhichZone = 4;
        targetPuckColor = BluePuck;   
    }
    else if (BU >= 27.5 && BU < 32.5) // Zone 5
    {
        pinInWhichZone = 5;
        targetPuckColor = GreenPuck;   
    }
    else if (BU >= 32.5 && BU < 37.5) // Zone 6
    {
        pinInWhichZone = 6;
        targetPuckColor = RedPuck;   
    }

}

/* [] END OF FILE */
