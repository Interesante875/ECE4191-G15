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
    
    bluetooth_start();
    ultrasonic_on();
    
}

void moveOutOfBaseFast() {
    
    
    
    CyDelay(500);
    
    bool wall_not_encountered = true;
    
    const double WALL_CUSHION = 25;
    
    wheel_move(FORWARD, PWM_VAL);
    
    while (wall_not_encountered) {
        
        wall_not_encountered = (kaldist_measure[2] > WALL_CUSHION && kaldist_measure[3] > WALL_CUSHION);
        
    }
    
    wheel_move(STOP, 0);
    
    wheel_move_by_metrics(RIGHT, PWM_VAL, 90);
//    CyDelay(500);
//    angle_correction(240, kaldist_measure[2], kaldist_measure[3]);
    
}

void moveOutOfBase() {
    
    bluetooth_start();
    
    trunk_up();
    trunk_off();
    ultrasonic_on();
    CyDelay(500);
    
    bool wall_not_encountered = true;
    bool wall_not_too_far = true;
    
    const double WALL_CUSHION = 25;
    
    //double cumulative_dist = 0;
    
    //double move_dist = (kaldist_measure[2]>kaldist_measure[3])?kaldist_measure[3]:kaldist_measure[2] - WALL_CUSHION;
    
    while (wall_not_encountered && wall_not_too_far) { 
        
        wheel_move_by_metrics(FORWARD, PWM_VAL, 0.15);
        //angle_correction_with_ticks (FORWARD, 240);
        
//        for (int i = 0; i < NUMBER_OF_UDS; i++) {
//            printValue("%d:%d, %s", i, (int) kaldist_measure[i], i==NUMBER_OF_UDS-1?"\n":"");   
//        }
        
        wall_not_encountered = (kaldist_measure[2] > WALL_CUSHION || kaldist_measure[3] > WALL_CUSHION);
        //wall_not_too_far = (kaldist_measure[2] < 140 || kaldist_measure[3] < 140);
    }
    
    //if (!wall_not_too_far) wheel_move_by_metrics(FORWARD, 240, 1.0);
    
    wheel_move_by_metrics(RIGHT, PWM_VAL, 90);
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
    
//    CyDelay(500);
//    angle_correction(240, kaldist_measure[2], kaldist_measure[3]);
    
    slit_not_encountered = (kaldist_measure[0] < SLIT_EPISLON);
    
    front_wall_not_encountered = (kaldist_measure[2] > FRONT_WALL_CUSHION || kaldist_measure[3] > FRONT_WALL_CUSHION);
    back_wall_not_encountered = (kaldist_measure[4] > BACK_WALL_CUSHION); 
    
    while (slit_not_encountered) {
        
        front_wall_not_encountered = true;
        back_wall_not_encountered = true;
        
        while (front_wall_not_encountered && slit_not_encountered) {
            
            wheel_move(FORWARD, PWM_VAL);
            
//            CyDelay(500);
//            angle_correction(240, kaldist_measure[2], kaldist_measure[3]);
            //angle_correction(240, kaldist_measure[2], kaldist_measure[3]);
            
            front_wall_not_encountered = (kaldist_measure[2] > FRONT_WALL_CUSHION || kaldist_measure[3] > FRONT_WALL_CUSHION);
            
            slit_not_encountered = (kaldist_measure[0] < SLIT_EPISLON);      
        }
        
        wheel_move(STOP, 0);
        
//        CyDelay(500);
//        angle_correction (PWM_VAL, kaldist_measure[2], kaldist_measure[3]);

        while (back_wall_not_encountered && slit_not_encountered) {
            
            wheel_move(BACKWARD, PWM_VAL);
            //angle_correction_with_ticks (BACKWARD, 240);
            
            back_wall_not_encountered = (kaldist_measure[4] > BACK_WALL_CUSHION);
            
            slit_not_encountered = (kaldist_measure[0] < SLIT_EPISLON);
            
        }
        
        wheel_move(STOP, 0);
        
        
    }
    
    if (front_wall_not_encountered && back_wall_not_encountered) wheel_move_by_metrics(FORWARD, 240, 0.08); 
    else if (!front_wall_not_encountered) wheel_move_by_metrics(BACKWARD, 240, 0.12);
    
    wheel_move_by_metrics(LEFT, PWM_VAL, 90);
    
//    CyDelay(500);
//    angle_correction(240, kaldist_measure[2], kaldist_measure[3]);
    //angle_correction_with_ticks (LEFT, 240);
}

void prepMoveThroughSlit() {
    trunk_up();
    gripper_full_close();
    lift_down();
    CyDelay(200);
    arm_swing_flat();
    gripper_open();
    
}

void moveThroughSlit() {
    
    wheel_move_by_metrics(FORWARD, PWM_VAL, 0.45); 
    
    
    
}

void grabPuck() {
    
    gripper_close();
    
    
    trunk_ground();
    CyDelay(250);
    
    color_sensor_start();
    set_frequency_scaling(1, 1);
    color_detection_run(COLOR_DETECTION_RUNS);
    
    switch(detectedColor) {
        case RED:
            Indicator_RED_Write(1); 
            CyDelay(2000);
            break;
        case BLUE:
            Indicator_BLUE_Write(1);
            CyDelay(2000);
            break;
        case GREEN:
            Indicator_GREEN_Write(1);
            CyDelay(2000);
            break;
        default:
            Indicator_RED_Write(0);
            Indicator_BLUE_Write(0);
            Indicator_GREEN_Write(0);
            CyDelay(1000);
            break;
    }
    
    color_sensor_stop();
    
//    gripper_close();
    trunk_up();
    //lift_up();
    
    
}

void findDeckPrelim() {
    
    wheel_move_by_metrics(BACKWARD, PWM_VAL, 0.5);
    
    switch(start_base_color) {
        case (RED_BASE):
            wheel_move_by_metrics(RIGHT, PWM_VAL, 90);   
        break;
        case (YELLOW_BASE):
            wheel_move_by_metrics(LEFT, PWM_VAL, 90);
        break;
        case (BLUE_BASE):
            wheel_move_by_metrics(LEFT, PWM_VAL, 90);  
        break;
        case (GREEN_BASE):
            wheel_move_by_metrics(RIGHT, PWM_VAL, 90);
        break;
    }
    
    //CyDelay(500);
    //angle_correction(240, kaldist_measure[2], kaldist_measure[3]);
    
    const double WALL_CUSHION = 27;
    
    bool wall_not_encountered = true;
    
    bool wheel_sync = true;

    wheel_move(FORWARD, 240);
    
    while (wall_not_encountered) {
        
        wall_not_encountered = (kaldist_measure[2] > WALL_CUSHION && kaldist_measure[3] > WALL_CUSHION);
        
    }
    
    wheel_move(STOP, 0);
    
    CyDelay(500);
    angle_correction(PWM_VAL, kaldist_measure[2], kaldist_measure[3]);
    
    wheel_move_by_metrics (BACKWARD, PWM_VAL, 0.05);
    
    CyDelay(500);
    angle_correction (PWM_VAL, kaldist_measure[2], kaldist_measure[3]);
    
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
    
    
    CyDelay(500);
    angle_correction (PWM_VAL, kaldist_measure[2], kaldist_measure[3]);
    
    
    wall_not_encountered = true;

    wheel_move(FORWARD, PWM_VAL);
    
    while (wall_not_encountered) {
        
        wall_not_encountered = (kaldist_measure[2] > WALL_CUSHION || kaldist_measure[3] > WALL_CUSHION);
        
    }
    
    wheel_move(STOP, 0);
    
    CyDelay(500);
    angle_correction (240, kaldist_measure[2], kaldist_measure[3]);
    
    
}


void adjustDistance() {
    
    double wall_distance_not_fit = true;
    
    const double WALL_DISTANCE = 22;
    
    CyDelay(500);
    wall_distance_not_fit = (kaldist_measure[2] < WALL_DISTANCE && kaldist_measure[3] < WALL_DISTANCE);
    
    
    wheel_move(BACKWARD, PWM_VAL);
    
    
    while (wall_distance_not_fit) {
        
        wall_distance_not_fit = (kaldist_measure[2] < WALL_DISTANCE && kaldist_measure[3] < WALL_DISTANCE);
    }
    
    wheel_move(STOP, 0);
    
    CyDelay(500);
    angle_correction (240, kaldist_measure[2], kaldist_measure[3]);
    
    
}

void placePuck() {
    trunk_up();
    //lift_down();
    gripper_open();
    
}

void flickPuck() {
    
    double move_dist = 0.25;

    wheel_move_by_metrics(BACKWARD, PWM_VAL, move_dist - 0.15);
    trunk_up();
    gripper_full_close();
    lift_up();
    arm_swing_flick();
    wheel_move_by_metrics(FORWARD, PWM_VAL, move_dist-0.05);
    arm_swing_flat(); 
   
    
}

void returnToBase() {
    
    switch(start_base_color) {
        case (RED_BASE):
            wheel_move_by_metrics(LEFT, PWM_VAL, 90); 
        break;
        case (YELLOW_BASE):
            wheel_move_by_metrics(RIGHT, PWM_VAL, 90);
        break;
        case (BLUE_BASE):
            wheel_move_by_metrics(RIGHT, PWM_VAL, 90);
        break;
        case (GREEN_BASE):
            wheel_move_by_metrics(LEFT, PWM_VAL, 90); 
        break;
    }
    
    CyDelay(500);
    
    angle_correction(PWM_VAL, kaldist_measure[2], kaldist_measure[3]);
    
    CyDelay(500);
    
    bool wall_distance_not_enough = true;
    
    const double WALL_CUSHION = 60;
    
    wheel_move(BACKWARD, PWM_VAL);
    
    while (wall_distance_not_enough) {
        
        wall_distance_not_enough = (kaldist_measure[2] < WALL_CUSHION || kaldist_measure[3] < WALL_CUSHION);
        
    }
    
    wheel_move(STOP, 0);
    
    switch(start_base_color) {
        case (RED_BASE):
            wheel_move_by_metrics(RIGHT, PWM_VAL, 90); 
        break;
        case (YELLOW_BASE):
            wheel_move_by_metrics(LEFT, PWM_VAL, 90);
        break;
        case (BLUE_BASE):
            wheel_move_by_metrics(LEFT, PWM_VAL, 90);
        break;
        case (GREEN_BASE):
            wheel_move_by_metrics(RIGHT, PWM_VAL, 90); 
        break;
    }
    
    CyDelay(500);
    
    angle_correction(PWM_VAL, kaldist_measure[2], kaldist_measure[3]);
    
    wheel_move_by_metrics(RIGHT, PWM_VAL, 180); 
    
    
    CyDelay(500);
    double move_dist = (kaldist_measure[4])/(100);
    
    wheel_move_by_metrics(BACKWARD, PWM_VAL, move_dist); 
    
    //wheel_move_by_metrics(FORWARD, PWM_VAL, move_dist - 0.05);
}

void test() {
    wheel_move_by_metrics(RIGHT, PWM_VAL, 90);
//    gripper_full_close();
//    lift_down();
//    gripper_full_close();
//    lift_up();
//    arm_swing_flick();
    
    //angle_correction_with_ticks (LEFT, PWM_VAL);
    
    
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
