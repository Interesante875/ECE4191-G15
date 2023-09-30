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
#include "irsensor.h"
#include "bluetooth.h"

volatile DetectionStatus infraredDetectionStatus;

CY_ISR(ISR_Handler_IR) {
    infraredDetectionStatus = Presence;
    printValue("Detected!\n");
}

void startIR() {
    isr_IR_Sensor_StartEx(ISR_Handler_IR);
    infraredDetectionStatus = Absence;
}

void stopIR() {
    isr_IR_Sensor_Stop();
}

int IRPinValue() {
    return IR_Sensor_Read();   
}
/* [] END OF FILE */
