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
#include "servo.h"
#include "bluetooth.h"
#include "prelim.h"
#include "ultrasonic.h"
/*
void detect_the_slit() {
    
    bool slit_not_encountered = true;
    bool front_wall_not_encountered = true;
    bool back_wall_not_encountered = true;
    
    bool front_complete = false;
    bool back_complete = false;
    
    ultrasonic_measuring();
    
    slit_not_encountered = (kaldist_measure[0] < 80 && kaldist_measure[1] < 80);
    front_wall_not_encountered = (kaldist_measure[2] > 20 || kaldist_measure[3] > 20);
    back_wall_not_encountered = (kaldist_measure[4] > 20); 
    
    while (front_wall_not_encountered && slit_not_encountered) {
        wheel_move_by_distance(FORWARD, 240, 0.1);
        

        ultrasonic_measuring();
        front_wall_not_encountered = (kaldist_measure[2] > 20 || kaldist_measure[3] > 20);
        
        slit_not_encountered = (kaldist_measure[0] < 75 && kaldist_measure[1] < 75);  
        
    
        //angle_correction(240, (int) kaldist_measure[2], (int) kaldist_measure[3]);
        
  
        ultrasonic_measuring();
        
        front_complete = true;
    }
    
    
    
    while (back_wall_not_encountered && slit_not_encountered) {
        
        
        wheel_move_by_distance(BACKWARD, 240, 0.1);

        ultrasonic_measuring();

        back_wall_not_encountered = (kaldist_measure[4] > 15);
       
        slit_not_encountered = (kaldist_measure[0] < 75 && kaldist_measure[1] < 75);
        
        //angle_correction(240, (int) kaldist_measure[2], (int) kaldist_measure[3]);
        
        ultrasonic_measuring();

        back_complete = true;
    }
   
    if (front_complete && !back_complete)
    wheel_move_by_distance(FORWARD, 240, 0.075);
    else if (front_complete && back_complete)
    wheel_move_by_distance(BACKWARD, 240, 0.075);
    wheel_turn_by_angle(RIGHT, 240, 90);
    
}

void move_through_slit() {
    
    //trunk_up();
    
    //gripper_open();
    wheel_move_by_distance(FORWARD, 240, 0.5);   
    //gripper_close();
}

void move_out_of_base_2() {
    
    bool wall_not_encountered = true;

    
    
    
}
*/
int main(void)
{
    CyGlobalIntEnable; /* Enable global interrupts. */
    
    moveOutOfBase();
    CyDelay(500);
    detectSlit();
    moveThroughSlit();
//    gripper_open();
//    CyDelay(250);
//    gripper_close();
//        CyDelay(1000);
    
    
    for(;;)
    {
        
        
      
    }
}

/* [] END OF FILE */
