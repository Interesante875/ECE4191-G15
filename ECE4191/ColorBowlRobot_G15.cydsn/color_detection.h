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
#ifndef COLOR_DETECTION_H
#define COLOR_DETECTION_H

    #include "project.h"
    
    #define DELAY_CYCLES 200

    #define INIT_COLOR_DETECTION color_detection_init()
    #define SET_COLOR_RED S2_Write(0); S3_Write(0)
    #define SET_COLOR_GREEN S2_Write(1); S3_Write(1)
    #define SET_COLOR_BLUE S2_Write(0); S3_Write(1)
    #define SET_COLOR_CLEAR S2_Write(1); S3_Write(0)
        
    #ifndef COLOR_DETECTION_RUNS
    #define COLOR_DETECTION_RUNS 1
    #endif

    typedef enum {
        RED,
        GREEN,
        BLUE,
        YELLOW,
        GREY,
        WHITE
    } COLOR;
    
    extern COLOR detectedColor;

    void color_sensor_start();
    void color_sensor_stop();
    void set_frequency_scaling(int S0_set, int S1_set);
    void color_detection_init(void);
    void color_detection_run(int num_runs);
    void color_detection_find_max(int max_red, int max_green, int max_blue);
    void color_detection(int max_red, int max_green, int max_blue);
    
    CY_ISR(ISR_Handler_Color_Sensor);

#endif /* COLOR_DETECTION_H */
/* [] END OF FILE */
