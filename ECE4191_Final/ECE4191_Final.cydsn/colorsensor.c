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
#include <colorsensor.h>
#include "bluetooth.h"

// Global variable
Color detectedColor;
volatile int compare_ready = 0;
int red_counts;
int green_counts;
int blue_counts;
int votes[3];

int color_sensor_status = 0;

CY_ISR(ISR_Handler_Color_Sensor) {
    PWM_Color_Sensor_ReadStatusRegister();
    compare_ready = 1;
}

void InitializeColorSensorArray() {
    red_counts = 0;
    green_counts = 0;
    blue_counts = 0;
    
    votes[0] = 0;
    votes[1] = 0;
    votes[2] = 0;
}

// Function prototypes
void InitializeColorSensor() {
    
    
    PWM_Color_Sensor_Start();
    Counter_Color_Sensor_Start();
    LED_Write(1);
    isr_color_sensor_StartEx(ISR_Handler_Color_Sensor);
    Control_Reg_Color_Sensor_Write(1);
    CyDelay(10);
    Control_Reg_Color_Sensor_Write(0);
    
    color_sensor_status = 1;
    CyDelay(20);
}

void ShutdownColorSensor() {
    InitializeColorSensorArray();
    
    PWM_Color_Sensor_Stop();
    Counter_Color_Sensor_Stop();
    isr_color_sensor_Stop();
    LED_Write(0);   
    CyDelay(20);
    color_sensor_status = 0;
}

void SetColor_Red() {
    S2_Write(0); S3_Write(0);
}
void SetColor_Green() {
    S2_Write(1); S3_Write(1);
}
void SetColor_Blue() {
    S2_Write(0); S3_Write(1);
}
void SetColor_Clear() {
    S2_Write(1); S3_Write(0);
}

void SetFrequencyScaling(int S0_set, int S1_set) {
    if (!color_sensor_status) return;
    
    S0_Write(S0_set);
    S1_Write(S1_set);
    
    CyDelayUs(1000);   
}

void ColorDetection_Run(int numRuns) {
    int mode = 1;
    int count = 0;
    
    int max_red = 0;
    int max_green = 0;
    int max_blue = 0;
    InitializeColorSensorArray();
    
    
    for (int run = 0; run < numRuns*3; run++) {
        InitializeColorSensor();
        CyDelay(20);
        SetFrequencyScaling(1, 1);
        CyDelay(20);
        compare_ready = 0;
        count = 0;
        switch (mode) {
            case 1:
                SetColor_Red();
                break;
            case 2:
                SetColor_Green();
                break;
            case 3:
                SetColor_Blue();
                break;
        }
        
        CyDelayUs(10);
        Control_Reg_Color_Sensor_Write(1);
        CyDelayUs(10);
        Control_Reg_Color_Sensor_Write(0);

        while (!compare_ready) {
        }

        count = Counter_Color_Sensor_ReadCapture();
        
        switch (mode) {
            case 1:
                if (count > max_red) max_red = count;
                break;
            case 2:
                if (count > max_green) max_green = count;
                break;
            case 3:
                if (count > max_blue) max_blue = count;
                break;
        }

        mode = (mode % 3) + 1;
        //ColorDetection_FindMax(max_red, max_green, max_blue);
        if (max_red > max_green && max_red > max_blue) {
            votes[0] += 1;
        } else if (max_green > max_red && max_green > max_blue) {
            votes[1] += 1;
        } else if (max_blue > max_red && max_blue > max_green) {
            votes[2] += 1;
        } else {
        }
        
    }
    
    // DetectColor();
    ColorDetection_FindMax(max_red, max_green, max_blue);
    ShutdownColorSensor();
}

void ColorDetection_FindMax(int maxRed, int maxGreen, int maxBlue) {
    // Compare the maximum counts to determine the detected color
    printValue("Red: %d ", maxRed);
    printValue("Green: %d ", maxGreen);
    printValue("Blue: %d ", maxBlue);
    printValue("\n");
    
    if (maxRed >= 8000 && maxBlue >= 8000 && maxGreen >= 8000) {
        detectedColor = WhiteColor;
        return;
    }
    
    if (maxRed >= 6000 && maxBlue >= 6000 && maxGreen >= 6000) {
        detectedColor = GreyColor;
        return;
    }
    
    if (maxRed > maxGreen && maxRed > maxBlue) {
        detectedColor = RedColor;
        printValue("RED\n");
    } else if (maxGreen > maxRed && maxGreen > maxBlue) {
        detectedColor = GreenColor;
        printValue("GREEN\n");
    } else if (maxBlue > maxRed && maxBlue > maxGreen) {
        detectedColor = BlueColor;
        printValue("BLUE\n");
    } else {
        detectedColor = GreyColor;
        printValue("NONE\n");
    }
    
}

void DetectColor() {
    int max_index, max_value;
    
    max_index = 0;
    max_value = 0;
    
    for (int i = 0; i < 3; i++) {
        if (votes[i] > max_value) {
            max_value = votes[i];
            max_index = i;
        }
        
    }
    
    if (max_index == 0) {
        detectedColor = RedColor;
        printValue("RED\n");
    } else if (max_index == 1) {
        detectedColor = GreenColor;
        printValue("GREEN\n");
    } else if (max_index == 2) {
        detectedColor = BlueColor;
        printValue("BLUE\n");
    } else {
        detectedColor = GreyColor;
        printValue("NONE\n");
    }
    
}

void printColor() {
    if (detectedColor == RedColor) {
        printValue("RED\n");
    } else if (detectedColor == GreenColor) {
        printValue("GREEN\n");
    } else if (detectedColor == BlueColor) {
        printValue("BLUE\n");
    } else {
    }
    
}

/* [] END OF FILE */
