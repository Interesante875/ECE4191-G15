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
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

/*
#include "ultrasonic.h"
#include "bluetooth.h"
#include "locomotion.h"
#include "color_detection.h"
#include "servo.h"
*/


#include "color_detection.h"
#include "locomotion.h"
#include "irsensor.h"
#include "servo.h"
#include "ultrasonic.h"
#include "bluetooth.h"

void move_out_of_base() {
    
    
    bool wall_not_encountered = true;
    
    ultrasonic_measuring();
    
    
    wall_not_encountered = (kaldist_measure[2] > 20 || kaldist_measure[3] > 20);
    
    //printValue("Distance: %d %d\n", (int) kaldist_measure[2], (int) kaldist_measure[3]);
    while (wall_not_encountered) {
        
        wheel_move_by_distance(FORWARD, 240, 0.15);
        
        ultrasonic_measuring();
        
        wall_not_encountered = (kaldist_measure[2] > 25 || kaldist_measure[3] > 25);
        
        angle_correction(240, (int) kaldist_measure[2], (int) kaldist_measure[3]);
        
        ultrasonic_measuring();
        //printValue("%d %d\n", (int) kaldist_measure[2], (int) kaldist_measure[3]);
    }
    
    wheel_turn_by_angle(LEFT, 240, 90);
}

void detect_the_slit() {
    
    bool slit_not_encountered = true;
    bool front_wall_not_encountered = true;
    bool back_wall_not_encountered = true;
    
    ultrasonic_measuring();
    
    
    slit_not_encountered = (kaldist_measure[0] < 80 && kaldist_measure[1] < 80);
    front_wall_not_encountered = (kaldist_measure[2] > 15 || kaldist_measure[3] > 15);
    back_wall_not_encountered = (kaldist_measure[4] > 15); 
    
    while (front_wall_not_encountered && slit_not_encountered) {
        wheel_move_by_distance(FORWARD, 240, 0.1);
        
        ultrasonic_measuring();
        front_wall_not_encountered = (kaldist_measure[2] > 20 || kaldist_measure[3] > 20);
        
        slit_not_encountered = (kaldist_measure[0] < 75 && kaldist_measure[1] < 75);  
        
        angle_correction(240, (int) kaldist_measure[2], (int) kaldist_measure[3]);
        
        ultrasonic_measuring();
    }
    
    while (back_wall_not_encountered && slit_not_encountered) {
        
        wheel_move_by_distance(BACKWARD, 240, 0.1);
        
        ultrasonic_measuring();
        back_wall_not_encountered = (kaldist_measure[4] > 15);
        
        slit_not_encountered = (kaldist_measure[0] < 75 && kaldist_measure[1] < 75);
        
        //angle_correction(240, (int) kaldist_measure[2], (int) kaldist_measure[3]);
        
        ultrasonic_measuring();
        
    }
    
    wheel_turn_by_angle(RIGHT, 240, 90);
    
}

void move_through_slit() {
    trunk_up();
    wheel_move_by_distance(FORWARD, 240, 0.5);   

}

int main(void)
{
    CyGlobalIntEnable; /* Enable global interrupts. */
    bluetooth_start();
    /*
    
    
    lift_up();
    */
    
    for (int i = 0; i < 1; i++)
    wheel_move_by_ticks(FORWARD, 240, 10000);
    
    for (int i = 0; i < 1; i++)
    wheel_move_by_ticks(BACKWARD, 240, 10000);
    
    for (int i = 0; i < 1; i++)
    wheel_turn_by_ticks(LEFT, 240, 20000);
    
    
    for (int i = 0; i < 1; i++)
    wheel_turn_by_ticks(RIGHT, 240, 20000);
    
    for (int i = 0; i < 1; i++)
    wheel_move_by_ticks(BACKWARD, 240, 20000);
    
    for (int i = 0; i < 1; i++)
    wheel_move_by_ticks(FORWARD, 240, 10000);
    
   
    for(;;)
    {
        
    }
}

/* [] END OF FILE */
