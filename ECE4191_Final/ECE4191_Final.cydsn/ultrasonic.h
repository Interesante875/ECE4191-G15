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
#ifndef ULTRASONIC_SENSOR_H
#define ULTRASONIC_SENSOR_H

#include <stdint.h>
#include "project.h"
#include "cytypes.h"
#include "median_filter.h"

    #define ENABLE_MEDIAN_FILTERING 0
    #define ENABLE_TIMER_BURST 0
    #define NUM_ULTRASONIC_SENSORS 8
    #define ARRAY_SIZE 3
    /*
    DP-0
    0 bit 0 - FRONT_LEFT
    1 bit 1 - FRONT_RIGHT
    2 bit 2 - BACK_LEFT
    3 bit 3 - BACK_RIGHT
    
    DP-1
    4 bit 0 - LEFT_FRONT
    5 bit 1 - LEFT_BACK
    6 bit 2 - RIGHT_FRONT
    7 bit 3 - RIGHT_BACK
    */
    
    typedef enum {
        UdsDetectAll,
        UdsDetectFront,
        UdsDetectBack,
        UdsDetectLeft,
        UdsDetectRight,
        UdsDetectFLR,
        UdsDetectBLR,
        UdsDetectLR
    } UdsDetectState;
    
    extern UdsDetectState udsDetectState;
    
    extern int ultrasonicSensorIndex_FB;
    extern int ultrasonicSensorIndex_LR;
    
    extern double sensorMeasuredDistances[NUM_ULTRASONIC_SENSORS][ARRAY_SIZE];


    // Function prototypes
    void InitalizeUltrasonicSensor();
    void ShutdownUltrasonicSensor();

    void UltrasonicSensor_TriggerBurst();
 
    double UltrasonicSensor_ReadDistanceData(int sensorIdx);
    void UltrasonicSensor_SelectSensor(int sensorIndex);
    void UltrasonicSensor_MeasureDistance();
    
    void UltrasonicSensor_ChangeState(UdsDetectState state);

    
    CY_ISR(ISR_Handler_Ultrasonic_Burst);
    CY_ISR(ISR_Handler_Ultrasonic_Echo_FB);
    CY_ISR(ISR_Handler_Ultrasonic_Echo_LR);

#endif // ULTRASONIC_SENSOR_H
/* [] END OF FILE */
