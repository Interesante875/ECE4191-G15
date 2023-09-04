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
#include "color_detection.h"
#include "locomotion.h"
#include "ultrasonic.h"
#include "servo.h"
#include "navigation_stack.h"


void initBase() {
    
    initializePosition(RED_BASE);
    
}

void moveOutOfBaseFast() {
    
    
    bluetooth_start();
    ultrasonic_on();
    CyDelay(500);
    
    bool wall_not_encountered = true;
    
    const double WALL_CUSHION = 20;
    
    wheel_move(FORWARD, 240);
    
    while (wall_not_encountered) {
        
        wall_not_encountered = (kaldist_measure[2] > WALL_CUSHION || kaldist_measure[3] > WALL_CUSHION);
        
    }
    
    wheel_move(STOP, 0);
    
    wheel_move_by_metrics(RIGHT, 240, 90);
    
}

void moveOutOfBase() {
    
    bluetooth_start();
    
    trunk_up();
    trunk_off();
    ultrasonic_on();
    CyDelay(500);
    
    bool wall_not_encountered = true;
    
    const double WALL_CUSHION = 25;
    
    //double cumulative_dist = 0;
    
    //double move_dist = (kaldist_measure[2]>kaldist_measure[3])?kaldist_measure[3]:kaldist_measure[2] - WALL_CUSHION;
    
    while (wall_not_encountered) { 
        
        wheel_move_by_metrics(FORWARD, 240, 0.1);
        //angle_correction_with_ticks (FORWARD, 240);
        
//        for (int i = 0; i < NUMBER_OF_UDS; i++) {
//            printValue("%d:%d, %s", i, (int) kaldist_measure[i], i==NUMBER_OF_UDS-1?"\n":"");   
//        }
        
        wall_not_encountered = (kaldist_measure[2] > WALL_CUSHION || kaldist_measure[3] > WALL_CUSHION);
        
    }
    
    wheel_move_by_metrics(RIGHT, 240, 90);
    //angle_correction_with_ticks (RIGHT, 240);
}

void detectSlit() {
    bool slit_not_encountered = true;
    bool front_wall_not_encountered = true;
    bool back_wall_not_encountered = true;
    
    
    
    const double FRONT_WALL_CUSHION = 12;
    const double BACK_WALL_CUSHION = 12;
    const double SLIT_EPISLON = 50;
    
    static double prevToObsDist = 0;
    
    prevToObsDist = kaldist_measure[1];
    
    slit_not_encountered = (kaldist_measure[0] < SLIT_EPISLON);
    
    front_wall_not_encountered = (kaldist_measure[2] > FRONT_WALL_CUSHION || kaldist_measure[3] > FRONT_WALL_CUSHION);
    back_wall_not_encountered = (kaldist_measure[4] > BACK_WALL_CUSHION); 
    
    while (slit_not_encountered) {
        
        front_wall_not_encountered = true;
        back_wall_not_encountered = true;
        
        while (front_wall_not_encountered && slit_not_encountered) {
            wheel_move_by_metrics(FORWARD, 240, 0.125);
            angle_correction_with_ticks (FORWARD, 240);
            
            front_wall_not_encountered = (kaldist_measure[2] > FRONT_WALL_CUSHION || kaldist_measure[3] > FRONT_WALL_CUSHION);
            
            slit_not_encountered = (kaldist_measure[0] < SLIT_EPISLON);      
        }
        
        while (back_wall_not_encountered && slit_not_encountered) {
            
            wheel_move_by_metrics(BACKWARD, 240, 0.125);
            angle_correction_with_ticks (BACKWARD, 240);
            
            back_wall_not_encountered = (kaldist_measure[4] > BACK_WALL_CUSHION);
            
            slit_not_encountered = (kaldist_measure[0] < SLIT_EPISLON);
            
        }
        
        
    }
    
    if (front_wall_not_encountered && back_wall_not_encountered) wheel_move_by_metrics(FORWARD, 240, 0.04); 
    else if (!front_wall_not_encountered) wheel_move_by_metrics(BACKWARD, 240, 0.04);
    
    wheel_move_by_metrics(LEFT, 240, 90);
    angle_correction_with_ticks (LEFT, 240);
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

void findDeckPrelim() {
    
    
    switch(start_base_color) {
        case (RED_BASE):
            if (pos_y > pos_y_base) {
                wheel_move_by_metrics(RIGHT, 240, 90);   
            } else {
                wheel_move_by_metrics(LEFT, 240, 90);  
            }
        
        break;
        case (YELLOW_BASE):
            if (pos_y > pos_y_base) {
                wheel_move_by_metrics(RIGHT, 240, 90);   
            } else {
                wheel_move_by_metrics(LEFT, 240, 90);  
            }
        break;
        case (BLUE_BASE):
            if (pos_y > pos_y_base) {
                wheel_move_by_metrics(LEFT, 240, 90);   
            } else {
                wheel_move_by_metrics(RIGHT, 240, 90);  
            }
        break;
        case (GREEN_BASE):
            if (pos_y > pos_y_base) {
                wheel_move_by_metrics(LEFT, 240, 90);   
            } else {
                wheel_move_by_metrics(RIGHT, 240, 90);  
            }
        break;
    }
    
    CyDelay(500);
    
    bool wall_not_encountered = true;
    
    const double WALL_CUSHION = 25;
    
    //double cumulative_dist = 0;
    
    //double move_dist = (kaldist_measure[2]>kaldist_measure[3])?kaldist_measure[3]:kaldist_measure[2] - WALL_CUSHION;
    
    while (wall_not_encountered) { 
        
        wheel_move_by_metrics(FORWARD, 240, 0.2);
        angle_correction_with_ticks (FORWARD, 240);
        
        wall_not_encountered = (kaldist_measure[2] > WALL_CUSHION && kaldist_measure[3] > WALL_CUSHION);
        
    }
    
    
    angle_correction (240, kaldist_measure[2], kaldist_measure[3]);
    
    wheel_move_by_metrics (BACKWARD, 240, 0.15);
    
    switch(start_base_color) {
        case (RED_BASE):
            wheel_move_by_metrics(RIGHT, 240, 90); 
        break;
        case (YELLOW_BASE):
            wheel_move_by_metrics(LEFT, 240, 90);
        break;
        case (BLUE_BASE):
            wheel_move_by_metrics(LEFT, 240, 90);
        break;
        case (GREEN_BASE):
            wheel_move_by_metrics(RIGHT, 240, 90); 
        break;
    }
    
    double move_dist = (kaldist_measure[2] + kaldist_measure[3])/2;
    
    if (move_dist - 20 < 0) wheel_move_by_metrics(BACKWARD, 240, move_dist - 20);
    else wheel_move_by_metrics(FORWARD, 240, move_dist-20);
    
}

void placePuck() {
    trunk_up();
    trunk_off();
    
    lift_down();
    //arm_flat();
    gripper_open();
    
}

void flickPuck() {
    //gripper_semi_open();
    lift_up();
    //arm_swing_position();
    //arm_flat();
    
    gripper_open();   
    
}

void returnToBase() {
    
    switch(start_base_color) {
        case (RED_BASE):
            wheel_move_by_metrics(RIGHT, 240, 90); 
        break;
        case (YELLOW_BASE):
            wheel_move_by_metrics(LEFT, 240, 90);
        break;
        case (BLUE_BASE):
            wheel_move_by_metrics(LEFT, 240, 90);
        break;
        case (GREEN_BASE):
            wheel_move_by_metrics(RIGHT, 240, 90); 
        break;
    }
    
    angle_correction(240, kaldist_measure[0], kaldist_measure[1]);
    
    wheel_move_by_metrics(FORWARD, 240, 0.2);
    
    switch(start_base_color) {
        case (RED_BASE):
            wheel_move_by_metrics(LEFT, 240, 90); 
        break;
        case (YELLOW_BASE):
            wheel_move_by_metrics(RIGHT, 240, 90);
        break;
        case (BLUE_BASE):
            wheel_move_by_metrics(RIGHT, 240, 90);
        break;
        case (GREEN_BASE):
            wheel_move_by_metrics(LEFT, 240, 90); 
        break;
    }
    
    double move_dist = (kaldist_measure[0] + kaldist_measure[1])/2;
    
    wheel_move_by_metrics(FORWARD, 240, move_dist);
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
