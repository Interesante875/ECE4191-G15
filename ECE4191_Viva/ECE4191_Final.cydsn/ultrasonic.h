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

#define ENABLE_MEDIAN_FILTERING 1
#define NUM_ULTRASONIC_SENSORS 8
#define ARRAY_SIZE 2
    /*
    bit 0 - FRONT_LEFT
    bit 1 - FRONT_RIGHT
    bit 2 - BACK_LEFT
    bit 3 - BACK_RIGHT
    bit 4 - LEFT_FRONT
    bit 5 - LEFT_BACK
    bit 6 - RIGHT_FRONT
    bit 7 - RIGHT_BACK
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
    
    extern int ultrasonicSensorIndex;

    extern double sensorMeasuredDistances[NUM_ULTRASONIC_SENSORS][ARRAY_SIZE];


    // Function prototypes
    void InitalizeUltrasonicSensor();
    void ShutdownUltrasonicSensor();

    void UltrasonicSensor_TriggerBurst();
 
    double UltrasonicSensor_ReadDistanceData(int sensorIdx);
    void UltrasonicSensor_SelectSensor(int sensorIndex);
    void UltrasonicSensor_MeasureDistance();
    void UltrasonicSensor_ChangeState(UdsDetectState state);
    void UltrasonicSensor_Read(int sensorIdx);
    
    CY_ISR(ISR_Handler_Ultrasonic_Burst);
    CY_ISR(ISR_Handler_Ultrasonic_Echo);
    CY_ISR(ISR_Handler_Ultrasonic_Read);

#endif // ULTRASONIC_SENSOR_H
/* [] END OF FILE */
