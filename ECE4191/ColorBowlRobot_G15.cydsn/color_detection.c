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
#include "color_detection.h"
#include <stdio.h>
#include <stdlib.h>

static volatile int compare_ready = 0;
static int red_counts;
static int green_counts;
static int blue_counts;

static int color_sensor_status_1 = 0;

CY_ISR(ISR_Handler_Color_Sensor) {
    PWM_Color_Sensor_ReadStatusRegister();
    compare_ready = 1;
}

void color_sensor_start() {
    PWM_Color_Sensor_Start();
    Counter_Color_Sensor_Start();
    isr_color_sensor_StartEx(ISR_Handler_Color_Sensor);
    
    color_sensor_status_1 = 1;
}

void color_sensor_stop() {
    PWM_Color_Sensor_Stop();
    Counter_Color_Sensor_Stop();
    isr_color_sensor_Stop();
    LED_Write(0);
    color_sensor_status_1 = 0;
}

void set_frequency_scaling(int S0_set, int S1_set) {
    if (!color_sensor_status_1) return;
    
    S0_Write(S0_set);
    S1_Write(S1_set);
    
    CyDelayCycles(DELAY_CYCLES);
}

void color_sensor_array_initialization() {
    red_counts = 0;
    green_counts = 0;
    blue_counts = 0;
    
}

void color_detection_run(int num_runs) {
    int mode = 1;
    int count = 0;
    
    int max_red = 0;
    int max_green = 0;
    int max_blue = 0;
    
    color_sensor_array_initialization();
    

    for (int run = 0; run < num_runs*3; run++) {
        compare_ready = 0;

        switch (mode) {
            case 1:
                SET_COLOR_RED;
                break;
            case 2:
                SET_COLOR_GREEN;
                break;
            case 3:
                SET_COLOR_BLUE;
                break;
        }
        CyDelayUs(10);
        LED_Write(1);
        CyDelayUs(10);
        Control_Reg_Color_Sensor_Write(1);
        CyDelayUs(10);
        Control_Reg_Color_Sensor_Write(0);

        while (!compare_ready) {
        }

        count = Counter_Color_Sensor_ReadCapture();
        CyDelayUs(200);
        
        switch (mode) {
            case 1:
                if (count > max_red) max_red = count;
                //sprintf(string1, "Red: %d\n", count);
                break;
            case 2:
                if (count > max_green) max_green = count;
                //sprintf(string1, "Green: %d\n", count);
                break;
            case 3:
                if (count > max_blue) max_blue = count;
                //sprintf(string1, "Blue: %d\n", count);
                break;
        }

        //UART_1_PutString(string1);
        mode = (mode % 3) + 1;
        
    }
    
    color_detection_find_max(max_red, max_green, max_blue);
    color_sensor_array_initialization();
}

void color_detection_find_max(int max_red, int max_green, int max_blue) {
    /*
    char string2[20];
    sprintf(string2, "Max Red: %d\n", max_red);
    UART_1_PutString(string2);
    sprintf(string2, "Max Green: %d\n", max_green);
    UART_1_PutString(string2);
    sprintf(string2, "Max Blue: %d\n", max_blue);
    UART_1_PutString(string2);
    */
    // Compare the maximum counts to determine the detected color
    if (max_red > max_green && max_red > max_blue) {
        /*
        UART_1_PutString("Detected Color: Red\n");
        sprintf(string2, "Max Red: %d\n", max_red);
        UART_1_PutString(string2);
        */
        detectedColor = RED;
    } else if (max_green > max_red && max_green > max_blue) {
        /*
        UART_1_PutString("Detected Color: Green\n");
        sprintf(string2, "Max Green: %d\n", max_green);
        UART_1_PutString(string2);
        */
        detectedColor = GREEN;
    } else if (max_blue > max_red && max_blue > max_green) {
        /*
        UART_1_PutString("Detected Color: Blue\n");
        sprintf(string2, "Max Blue: %d\n", max_blue);
        UART_1_PutString(string2);
        */
        detectedColor = BLUE;
    } else {
        // UART_1_PutString("Unable to determine detected color\n");
        detectedColor = GREY;
    }
    
}

/*
void color_detection(int max_red, int max_green, int max_blue) {
    const int GREY_THRESHOLD[3] = {0, 0, 0};
    const int YELLOW_THRESHOLD[3] = {0, 0, 0};
    const int WHITE_THRESHOLD[3] = {0, 0, 0};
    
    detectedColor = GREY;
}
*/
/* [] END OF FILE */
