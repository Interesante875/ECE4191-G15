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

int adc_receive = 0;
double analogDistance;
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
    
    analogDistance = 0;
    SharpIndex = 0;
    
    AMux_1_Start();
    selectSharpIR(SharpIndex);
    ADC_DelSig_1_Start();
    ADC_DelSig_1_StartConvert();  
    
    if (interruptBased) {
        Timer_SharpIR_Start();
        isr_analog_measure_StartEx(ISR_Handler_Analog_Measure);
    }
    
}

void selectSharpIR(int sensorIndex) {
    AMux_1_FastSelect(sensorIndex);
}

void stopSharpIR() {
    AMux_1_Stop();
    ADC_DelSig_1_Stop();
    Timer_SharpIR_Stop();
    isr_analog_measure_Stop();
}

double SharpIR_ReadDistance() {
    const double a = 237299.53, b = 1840.20;
    adc_receive = ADC_DelSig_1_GetResult16();
    analogDistance = a/(adc_receive - b) - 0.5;
    
    return analogDistance;
    
}

void readSharpIR() {
    const double a = 237299.53, b = 1840.20;
    
    adc_receive = ADC_DelSig_1_GetResult16();
    
    // printValue("ADC: %d\n", adc_receive);
    
    analogDistance = a/(adc_receive - b) - 0.5;
    
}

CY_ISR (ISR_Handler_Analog_Measure) {
    
    Timer_SharpIR_ReadStatusRegister();
    readSharpIR();
}
/* [] END OF FILE */
