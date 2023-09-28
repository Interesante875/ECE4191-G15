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

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

int ultrasonicSensorIndex;
UdsDetectState udsDetectState;
int count;
double distance;
double median;
double sensorMeasuredDistances[NUM_ULTRASONIC_SENSORS][ARRAY_SIZE] = {0};

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
}

void UltrasonicSensor_TriggerBurst() {
    Trigger_Write(1);
    CyDelayUs(10);
    Trigger_Write(0);   
}

double UltrasonicSensor_ReadDistanceData(int sensorIndex) {
    double* rowPointer = sensorMeasuredDistances[sensorIndex];
    
    median = findMedian(rowPointer, ARRAY_SIZE);
    return median;
}

void UltrasonicSensor_SelectSensor(int sensorIndex) {
    Control_Reg_Ultrasonic_Write(sensorIndex);
    CyDelayUs(10);
    ultrasonicSensorIndex = Control_Reg_Ultrasonic_Read();
    CyDelayUs(2);   
}

void UltrasonicSensor_MeasureDistance(int sensorIndex) {
    
    static int currIdx[NUM_ULTRASONIC_SENSORS] = {0};
    
    count = Timer_Ultrasonic_Echo_ReadCounter();
    distance = (65535 - count)/58.0;
    
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
    
    ultrasonicSensorIndex = (ultrasonicSensorIndex + 1) % NUM_ULTRASONIC_SENSORS; 
    UltrasonicSensor_SelectSensor(ultrasonicSensorIndex);
}
/* [] END OF FILE */
