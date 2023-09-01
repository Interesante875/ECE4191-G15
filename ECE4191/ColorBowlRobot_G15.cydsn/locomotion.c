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
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "locomotion.h"
#include "bluetooth.h"

volatile int master_pwm = 0;
volatile int slave_pwm = 0;
volatile int master_motor_left_ticks = 0;
volatile int slave_motor_right_ticks = 0;

int last_master_ticks = 0;
int last_slave_ticks = 0;

CY_ISR (ISR_Handler_wheel_controller) {
    
    Wheel_Timer_ReadStatusRegister();
    
    master_pwm = motor_left_get_pwm();
        
    uint8 updated_slave_pwm = computePController(master_motor_left_ticks, 
            slave_motor_right_ticks, master_pwm);

    motor_right_set_pwm_compare((uint8) updated_slave_pwm);
    master_motor_left_ticks = -motor_left_get_count_QuadDec();
    slave_motor_right_ticks = -motor_right_get_count_QuadDec();
    
    
    
}

void wheel_controller_start() {
    Wheel_Timer_Start();
    isr_wheel_controller_StartEx(ISR_Handler_wheel_controller);
    
}

void wheel_controller_stop() {
    Wheel_Timer_Stop();
    isr_wheel_controller_Stop(); 
}

void wheel_move_by_ticks(MOTION motion, uint8 pwm, int target_ticks) {
     /**
     * Move the wheels of the robot by a specified number of quad decoder ticks using a closed-loop control approach.
     *
     * This function moves the robot's wheels by a specified number of encoder ticks using a closed-loop control approach.
     * It adjusts the slave wheel's PWM signal based on the difference in encoder ticks between the master and slave wheels,
     * applying a proportional correction to maintain synchronization.
     *
     * @param motion        The desired motion of the robot wheels (FORWARD, BACKWARD, LEFT, RIGHT, or STOP).
     * @param pwm           The PWM signal value for controlling both motors.
     * @param target_ticks  The target number of encoder ticks to move the wheels.
     */

    motor_on(pwm);
    wheel_controller_start();
    
    master_pwm = motor_left_get_pwm();
    slave_pwm = motor_right_get_pwm();
    
    master_motor_left_ticks = -motor_left_get_count_QuadDec();
    slave_motor_right_ticks = -motor_right_get_count_QuadDec();
    
    motor_motion_set(motion);
    
    while (abs(master_motor_left_ticks) < target_ticks);
    
    printValue("LEFT: %d\t RIGHT: %d\n ", master_motor_left_ticks, slave_motor_right_ticks);
    printValue("Master PWM - %d Slave PWM - %d\n", master_pwm, slave_pwm);
    
    wheel_controller_stop();
    motor_off();
    
    CyDelay(EMF_BUFFER_DELAY);
}

void wheel_move_by_metrics (MOTION motion, uint8 pwm, double metrics) {
    
    int ticks = 0;
    
    if (motion == FORWARD || motion == BACKWARD) {
        double linear_ticks = (metrics * TICKS_PER_REVOLUTION)/(2 * CY_M_PI * WHEEL_RADIUS);
        ticks = (int) linear_ticks;
    } else if (motion == LEFT || motion == RIGHT) {
        double circum_distance = metrics * CY_M_PI / 360 * WHEEL_DISTANCE;
        double n_revs = circum_distance/(2 * CY_M_PI * WHEEL_RADIUS);
        ticks = (int) TICKS_PER_REVOLUTION * n_revs;

    } else {
        ticks = 0;
    }
    
    motor_on(pwm);
    wheel_controller_start();
    
    master_pwm = motor_left_get_pwm();
    slave_pwm = motor_right_get_pwm();
    
    master_motor_left_ticks = -motor_left_get_count_QuadDec();
    slave_motor_right_ticks = -motor_right_get_count_QuadDec();
    
    motor_motion_set(motion);
    
    while (abs(master_motor_left_ticks) < ticks);
    
    printValue("LEFT: %d\t RIGHT: %d\n ", master_motor_left_ticks, slave_motor_right_ticks);
    printValue("Master PWM - %d Slave PWM - %d\n", master_pwm, slave_pwm);
    
    wheel_controller_stop();
    motor_off();
    
    CyDelay(EMF_BUFFER_DELAY);
}


double my_atan(double x)
{
    return x - (x*x*x)/3 + (x*x*x*x*x)/5 - (x*x*x*x*x*x*x)/7;
}

void angle_correction(uint8 pwm, double flu, double fru) {
    
    double lr = flu - fru;
    
    if (fabs(lr) > 1) {
        double angle = my_atan(0.01*fabs(lr)/0.2) * 180 / CY_M_PI;
        
        printValue("Angle: %d lr:%d\n", (int) angle, lr);
        
        if (lr > 0) {
           wheel_move_by_metrics(RIGHT, pwm, angle);
        } else {
           wheel_move_by_metrics(LEFT, pwm, angle);
        }
    }
    
    
}

void robot_locomotion_initialization() {
    moveStatus = ENABLE;
    currentMotion = STOP;
}

/* [] END OF FILE */
