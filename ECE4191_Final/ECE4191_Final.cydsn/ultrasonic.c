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
#include "ultrasonic.h"
#include "median_filter.h"
#include "bluetooth.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

int ultrasonicSensorIndex_FB;
int ultrasonicSensorIndex_LR;

UdsDetectState udsDetectState;
int count;
double distance;
double median;
double sensorMeasuredDistances[NUM_ULTRASONIC_SENSORS][ARRAY_SIZE];
static int currIdx[NUM_ULTRASONIC_SENSORS];

// Function prototypes
void InitalizeUltrasonicSensor() {
        
    Timer_Ultrasonic_Burst_Start();
    Timer_Ultrasonic_Echo_FB_Start();    
    Timer_Ultrasonic_Echo_LR_Start();
    
    isr_ultrasonic_burst_StartEx(ISR_Handler_Ultrasonic_Burst);
    isr_ultrasonic_echo_FB_StartEx(ISR_Handler_Ultrasonic_Echo_FB);
    isr_ultrasonic_echo_LR_StartEx(ISR_Handler_Ultrasonic_Echo_LR);
    
    for (int i = 0; i < NUM_ULTRASONIC_SENSORS; i++) {
        currIdx[i] = 0;
    }
    for (int i = 0; i < NUM_ULTRASONIC_SENSORS; i++) {
        for (int j = 0; j < ARRAY_SIZE; j++) {
            sensorMeasuredDistances[i][j] = 0;
        }
    }
    
}

void ShutdownUltrasonicSensor() {

    Timer_Ultrasonic_Burst_Stop(); 
    Timer_Ultrasonic_Echo_FB_Stop();    
    Timer_Ultrasonic_Echo_LR_Stop();
    isr_ultrasonic_burst_Stop();
    isr_ultrasonic_echo_FB_Stop();
    isr_ultrasonic_echo_LR_Stop(); 
    
}


double UltrasonicSensor_ReadDistanceData(int sensorIndex) {
    
    
    #if ENABLE_MEDIAN_FILTERING == 1
        double* rowPointer = sensorMeasuredDistances[sensorIndex];
        median = findMedian(rowPointer, ARRAY_SIZE);
        // printValue("UDS(%d) %lf\n", sensorIndex, median);
        return median;
    #endif
    
    
    double value = sensorMeasuredDistances[sensorIndex][0];
    // printValue("Read: (%d) %lf\n", sensorIndex, value);
    return value;

}


void UltrasonicSensor_SelectSensor(int sensorIndex) {
    
    if (sensorIndex < 4) {
        Control_Reg_Ultrasonic_FB_Write(sensorIndex);
        ultrasonicSensorIndex_FB = sensorIndex;
    } else {
        Control_Reg_Ultrasonic_LR_Write(sensorIndex);  
        ultrasonicSensorIndex_LR = sensorIndex;
    }
    
    CyDelayUs(2); 
}

void UltrasonicSensor_MeasureDistance(int sensorIndex) {

    if (sensorIndex < 4)
        count = Timer_Ultrasonic_Echo_FB_ReadCounter();
    else
        count = Timer_Ultrasonic_Echo_LR_ReadCounter();
        
    distance = (double) (65536 - count)/58.0;
    
    // printValue("(%d): %d %lf\n", sensorIndex, count, distance);
    #if ENABLE_MEDIAN_FILTERING
        sensorMeasuredDistances[sensorIndex][currIdx[sensorIndex]] = distance;
    
        // printValue("(%d): CurrIndex: %d %lf, ", sensorIndex, currIdx[sensorIndex], sensorMeasuredDistances[sensorIndex][currIdx[sensorIndex]]);
        
        currIdx[sensorIndex] = (currIdx[sensorIndex] + 1) % ARRAY_SIZE;
        
        // printValue("Next Index: %d\n", currIdx[sensorIndex]);
    #else
        sensorMeasuredDistances[sensorIndex][0] = distance;    
    #endif
    
}

CY_ISR(ISR_Handler_Ultrasonic_Burst) {
    Timer_Ultrasonic_Burst_ReadStatusRegister();
    Trigger_FB_Write(1);
    Trigger_LR_Write(1);
    CyDelayUs(10);
    Trigger_FB_Write(0);
    Trigger_LR_Write(0);
}

CY_ISR(ISR_Handler_Ultrasonic_Echo_FB) {
    Timer_Ultrasonic_Echo_FB_ReadStatusRegister();
    UltrasonicSensor_MeasureDistance(ultrasonicSensorIndex_FB);
    
    ultrasonicSensorIndex_FB = (ultrasonicSensorIndex_FB + 1) % (NUM_ULTRASONIC_SENSORS/2);
    UltrasonicSensor_SelectSensor(ultrasonicSensorIndex_FB);
}


CY_ISR(ISR_Handler_Ultrasonic_Echo_LR) {
    
    Timer_Ultrasonic_Echo_LR_ReadStatusRegister();
    UltrasonicSensor_MeasureDistance(ultrasonicSensorIndex_LR + 4);
    
    ultrasonicSensorIndex_LR = (ultrasonicSensorIndex_LR + 1) % (NUM_ULTRASONIC_SENSORS/2);
    UltrasonicSensor_SelectSensor(ultrasonicSensorIndex_LR + 4);
}

/* [] END OF FILE */
