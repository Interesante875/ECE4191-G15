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
#include "prelim.h"
#include <cytypes.h>
#include <stdbool.h>

/*
#include "color_detection.h"
#include "locomotion.h"
#include "irsensor.h"
#include "servo.h"
#include "ultrasonic.h"
*/
/*
void move_out_of_base() {
    
    bool wall_not_encountered = true;
    
    ultrasonic_measuring();
    wall_not_encountered = (kaldist_measure[2] > 15 || kaldist_measure[3] > 15);
    
    while (wall_not_encountered) {
        
        wheel_move_by_distance(FORWARD, 240, 0.1);
        
        ultrasonic_measuring();
        wall_not_encountered = (kaldist_measure[2] > 15 || kaldist_measure[3] > 15);
    }
    
    wheel_turn_by_angle(LEFT, 240, 90);
}

void detect_the_slit() {
    
    bool slit_not_encountered = true;
    bool front_wall_not_encountered = true;
    bool back_wall_not_encountered = true;
    
    ultrasonic_measuring();
    
    slit_not_encountered = (kaldist_measure[0] < 80 || kaldist_measure[1] < 80);
    front_wall_not_encountered = (kaldist_measure[2] > 15 || kaldist_measure[3] > 15);
    back_wall_not_encountered = (kaldist_measure[4] > 15); 
    
    while (front_wall_not_encountered && slit_not_encountered) {
        wheel_move_by_distance(FORWARD, 240, 0.1);
        
        ultrasonic_measuring();
        front_wall_not_encountered = (kaldist_measure[2] > 15 || kaldist_measure[3] > 15);
        
        slit_not_encountered = (kaldist_measure[0] < 80 || kaldist_measure[1] < 80);      
    }
    
    while (back_wall_not_encountered && slit_not_encountered) {
        
        wheel_move_by_distance(BACKWARD, 240, 0.1);
        
        ultrasonic_measuring();
        back_wall_not_encountered = (kaldist_measure[4] > 15);
        
        slit_not_encountered = (kaldist_measure[0] < 80 || kaldist_measure[1] < 80);
        
    }
    
    wheel_turn_by_angle(RIGHT, 240, 90);
    
}

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
