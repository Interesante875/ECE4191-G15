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
#include "bluetooth.h"

int pinInWhichZone;
TargetPuckColor targetPuckColor;
double distanceToPin;
int zoneCount[6];

void startingBowlingPinWhichStripe() {
    
    int zone = 0;
    
    if (start_base_color == YellowBase || start_base_color == BlueBase) {
        UltrasonicSensor_ChangeState(UdsDetectAll);
    } else {
        UltrasonicSensor_ChangeState(UdsDetectAll);
    }
    
    static double BU[8] = {0};
    static double BU_count[8] = {0};
    
    for (int j = 0; j < 8; j++) {
        BU_count[j] += 1;
        BU[j] = UltrasonicSensor_ReadDistanceData(j);
        CyDelay(10);
    }
    
    for (int j = 0; j < 8; j++) {
        printValue("(%d): %lf ", j, BU[j]);   
    }
    
    printValue("\n");
    
    // Which one is possible?
    int detectedPin[8] = {0};
    
    for (int j = 0; j < 8; j++) {
        if (BU[j] <= 5 || BU[j] >= 60) {
            detectedPin[j] = 0;
        } else {
            detectedPin[j] = 1;
        }
        
        if (detectedPin[j]) {
            double a = BU[j];
            if (a >= 7.5 && a < 12.5) // Zone 1
            {
                pinInWhichZone = 1;
                targetPuckColor = BluePuck;
            }
            else if (a >= 12.5 && a < 17.5) // Zone 2
            {
                pinInWhichZone = 2;
                targetPuckColor = GreenPuck;   
            }
            else if (a >= 17.5 && a < 22.5) // Zone 3
            {
                pinInWhichZone = 3;
                targetPuckColor = RedPuck;   
            }
            else if (a >= 22.5 && a < 27.5) // Zone 4
            {
                pinInWhichZone = 4;
                targetPuckColor = BluePuck;   
            }
            else if (a >= 27.5 && a < 32.5) // Zone 5
            {
                pinInWhichZone = 5;
                targetPuckColor = GreenPuck;   
            }
            else if (a >= 32.5 && a < 37.5) // Zone 6
            {
                pinInWhichZone = 6;
                targetPuckColor = RedPuck;   
            }
            
            printValue("UDS: %d at ZONE: %d @ %lf\n", j, pinInWhichZone, a);
            
            break;
        }
    }

    
    

}

/* [] END OF FILE */
