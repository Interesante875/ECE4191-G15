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


double FLU, FRU, LFU, RFU, BLU, BRU, LBU, RBU;





int pinInWhichZone;
TargetPuckColor targetPuckColor;
double distanceToPin;
int zoneCount[6];

void read_U() {
    FLU = UltrasonicSensor_ReadDistanceData(0);  
    FRU = UltrasonicSensor_ReadDistanceData(1); 
    LFU = UltrasonicSensor_ReadDistanceData(2); 
    LBU = UltrasonicSensor_ReadDistanceData(6);
    RFU = UltrasonicSensor_ReadDistanceData(3); 
    RBU = UltrasonicSensor_ReadDistanceData(7);
    BLU = UltrasonicSensor_ReadDistanceData(4);
    BRU = UltrasonicSensor_ReadDistanceData(5);
}

void read_FU() {
    FLU = UltrasonicSensor_ReadDistanceData(0);  
    FRU = UltrasonicSensor_ReadDistanceData(1); 
}

void read_LU() {
    LFU = UltrasonicSensor_ReadDistanceData(2); 
    LBU = UltrasonicSensor_ReadDistanceData(6);  
}

void read_RU() {
    RFU = UltrasonicSensor_ReadDistanceData(3); 
    RBU = UltrasonicSensor_ReadDistanceData(7);
}

void read_BU() {
    BLU = UltrasonicSensor_ReadDistanceData(4);
    BRU = UltrasonicSensor_ReadDistanceData(5);
}

void read_FLU() {
    FLU = UltrasonicSensor_ReadDistanceData(0);   
}
void read_FRU() {
    FRU = UltrasonicSensor_ReadDistanceData(1);   
}
void read_LFU() {
    LFU = UltrasonicSensor_ReadDistanceData(2);   
}
void read_RFU() {
    RFU = UltrasonicSensor_ReadDistanceData(3);   
}
void read_BLU() {
    BLU = UltrasonicSensor_ReadDistanceData(4);   
}
void read_BRU() {
    BRU = UltrasonicSensor_ReadDistanceData(5);   
}
void read_LBU() {
    LBU = UltrasonicSensor_ReadDistanceData(6);   
}
void read_RBU() {
    RBU = UltrasonicSensor_ReadDistanceData(7);   
}

void print_U() {
    for (int i = 0; i < 8; i++) {
        printValue("%d: %.2lf ", i, UltrasonicSensor_ReadDistanceData(i));
    }
    printValue("\n");
}


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
