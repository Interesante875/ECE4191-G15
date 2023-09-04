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
#include "ultrasonic.h"
#include "bluetooth.h"
#include "project.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <median_filter.h>

int udsState;
double kaldist_measure[NUMBER_OF_UDS];
double uds_LEFT[ARRAY_SIZE];
double uds_RIGHT[ARRAY_SIZE];
double uds_FLEFT[ARRAY_SIZE];
double uds_FRIGHT[ARRAY_SIZE];
double uds_BACK[ARRAY_SIZE];
int burst_count = 0;

void ultrasonic_on() {
    Timer_Ultrasonic_Burst_Start();
    Timer_Ultrasonic_Start();
    
    isr_ultrasonic_burst_StartEx(ISR_Handler_ultrasonic_burst);
    isr_ultrasonic_echo_StartEx(ISR_Handler_ultrasonic_echo);
}

void ultrasonic_off() {

    Timer_Ultrasonic_Stop();
    Timer_Ultrasonic_Burst_Stop();
    isr_ultrasonic_burst_Stop();
    isr_ultrasonic_echo_Stop();
    
}

void ultrasonic_start() {
    
    burst_count = 0;
    
    Timer_Ultrasonic_Burst_Start();
    Timer_Ultrasonic_Start();
    
}

void ultrasonic_stop() {
    Timer_Ultrasonic_Stop();
    Timer_Ultrasonic_Burst_Stop();
}

void ultrasonic_trigger_burst() {
    Trigger_Write(1);
    CyDelayUs(10);
    Trigger_Write(0);
    CyDelayUs(2);
}

CY_ISR (ISR_Handler_ultrasonic_burst) {
    Timer_Ultrasonic_Burst_ReadStatusRegister();
    ultrasonic_trigger_burst();
}

CY_ISR (ISR_Handler_ultrasonic_echo) {
    
    Timer_Ultrasonic_ReadStatusRegister();
    int count = Timer_Ultrasonic_ReadCounter();
    double distance = (double) (65535 - count)/58.0;
    
    //printValue("%d Distance: %d\n", udsState, (int) distance);
    kaldist_measure[udsState] = median(udsState, distance);
    //kaldist_measure[udsState] = distance;
    if (udsState == 4) burst_count++;
    
    udsState = (udsState + 1) % 5; 
    ultrasonic_select(udsState);

}

void ultrasonic_select(int idx) {
    Control_Reg_Ultrasonic_Write(idx);
    CyDelayUs(10);
    udsState = Control_Reg_Ultrasonic_Read();
    CyDelayUs(10);
}

double median(int idx, double distance) {
    double med = 0;
    
    switch(idx) {
        case 0:   
            med = medianFilter(uds_LEFT, distance, 0);
            break;
        case 1:
            med = medianFilter(uds_RIGHT, distance, 1);
            break;
        case 2:
            med = medianFilter(uds_FLEFT, distance, 2);
            break;
        case 3:
            med = medianFilter(uds_FRIGHT, distance, 3);
            break;
        case 4:
            med = medianFilter(uds_BACK, distance, 4);
            break;
        default:
            med = medianFilter(uds_BACK, distance, 4);
            break;
    }
    
    return med;
}

/*
void ultrasonic_get_distance() {
    
    Timer_Ultrasonic_ReadStatusRegister();
    int counter = Timer_Ultrasonic_ReadCounter();
    double distance = (double)(65535 - counter)/58;
    
    int idx = Control_Reg_Ultrasonic_Read();
    CyDelayUs(10);
    //printValue("%d: Distance: %d\t", idx, (int) distance);
    printValue("%d: %d\n", idx, (int) distance);
    kaldist_measure[idx] = kalman_filter(distance, idx);
    
    switch(idx) {
        case 0:   
            medianFilter(uds_LEFT, distance);
            break;
        case 1:
            medianFilter(uds_RIGHT, distance);
            break;
        case 2:
            medianFilter(uds_FLEFT, distance);
            break;
        case 3:
            medianFilter(uds_FRIGHT, distance);
            break;
        case 4:
            medianFilter(uds_BACK, distance);
            break;
        default:
            medianFilter(uds_BACK, distance);
            break;
    }
    
}

double kalman_filter(double U, int idx) {
    static const double R = 0.5; // Measurement noise covariance. It represents the amount of noise in the sensor measurement.
    static const double Q = 0.25; // Process noise covariance. It represents the amount of uncertainty in the process or system being modeled.
    static const double H = 1.0; // Measurement matrix. It relates the true state to the measurement.
  
    static double P = 0.5; // Initial estimate error covariance. It represents the uncertainty in the initial estimated state.
    static double U_hat[NUMBER_OF_UDS] = {0}; // Estimated state. This is the current best estimate of the true state.
    static double K = 0; // Kalman gain. It represents the weight given to the new measurement in the estimation process.

    K = P * H / (H * P * H + R);
    U_hat[idx] += K * (U - H * U_hat[idx]);
    P = (1 - K * H) * P + Q;

    return U_hat[idx];
}
*/


/* [] END OF FILE */
