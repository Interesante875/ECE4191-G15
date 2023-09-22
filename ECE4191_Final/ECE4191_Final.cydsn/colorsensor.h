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

#ifndef COLORSENSOR_H
#define COLORSENSOR_H

#include "project.h"

// Constants
#define COLOR_DETECTION_DELAY_CYCLES 200
    
// Color enum
typedef enum {
    RedColor,
    GreenColor,
    BlueColor,
    GreyColor
} Color;


// Global variable
extern Color detectedColor;

// Function prototypes
void ColorDetection_Start();
void ColorDetection_Stop();
void SetColor_Red();
void SetColor_Green();
void SetColor_Blue();
void SetColor_Clear();
void SetFrequencyScaling(int s0Set, int s1Set);
void ColorDetection_Init();
void ColorDetection_Run(int numRuns);
void ColorDetection_FindMax(int maxRed, int maxGreen, int maxBlue);
void DetectColor(int maxRed, int maxGreen, int maxBlue);


// Interrupt service routine
CY_ISR(ISR_Handler_Color_Sensor);

#endif /* COLORSENSOR_H */

/* [] END OF FILE */
