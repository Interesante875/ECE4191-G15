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
#include <pthread.h>
#include <stdlib.h>
#include <stdbool.h>


int burst_count = 0;

CY_ISR (ISR_Handler_Ultrasonic_Burst_Timer) {
    
    
    Timer_Ultrasonic_Burst_ReadStatusRegister();
    Trigger_Write(1);
    CyDelayUs(10);
    Trigger_Write(0);
    
}

CY_ISR (ISR_Handler_Ultrasonic_Timer) {
    
    Timer_Ultrasonic_ReadStatusRegister();
    int count = Timer_Ultrasonic_ReadCounter();
    double distance = (double) (65535 - count)/58.0;
    
    printValue("%d Distance: %d\n", udsState, (int) distance);
    kaldist_measure[udsState] = distance;
    if (udsState == 4) burst_count++;
    
    udsState = (udsState + 1) % 5; 
    ultrasonic_select(udsState);

}


void ultrasonic_setup() {
    
    //memset(kaldist_measure, 0, sizeof(kaldist_measure));
    burst_count = 0;
    Control_Reg_Ultrasonic_Write(0);
    udsState = Control_Reg_Ultrasonic_Read();
    
    
    Timer_Ultrasonic_Burst_Start();
    Timer_Ultrasonic_Start();
    
    isr_ultrasonic_burst_StartEx(ISR_Handler_Ultrasonic_Burst_Timer);
    isr_ultrasonic_StartEx(ISR_Handler_Ultrasonic_Timer);
    
    printValue("Ultrasonic Module ON\n");
    
}

void ultrasonic_off() {
    //memset(kaldist_measure, 0, sizeof(kaldist_measure));
    Timer_Ultrasonic_Stop();
    Timer_Ultrasonic_Burst_Stop();
    isr_ultrasonic_Stop();
    isr_ultrasonic_burst_Stop();
    
    //printValue("Ultrasonic Module OFF\n");
}

void ultrasonic_select(int idx) {
    Control_Reg_Ultrasonic_Write(idx);
    //CyDelayUs(10);
    udsState = Control_Reg_Ultrasonic_Read();
    //CyDelayUs(10);
    //printValue("Ultrasonic Module Selection %d\n", udsState);
}

void ultrasonic_transmit() {
    

    if (udsState == 0) {
        
        while (!Echo_LEFT_Read()) {
            Trigger_Write(1);
            CyDelayUs(10);
            Trigger_Write(0);
         
        }
    } else if (udsState == 1) {
        while (!Echo_RIGHT_Read()) {
            Trigger_Write(1);
            CyDelayUs(10);
            Trigger_Write(0);
        
        }
    } else if (udsState == 2) {
        
        while (!Echo_FLEFT_Read()) {
            
            Trigger_Write(1);
            CyDelayUs(10);
            Trigger_Write(0);
      
        }
    } else if (udsState == 3) {
       
        while (!Echo_FRIGHT_Read()) {
            
            Trigger_Write(1);
            CyDelayUs(10);
            Trigger_Write(0);
     
        }
    } else if (udsState == 4) {

        while (!Echo_BACK_Read()) {
            Trigger_Write(1);
            CyDelayUs(10);
            Trigger_Write(0);
       
        }
    }
    
    //printValue("Ultrasonic Module finish TRANSMITTING with Selection %d\n", udsState);
}

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


void ultrasonic_measuring() {
    ultrasonic_setup();
    
    while (burst_count <= ULTRASONIC_BURSTS);
    
    ultrasonic_off();
}


/* [] END OF FILE */
