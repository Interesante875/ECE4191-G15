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

int ultrasonicSensorIndex_Front;
int ultrasonicSensorIndex_Back;

UdsDetectState udsDetectState;
int count;
double distance;
double median;
double sensorMeasuredDistances[NUM_ULTRASONIC_SENSORS][ARRAY_SIZE];
static int currIdx[NUM_ULTRASONIC_SENSORS];

// Function prototypes
void InitalizeUltrasonicSensor() {
        
    PWM_Ultrasonic_Burst_Front_Start();
    PWM_Ultrasonic_Burst_Back_Start();
    Timer_Ultrasonic_Echo_Front_Start();    
    Timer_Ultrasonic_Echo_Back_Start();
    
    isr_ultrasonic_echo_front_StartEx(ISR_Handler_Ultrasonic_Echo_Front);
    isr_ultrasonic_echo_back_StartEx(ISR_Handler_Ultrasonic_Echo_Back);
    
    for (int i = 0; i < NUM_ULTRASONIC_SENSORS; i++) {
        currIdx[i] = 0;
    }
    for (int i = 0; i < NUM_ULTRASONIC_SENSORS; i++) {
        for (int j = 0; j < ARRAY_SIZE; j++) {
            sensorMeasuredDistances[i][j] = 0;
        }
    }
    ultrasonicSensorIndex_Front = 0;
    ultrasonicSensorIndex_Back = 0;
}

void ShutdownUltrasonicSensor() {

    Timer_Ultrasonic_Echo_Front_Stop();    
    Timer_Ultrasonic_Echo_Back_Stop();
    
    isr_ultrasonic_echo_front_Stop();
    isr_ultrasonic_echo_back_Stop(); 
    
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
        
        Control_Reg_Ultrasonic_Front_Write(sensorIndex);
        ultrasonicSensorIndex_Front = sensorIndex;
    } else {
        
        Control_Reg_Ultrasonic_Back_Write(sensorIndex - 4);  
        ultrasonicSensorIndex_Back = sensorIndex - 4;
    }
    
    CyDelayUs(5); 
}

void UltrasonicSensor_MeasureDistance(int sensorIndex) {

    if (sensorIndex < 4)
        count = Timer_Ultrasonic_Echo_Front_ReadCounter();
    else
        count = Timer_Ultrasonic_Echo_Back_ReadCounter();
        
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


CY_ISR(ISR_Handler_Ultrasonic_Echo_Front) {
    Timer_Ultrasonic_Echo_Front_ReadStatusRegister();
    // printValue("FRONT %d %d\n", ultrasonicSensorIndex_Front, ultrasonicSensorIndex_Back + 4);
    UltrasonicSensor_MeasureDistance(ultrasonicSensorIndex_Front);
    
    ultrasonicSensorIndex_Front = (ultrasonicSensorIndex_Front + 1) % (NUM_ULTRASONIC_SENSORS/2);
    UltrasonicSensor_SelectSensor(ultrasonicSensorIndex_Front);
}


CY_ISR(ISR_Handler_Ultrasonic_Echo_Back) {
    
    Timer_Ultrasonic_Echo_Back_ReadStatusRegister();
    // printValue("BACK %d %d\n", ultrasonicSensorIndex_Front, ultrasonicSensorIndex_Back + 4);
    UltrasonicSensor_MeasureDistance(ultrasonicSensorIndex_Back + 4);
    
    ultrasonicSensorIndex_Back = (ultrasonicSensorIndex_Back + 1) % (NUM_ULTRASONIC_SENSORS/2);
    UltrasonicSensor_SelectSensor(ultrasonicSensorIndex_Back + 4);
}

/* [] END OF FILE */
