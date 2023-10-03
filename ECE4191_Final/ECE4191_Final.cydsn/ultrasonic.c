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

int ultrasonicSensorIndex;
UdsDetectState udsDetectState;
int count;
double distance;
double median;
double sensorMeasuredDistances[NUM_ULTRASONIC_SENSORS][ARRAY_SIZE] = {0};
static int currIdx[NUM_ULTRASONIC_SENSORS] = {0};

// Function prototypes
void InitalizeUltrasonicSensor() {
    Timer_Ultrasonic_Burst_Start();
    Timer_Ultrasonic_Echo_Start();
    
    isr_ultrasonic_burst_StartEx(ISR_Handler_Ultrasonic_Burst);
    isr_ultrasonic_echo_StartEx(ISR_Handler_Ultrasonic_Echo);
    
    ultrasonicSensorIndex = 0;
    udsDetectState = UdsDetectAll;
}

void ShutdownUltrasonicSensor() {
    Timer_Ultrasonic_Burst_Stop();
    Timer_Ultrasonic_Echo_Stop();
    
    isr_ultrasonic_burst_Stop();
    isr_ultrasonic_echo_Stop();
}

void UltrasonicSensor_ChangeState(UdsDetectState state) {
    
    udsDetectState = state;
    
    // change ultrasonicSensorIndex
    switch (state) {
        case UdsDetectAll:
            UltrasonicSensor_SelectSensor(0);
        break;
        
        case UdsDetectBack:
            UltrasonicSensor_SelectSensor(2);
        break;
        
        case UdsDetectLeft:
            UltrasonicSensor_SelectSensor(4);
        break;
        
        case UdsDetectRight:
            UltrasonicSensor_SelectSensor(6);
        break;
        
        case UdsDetectFront:
            UltrasonicSensor_SelectSensor(0);
        break;
        
        case UdsDetectFLR:
            UltrasonicSensor_SelectSensor(0);
        break;
        
        case UdsDetectBLR:
            UltrasonicSensor_SelectSensor(2);
        break;
            
        case UdsDetectLR:
            UltrasonicSensor_SelectSensor(4);
        break;
        
        default:
            UltrasonicSensor_SelectSensor(0);
        break;
        
    }
    
}

void UltrasonicSensor_TriggerBurst() {
    Trigger_Write(1);
    CyDelayUs(10);
    Trigger_Write(0);   
}

double UltrasonicSensor_ReadDistanceData(int sensorIndex) {
    double* rowPointer = sensorMeasuredDistances[sensorIndex];
    
    if (ENABLE_MEDIAN_FILTERING) {
        median = findMedian(rowPointer, ARRAY_SIZE);
        // printValue("UDS(%d) %d\n", sensorIndex, (int) median);
        return median;
    }
    
    double value = rowPointer[currIdx[sensorIndex]];
    printValue("%d, %lf\n", sensorIndex, value);
    return value;

}

void UltrasonicSensor_SelectSensor(int sensorIndex) {
    Control_Reg_Ultrasonic_Write(sensorIndex);
    CyDelayUs(10);
    ultrasonicSensorIndex = Control_Reg_Ultrasonic_Read();
    CyDelayUs(2);   
}

void UltrasonicSensor_MeasureDistance(int sensorIndex) {

    
    count = Timer_Ultrasonic_Echo_ReadCounter();
    
    distance = (double) (65535 - count)/58.0;
    
    // if (sensorIndex == 0) printValue("(0): %lf\n", distance);
    
    sensorMeasuredDistances[sensorIndex][currIdx[sensorIndex]] = distance;
    
    currIdx[sensorIndex] = (currIdx[sensorIndex] + 1) % ARRAY_SIZE;

}

CY_ISR(ISR_Handler_Ultrasonic_Burst) {
    Timer_Ultrasonic_Burst_ReadStatusRegister();
    UltrasonicSensor_TriggerBurst();  
}

CY_ISR(ISR_Handler_Ultrasonic_Echo) {
    
    Timer_Ultrasonic_Echo_ReadStatusRegister();
    UltrasonicSensor_MeasureDistance(ultrasonicSensorIndex);
    
    // // change according to uds state
    
    switch (udsDetectState) {
        case UdsDetectAll:
            ultrasonicSensorIndex = (ultrasonicSensorIndex + 1) % NUM_ULTRASONIC_SENSORS; 
            UltrasonicSensor_SelectSensor(ultrasonicSensorIndex);
        break;
        
        case UdsDetectBack:
            if (ultrasonicSensorIndex == 3) ultrasonicSensorIndex = 2;
            else if (ultrasonicSensorIndex == 2) ultrasonicSensorIndex = 3;
            else ultrasonicSensorIndex = 2;
            UltrasonicSensor_SelectSensor(ultrasonicSensorIndex);
        break;
        
        case UdsDetectLeft:
            if (ultrasonicSensorIndex == 5) ultrasonicSensorIndex = 4;
            else if (ultrasonicSensorIndex == 4) ultrasonicSensorIndex = 5;
            else ultrasonicSensorIndex = 4;
            UltrasonicSensor_SelectSensor(ultrasonicSensorIndex);
        break;
        
        case UdsDetectRight:
            if (ultrasonicSensorIndex == 7) ultrasonicSensorIndex = 6;
            else if (ultrasonicSensorIndex == 6) ultrasonicSensorIndex = 7;
            else ultrasonicSensorIndex = 6;
            UltrasonicSensor_SelectSensor(ultrasonicSensorIndex);
        break;
        
        case UdsDetectFront:
            if (ultrasonicSensorIndex == 1) ultrasonicSensorIndex = 0;
            else if (ultrasonicSensorIndex == 0) ultrasonicSensorIndex = 1;
            else ultrasonicSensorIndex = 0;
            UltrasonicSensor_SelectSensor(ultrasonicSensorIndex);
        break;
        
        case UdsDetectFLR:
            ultrasonicSensorIndex = (ultrasonicSensorIndex + 1) % NUM_ULTRASONIC_SENSORS; 
            if (ultrasonicSensorIndex == 2) ultrasonicSensorIndex = 4;
            else if (ultrasonicSensorIndex == 3) ultrasonicSensorIndex = 4;
            UltrasonicSensor_SelectSensor(ultrasonicSensorIndex);
        break;
        
        case UdsDetectBLR:
            ultrasonicSensorIndex = (ultrasonicSensorIndex + 1) % NUM_ULTRASONIC_SENSORS; 
            if (ultrasonicSensorIndex == 0) ultrasonicSensorIndex = 2;
            else if (ultrasonicSensorIndex == 1) ultrasonicSensorIndex = 2;
            UltrasonicSensor_SelectSensor(ultrasonicSensorIndex);
        break;
            
        case UdsDetectLR:
            ultrasonicSensorIndex = (ultrasonicSensorIndex + 1) % NUM_ULTRASONIC_SENSORS; 
            if (ultrasonicSensorIndex - 3 <= 0) ultrasonicSensorIndex = 4;
            UltrasonicSensor_SelectSensor(ultrasonicSensorIndex);
        break;
        
        default:
            ultrasonicSensorIndex = (ultrasonicSensorIndex + 1) % NUM_ULTRASONIC_SENSORS; 
            UltrasonicSensor_SelectSensor(ultrasonicSensorIndex);
        break;
        
    }

}
/* [] END OF FILE */
