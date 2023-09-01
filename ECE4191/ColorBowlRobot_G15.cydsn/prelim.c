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

void moveOutOfBase() {
    
    bluetooth_start();
    ultrasonic_on();
    CyDelay(500);
    
    bool wall_not_encountered = true;
    
    while (wall_not_encountered) { 
        
        wheel_move_by_metrics(FORWARD, 240, 0.2);
        
        for (int i = 0; i < NUMBER_OF_UDS; i++) {
            printValue("%d:%d, %s", i, (int) kaldist_measure[i], i==NUMBER_OF_UDS-1?"\n":"");   
        }
        
        wall_not_encountered = (kaldist_measure[2] > 28 || kaldist_measure[3] > 28);
        
    }
    
    wheel_move_by_metrics(LEFT, 240, 90);

}

void detectSlit() {
    bool slit_not_encountered = true;
    bool front_wall_not_encountered = true;
    bool back_wall_not_encountered = true;
    
    slit_not_encountered = (kaldist_measure[1] < 80);
    front_wall_not_encountered = (kaldist_measure[2] > 25 || kaldist_measure[3] > 25);
    back_wall_not_encountered = (kaldist_measure[4] > 25); 
    
    while (front_wall_not_encountered && slit_not_encountered) {
        wheel_move_by_metrics(FORWARD, 240, 0.2);
        
        front_wall_not_encountered = (kaldist_measure[2] > 25 || kaldist_measure[3] > 25);
        
        slit_not_encountered = (kaldist_measure[1] < 80);      
    }
    
    while (back_wall_not_encountered && slit_not_encountered) {
        
        wheel_move_by_metrics(BACKWARD, 240, 0.2);
        
        back_wall_not_encountered = (kaldist_measure[4] > 25);
        
        slit_not_encountered = (kaldist_measure[1] < 80);
        
    }
    
    if (front_wall_not_encountered && back_wall_not_encountered) wheel_move_by_metrics(FORWARD, 240, 0.075); 
    else if (!front_wall_not_encountered) wheel_move_by_metrics(BACKWARD, 240, 0.08);
    
    wheel_move_by_metrics(RIGHT, 240, 90);
    
}

void moveThroughSlit() {
    trunk_up();
    gripper_open();
    wheel_move_by_metrics(FORWARD, 240, 0.35); 

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
