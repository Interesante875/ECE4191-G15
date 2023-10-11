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
double sensorMeasuredDistances[NUM_ULTRASONIC_SENSORS][ARRAY_SIZE];
static int currIdx[NUM_ULTRASONIC_SENSORS];

// Function prototypes
void InitalizeUltrasonicSensor() {
        
    Timer_Ultrasonic_Trigger_Start();
    Timer_Ultrasonic_Echo_Start();    

    isr_ultrasonic_echo_StartEx(ISR_Handler_Ultrasonic_Echo);
    isr_ultrasonic_trigger_StartEx(ISR_Handler_Ultrasonic_Trigger);
    
    for (int i = 0; i < NUM_ULTRASONIC_SENSORS; i++) {
        currIdx[i] = 0;
    }
    for (int i = 0; i < NUM_ULTRASONIC_SENSORS; i++) {
        for (int j = 0; j < ARRAY_SIZE; j++) {
            sensorMeasuredDistances[i][j] = 0;
        }
    }
    ultrasonicSensorIndex = 0;

}

void ShutdownUltrasonicSensor() {

    Timer_Ultrasonic_Echo_Stop();    
    Timer_Ultrasonic_Trigger_Stop();
    
    isr_ultrasonic_echo_Stop();
    isr_ultrasonic_trigger_Stop(); 
    
}


double UltrasonicSensor_ReadDistanceData(int sensorIndex) {
    
    
    #if ENABLE_MEDIAN_FILTERING == 1
        double* rowPointer = sensorMeasuredDistances[sensorIndex];
        median = findMedian(rowPointer, ARRAY_SIZE);
        printValue("UDS(%d) %lf\n", sensorIndex, median);
        return median;
    #endif
    
    
    double value = sensorMeasuredDistances[sensorIndex][0];
    printValue("Read: (%d) %lf\n", sensorIndex, value);
    return value;

}


void UltrasonicSensor_SelectSensor(int sensorIndex) {
 
    Control_Reg_Ultrasonic_Write(sensorIndex);
    ultrasonicSensorIndex = sensorIndex;

}

void UltrasonicSensor_MeasureDistance(int sensorIndex) {


    count = Timer_Ultrasonic_Echo_ReadCounter();
 
    distance = (double) (65535 - count)/58.0;
    
    if (sensorIndex == 0)
    printValue("(%d): %d %lf\n", sensorIndex, count, distance);
    #if ENABLE_MEDIAN_FILTERING
        sensorMeasuredDistances[sensorIndex][currIdx[sensorIndex]] = distance;
    
        // printValue("(%d): CurrIndex: %d %lf, ", sensorIndex, currIdx[sensorIndex], sensorMeasuredDistances[sensorIndex][currIdx[sensorIndex]]);
        
        currIdx[sensorIndex] = (currIdx[sensorIndex] + 1) % ARRAY_SIZE;
        
        // printValue("Next Index: %d\n", currIdx[sensorIndex]);
    #else
        sensorMeasuredDistances[sensorIndex][0] = distance;    
    #endif
    
}


int readEcho(int sensorIndex) {
       
    switch (sensorIndex) {
        case 0: 
        return Echo_FRONT_LEFT_Read();
        case 1: 
        return Echo_FRONT_RIGHT_Read();
        case 2: 
        return Echo_BACK_LEFT_Read();
        case 3: 
        return Echo_BACK_RIGHT_Read();
        case 4: 
        return Echo_LEFT_FRONT_Read();
        case 5: 
        return Echo_LEFT_BACK_Read();
        case 6: 
        return Echo_RIGHT_FRONT_Read();
        case 7: 
        return Echo_RIGHT_BACK_Read();
        default:
        return 0;
    }
}

CY_ISR (ISR_Handler_Ultrasonic_Trigger) {
    Timer_Ultrasonic_Trigger_ReadStatusRegister();
    
    while (readEcho(ultrasonicSensorIndex) == 0) {
        Trigger_1_Write(1);
        Trigger_2_Write(1);
        CyDelayUs(10);
        Trigger_1_Write(0);
        Trigger_2_Write(0);
    }

}

CY_ISR(ISR_Handler_Ultrasonic_Echo) {
    Timer_Ultrasonic_Echo_ReadStatusRegister();
    // printValue("FRONT %d %d\n", ultrasonicSensorIndex_Front, ultrasonicSensorIndex_Back + 4);
    UltrasonicSensor_MeasureDistance(ultrasonicSensorIndex);
    
    // BASED ON STATE
    ultrasonicSensorIndex = (ultrasonicSensorIndex + 1) % (NUM_ULTRASONIC_SENSORS);
    UltrasonicSensor_SelectSensor(ultrasonicSensorIndex);
    
}

/* [] END OF FILE */
