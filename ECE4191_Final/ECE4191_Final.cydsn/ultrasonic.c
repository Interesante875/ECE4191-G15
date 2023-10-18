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
#include <math.h>

#define MAX_ALLOWED_STDDEV 2.0

int ultrasonicSensorIndex;

UdsDetectState udsDetectState;
int count;
double distance;
double median;
int setSensorReadIndex;
double setSensorResults;
double sensorMeasuredDistances[NUM_ULTRASONIC_SENSORS][ARRAY_SIZE];
double sensorResults[NUM_ULTRASONIC_SENSORS];
static int currIdx[NUM_ULTRASONIC_SENSORS];

// Function prototypes
void InitalizeUltrasonicSensor() {
        
    Timer_Ultrasonic_Trigger_Start();
    Timer_Ultrasonic_Echo_Start();    

    isr_ultrasonic_echo_StartEx(ISR_Handler_Ultrasonic_Echo);
    isr_ultrasonic_trigger_StartEx(ISR_Handler_Ultrasonic_Trigger);
    
    for (int i = 0; i < NUM_ULTRASONIC_SENSORS; i++) {
        currIdx[i] = 0;
        sensorResults[i] = 0;
    }
    for (int i = 0; i < NUM_ULTRASONIC_SENSORS; i++) {
        for (int j = 0; j < ARRAY_SIZE; j++) {
            sensorMeasuredDistances[i][j] = 0;
        }
    }
    UltrasonicSensor_ChangeState(UdsDetectAll);
    ultrasonicSensorIndex = 0;

}

void ShutdownUltrasonicSensor() {

    Timer_Ultrasonic_Echo_Stop();    
    Timer_Ultrasonic_Trigger_Stop();
    
    isr_ultrasonic_echo_Stop();
    isr_ultrasonic_trigger_Stop(); 
}

void pauseUltrasonicSensor() {
    Timer_Ultrasonic_Echo_Stop();    
    Timer_Ultrasonic_Trigger_Stop();
    
    isr_ultrasonic_echo_Stop();
    isr_ultrasonic_trigger_Stop();
    
}

void restartUltrasonicSensor() {
    Timer_Ultrasonic_Trigger_Start();
    Timer_Ultrasonic_Echo_Start();    

    isr_ultrasonic_echo_StartEx(ISR_Handler_Ultrasonic_Echo);
    isr_ultrasonic_trigger_StartEx(ISR_Handler_Ultrasonic_Trigger);
    
}

double UltrasonicSensor_ReadDistanceData(int sensorIndex) {


    double val = sensorResults[sensorIndex];

    return val;

}


void UltrasonicSensor_SelectSensor(int sensorIndex) {
    
    
    Control_Reg_Ultrasonic_Write(sensorIndex);
    ultrasonicSensorIndex = sensorIndex;

}

void UltrasonicSensor_MeasureDistance(int sensorIndex) {

   
    count = Timer_Ultrasonic_Echo_ReadCounter();
 
    distance = (double) (65536 - count)/58.0;

    // printValue("(%d): %d %lf\n", sensorIndex, count, distance);
    #if ENABLE_MEDIAN_FILTERING
        sensorMeasuredDistances[sensorIndex][currIdx[sensorIndex]] = distance;
    
        // printValue("(%d): CurrIndex: %d %lf ", sensorIndex, currIdx[sensorIndex], sensorMeasuredDistances[sensorIndex][currIdx[sensorIndex]]);
        
        currIdx[sensorIndex] = (currIdx[sensorIndex] + 1) % ARRAY_SIZE;
        
        // printValue("Next Index: %d\n", currIdx[sensorIndex]);
        
        double * rowPointer = sensorMeasuredDistances[sensorIndex];

//        double mean = 0;
//        for (int i = 0; i < ARRAY_SIZE; i++) {
//            mean += rowPointer[i];
//        }
//        mean /= ARRAY_SIZE;
//
//        double stddev = 0.0;
//        for (int i = 0; i < ARRAY_SIZE; i++) {
//            stddev += pow(rowPointer[i] - mean, 2);
//        }
//        stddev = sqrt(stddev / ARRAY_SIZE);
//
//        // Check if the new reading is within the allowed range.
//        if (fabs(distance - mean) < MAX_ALLOWED_STDDEV * stddev) {
//            double median = findMedian(rowPointer , ARRAY_SIZE);
//            sensorResults[sensorIndex] = median;
//        } else {
//            // Reject the reading if it's too far from the mean.
//            // You can choose to do nothing or handle it differently.
//            // sensorResults[sensorIndex] may not be updated in this case.
//        }
        
        double median = findMedian(rowPointer , ARRAY_SIZE);
        sensorResults[sensorIndex] = median;
        
        // printValue("[%d]: %d %lf\n", sensorIndex, count, sensorResults[sensorIndex]);
    #else
        sensorMeasuredDistances[sensorIndex][0] = distance;    
        sensorResults[sensorIndex] = distance;
    #endif
    
//    printValue("(%d): %.2lf %.2lf %.2lf\n", sensorIndex, sensorMeasuredDistances[sensorIndex][0],
//    sensorMeasuredDistances[sensorIndex][1], sensorMeasuredDistances[sensorIndex][2]);
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

void UltrasonicSensor_ChangeState(UdsDetectState state) {
    udsDetectState = state;
    
    switch (udsDetectState) {
        case UdsDetectAll:
            UltrasonicSensor_SelectSensor(0);
        break;
        case UdsDetectFront:
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
    
    
    
    if (readEcho(ultrasonicSensorIndex) == 0) {
        Trigger_1_Write(1);
        CyDelayUs(10);
        Trigger_1_Write(0);
        Trigger_2_Write(1);
        CyDelayUs(10);
        Trigger_2_Write(0);
        Trigger_3_Write(1);
        CyDelayUs(10);
        Trigger_3_Write(0);
            
    }
    
}

CY_ISR (ISR_Handler_Ultrasonic_Trigger) {
    Timer_Ultrasonic_Trigger_ReadStatusRegister();
    UltrasonicSensor_TriggerBurst();


}

CY_ISR(ISR_Handler_Ultrasonic_Echo) {
    Timer_Ultrasonic_Echo_ReadStatusRegister();
    // printValue("FRONT %d %d\n", ultrasonicSensorIndex_Front, ultrasonicSensorIndex_Back + 4);
    UltrasonicSensor_MeasureDistance(ultrasonicSensorIndex);
    
    // BASED ON STATE
    switch (udsDetectState) {
        case UdsDetectAll:
            ultrasonicSensorIndex = (ultrasonicSensorIndex + 1) % (NUM_ULTRASONIC_SENSORS);
        break;
        case UdsDetectFront:
            if (ultrasonicSensorIndex != 0 && ultrasonicSensorIndex != 1) {
                ultrasonicSensorIndex = 0;   
            } else {
                ultrasonicSensorIndex = (ultrasonicSensorIndex + 1) % (2);
            }
        break;
        case UdsDetectBack:
            if (ultrasonicSensorIndex != 2 && ultrasonicSensorIndex != 3) {
                ultrasonicSensorIndex = 2;
            } else {
                ultrasonicSensorIndex = (ultrasonicSensorIndex == 2) ? 3 : 2;
            }
        break;
        case UdsDetectLeft:
            if (ultrasonicSensorIndex != 4 && ultrasonicSensorIndex != 5) {
                ultrasonicSensorIndex = 4;
            } else {
                ultrasonicSensorIndex = (ultrasonicSensorIndex == 4) ? 5 : 4;
            }
        break;
        case UdsDetectRight:
            if (ultrasonicSensorIndex != 6 && ultrasonicSensorIndex != 7) {
                ultrasonicSensorIndex = 6;
            } else {
                ultrasonicSensorIndex = (ultrasonicSensorIndex == 6) ? 7 : 6;
            }
        break;
        case UdsDetectFLR:
            ultrasonicSensorIndex = (ultrasonicSensorIndex + 1) % 8;
            if (ultrasonicSensorIndex == 2 || ultrasonicSensorIndex == 3)
                ultrasonicSensorIndex = 4;
        break;
        case UdsDetectBLR:
            ultrasonicSensorIndex = (ultrasonicSensorIndex - 2 + 1) % 7 + 2;
            if (ultrasonicSensorIndex == 0 || ultrasonicSensorIndex == 1) ultrasonicSensorIndex = 2;
        break;
        case UdsDetectLR:
            ultrasonicSensorIndex = (ultrasonicSensorIndex - 4 + 1) % 4 + 4;
            if (ultrasonicSensorIndex < 4) ultrasonicSensorIndex = 4;
        break;
        default:
            ultrasonicSensorIndex = (ultrasonicSensorIndex + 1) % (NUM_ULTRASONIC_SENSORS);
        break;
        
        
    }

    UltrasonicSensor_SelectSensor(ultrasonicSensorIndex);
}

//
//CY_ISR (ISR_Handler_Ultrasonic_Read) {
//    setSensorResults = sensorResults[setSensorReadIndex];
//    
//}

/* [] END OF FILE */
