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
#include "project.h"
#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>

uint16 count = 0;




CY_ISR (ISR_Handler_Timer_LEFT) {
    ultrasonic_get_distance();
}


void ultrasonic_setup() {
    
    memset(kaldist_measure, 0, sizeof(kaldist_measure));
    udsState = Control_Reg_Ultrasonic_Read();
    
    Timer_Ultrasonic_Start();
    isr_ultrasonic_StartEx(ISR_Handler_Timer);
    
}

void ultrasonic_off() {
    memset(kaldist_measure, 0, sizeof(kaldist_measure));
    Timer_Ultrasonic_Stop();
    isr_ultrasonic_Stop();
}

void ultrasonic_select(int idx) {
    Control_Reg_Ultrasonic_Write(idx);
}

void ultrasonic_transmit() {
    
    udsState = Control_Reg_Ultrasonic_Read();
    
    if (udsState == 0) {
        while (!Echo_LEFT_Read()) {
            Trigger_Write(1);
            CyDelayUs(10);
            Trigger_Write(0);
            CyDelayUs(2);
        }
    } else if (udsState == 1) {
        while (!Echo_RIGHT_Read()) {
            Trigger_Write(1);
            CyDelayUs(10);
            Trigger_Write(0);
            CyDelayUs(2);
        } 
    } else if (udsState == 2) {
        while (!Echo_FLEFT_Read()) {
            Trigger_Write(1);
            CyDelayUs(10);
            Trigger_Write(0);
            CyDelayUs(2);
        } 
    } else {
        while (!Echo_FRIGHT_Read()) {
            Trigger_Write(1);
            CyDelayUs(10);
            Trigger_Write(0);
            CyDelayUs(2);
        }
    }
}

void ultrasonic_get_distance() {
    int idx = Control_Reg_Ultrasonic_Read();
    Timer_Ultrasonic_ReadStatusRegister();
    int counter = Timer_Ultrasonic_ReadCounter();
    double distance = (double)(65535 - count)/58;
    kaldist_measure[idx] = kalman_filter(distance, idx);
}

double kalman_filter(double U, int idx) {
    static const double R = 0.5; // Measurement noise covariance. It represents the amount of noise in the sensor measurement.
    static const double Q = 0.25; // Process noise covariance. It represents the amount of uncertainty in the process or system being modeled.
    static const double H = 1.0; // Measurement matrix. It relates the true state to the measurement.
  
    static double P = 0.5; // Initial estimate error covariance. It represents the uncertainty in the initial estimated state.
    static double U_hat[N] = {0}; // Estimated state. This is the current best estimate of the true state.
    static double K = 0; // Kalman gain. It represents the weight given to the new measurement in the estimation process.

    K = P * H / (H * P * H + R);
    U_hat[idx] += K * (U - H * U_hat[idx]);
    P = (1 - K * H) * P + Q;

    return U_hat[idx];
}

void ultrasonic_measuring() {
    ultrasonic_select(0);
    ultrasonic_transmit();
    
    ultrasonic_select(1);
    ultrasonic_transmit();
    
    ultrasonic_select(2);
    ultrasonic_transmit();
    
    ultrasonic_select(3);
    ultrasonic_transmit();
    
}


/* [] END OF FILE */
