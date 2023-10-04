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

CY_ISR(ISR_Handler_IR_Sensor_PositiveEdge) {
    infraredDetectionStatus = Absence;
    printValue("Not Detected!\n");
}

CY_ISR(ISR_Handler_IR_Sensor_NegativeEdge) {
    infraredDetectionStatus = Presence;
    printValue("Detected!\n");
}

void startIR() {
    isr_IR_Sensor_PositiveEdge_StartEx(ISR_Handler_IR_Sensor_PositiveEdge);
    isr_IR_Sensor_NegativeEdge_StartEx(ISR_Handler_IR_Sensor_NegativeEdge);
    infraredDetectionStatus = Absence;
}

void stopIR() {
    isr_IR_Sensor_PositiveEdge_Stop();
    isr_IR_Sensor_NegativeEdge_Stop();
    infraredDetectionStatus = Absence;
}

int IRPinValue() {
    return IR_Sensor_Read();   
}
/* [] END OF FILE */
