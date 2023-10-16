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
#include <cytypes.h>

int adc_receive_left = 0;
int adc_receive_right = 0;
double analogDistanceLeft;
double analogDistanceRight;
int SharpIndex;
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


void initializeSharpIR(int interruptBased) {
    
    analogDistanceLeft = 0;
    analogDistanceRight = 0;
    SharpIndex = 0;
    
//    selectSharpIR(SharpIndex);
    ADC_DelSig_1_Start();
    ADC_DelSig_1_StartConvert();  
    
    ADC_SAR_1_Start();
    ADC_SAR_1_SetPower(0);
    ADC_SAR_1_StartConvert();
    
    if (interruptBased) {
        Timer_SharpIR_Start();
        isr_analog_measure_StartEx(ISR_Handler_Analog_Measure);
    }
    
}

void selectSharpIR(int sensorIndex) {
//    Control_Reg_SharpIR_Write(sensorIndex);
}

void stopSharpIR() {
    
    ADC_DelSig_1_Stop();
    
    ADC_SAR_1_Stop();
    
    Timer_SharpIR_Stop();
    isr_analog_measure_Stop();
}

double SharpIR_ReadDistance(int LR) {
    const double c = 17841.10, d = 228.70;
    const double a = 237299.53, b = 1840.20;
    adc_receive_left = ADC_DelSig_1_GetResult16();
    adc_receive_right = ADC_SAR_1_GetResult16();
    analogDistanceLeft = a/(adc_receive_left - b) - 0.5;
    analogDistanceRight = c/(adc_receive_right - d);
    
    if (!LR)
        return analogDistanceLeft;
    else 
        return analogDistanceRight;
}

void readSharpIR() {
    const double a = 237299.53, b = 1840.20;
    
    const double c = 17841.10, d = 228.70;
    
    adc_receive_left = ADC_DelSig_1_GetResult16();
    adc_receive_right = ADC_SAR_1_GetResult16();
    analogDistanceLeft = a/(adc_receive_left - b) - 0.5;
    analogDistanceRight = c/(adc_receive_right - d);
    
    
    printValue("L: %.2lf R:%.2lf\n", analogDistanceLeft, analogDistanceRight);
    
    
    
}

CY_ISR (ISR_Handler_Analog_Measure) {
    
    Timer_SharpIR_ReadStatusRegister();
    readSharpIR();
}
/* [] END OF FILE */
