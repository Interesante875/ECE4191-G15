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
#include <stdio.h>
#include <string.h>
#include "prelim.h"
#include "bluetooth.h"


void moveOutOfBaseFast() {
    
    
    bluetooth_start();
    ultrasonic_on();
    CyDelay(500);
    
    bool wall_not_encountered = true;
    
    const double WALL_CUSHION = 28;
    
    wheel_move(FORWARD, 240);
    
    while (wall_not_encountered) {
        
        wall_not_encountered = (kaldist_measure[2] > WALL_CUSHION || kaldist_measure[3] > WALL_CUSHION);
        
    }
    
    wheel_move(STOP, 0);
    
    wheel_move_by_metrics(LEFT, 240, 90);
    
}

void moveOutOfBase() {
    
    bluetooth_start();
    ultrasonic_on();
    CyDelay(500);
    
    bool wall_not_encountered = true;
    
    const double WALL_CUSHION = 28;
    
    //double cumulative_dist = 0;
    
    //double move_dist = (kaldist_measure[2]>kaldist_measure[3])?kaldist_measure[3]:kaldist_measure[2] - WALL_CUSHION;
    
    while (wall_not_encountered) { 
        
        wheel_move_by_metrics(FORWARD, 240, 0.2);
        
        for (int i = 0; i < NUMBER_OF_UDS; i++) {
            printValue("%d:%d, %s", i, (int) kaldist_measure[i], i==NUMBER_OF_UDS-1?"\n":"");   
        }
        
        wall_not_encountered = (kaldist_measure[2] > WALL_CUSHION || kaldist_measure[3] > WALL_CUSHION);
        
    }
    
    wheel_move_by_metrics(LEFT, 240, 90);

}

void detectSlit() {
    bool slit_not_encountered = true;
    bool front_wall_not_encountered = true;
    bool back_wall_not_encountered = true;
    
    
    
    const double FRONT_WALL_CUSHION = 15;
    const double BACK_WALL_CUSHION = 15;
    const double SLIT_EPISLON = 40;
    
    static double prevToObsDist = 0;
    
    prevToObsDist = kaldist_measure[1];
    
    slit_not_encountered = (kaldist_measure[1] < SLIT_EPISLON);
    
    front_wall_not_encountered = (kaldist_measure[2] > FRONT_WALL_CUSHION || kaldist_measure[3] > FRONT_WALL_CUSHION);
    back_wall_not_encountered = (kaldist_measure[4] > BACK_WALL_CUSHION); 
    
    while (slit_not_encountered) {
        
        front_wall_not_encountered = true;
        back_wall_not_encountered = true;
        
        while (front_wall_not_encountered && slit_not_encountered) {
            wheel_move_by_metrics(FORWARD, 240, 0.2);
            
            front_wall_not_encountered = (kaldist_measure[2] > FRONT_WALL_CUSHION || kaldist_measure[3] > FRONT_WALL_CUSHION);
            
            slit_not_encountered = (kaldist_measure[1] < SLIT_EPISLON);      
        }
        
        while (back_wall_not_encountered && slit_not_encountered) {
            
            wheel_move_by_metrics(BACKWARD, 240, 0.2);
            
            back_wall_not_encountered = (kaldist_measure[4] > BACK_WALL_CUSHION);
            
            slit_not_encountered = (kaldist_measure[1] < SLIT_EPISLON);
            
        }
        
        
    }
    
    if (front_wall_not_encountered && back_wall_not_encountered) wheel_move_by_metrics(FORWARD, 240, 0.075); 
    else if (!front_wall_not_encountered) wheel_move_by_metrics(BACKWARD, 240, 0.08);
    
    wheel_move_by_metrics(RIGHT, 240, 90);
    
}

void moveThroughSlit() {
    trunk_up();
    gripper_open();
    wheel_move_by_metrics(FORWARD, 240, 0.35); 
    gripper_close();
    
    trunk_middle();
    CyDelay(250);
    
    color_sensor_start();
    set_frequency_scaling(1, 1);
    color_detection_run(COLOR_DETECTION_RUNS);
    
    printValue("Color: %d\n", detectedColor);
    
    for (int i = 0; i < 3; i++) {
    
        switch(detectedColor) {
            case RED:
                Indicator_RED_Write(1); 
                CyDelay(1000);
                break;
            case BLUE:
                Indicator_BLUE_Write(1);
                CyDelay(1000);
                break;
            case GREEN:
                Indicator_GREEN_Write(1);
                CyDelay(1000);
                break;
            default:
                Indicator_RED_Write(1);
                Indicator_BLUE_Write(1);
                Indicator_GREEN_Write(1);
                CyDelay(1000);
                break;
        }
    }
    
    color_sensor_stop();
    
    Indicator_RED_Write(0);
    Indicator_BLUE_Write(0);
    Indicator_GREEN_Write(0);
    
    wheel_move_by_metrics(BACKWARD, 240, 0.35);
}

void readIRSensor() {
    
    
}

/*
void adjust_to_the_slit() {
    
    bool not_passed = true;
    bool left_not_passed = true;
    bool right_not_passed = true;
    
    ultrasonic_measuring();
    
    left_not_passed = kaldist_measure[2] < 40;
    right_not_passed = kaldist_measure[3] < 40;
    
    double decreasing_factor = 0.8;
    double dist = 0.1;
    
    while (not_passed) {
        if (left_not_passed) {
            wheel_turn_by_angle(RIGHT, 240, 90);
            wheel_move_by_distance(FORWARD, 240, dist);
            wheel_turn_by_angle(LEFT, 240, 90);
        } else if (right_not_passed) {
            wheel_turn_by_angle(LEFT, 240, 90);
            wheel_move_by_distance(FORWARD, 240, dist);
            wheel_turn_by_angle(RIGHT, 240, 90); 
        }
        
        ultrasonic_measuring();
        
        left_not_passed = kaldist_measure[2] < 40;
        right_not_passed = kaldist_measure[3] < 40;
        
        not_passed = left_not_passed || right_not_passed;
        
        dist *= decreasing_factor;
        
    }
    
    
}

void move_through_slit() {
    wheel_move_by_distance(FORWARD, 240, 0.1);   
    
    
}
*/

/* [] END OF FILE */
