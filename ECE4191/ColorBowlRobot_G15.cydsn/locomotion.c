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
#include "controller.h"
#include "navigation_stack.h"

volatile int master_pwm = 0;
volatile int slave_pwm = 0;
volatile int master_motor_left_ticks = 0;
volatile int slave_motor_right_ticks = 0;

int last_master_ticks = 0;
int last_slave_ticks = 0;

int period = 0;

CY_ISR (ISR_Handler_wheel_controller) {
    
    
    Wheel_Timer_ReadStatusRegister();
    
    if (period > 4) {
    
        master_pwm = motor_left_get_pwm();
            
        uint8 updated_slave_pwm = computePController(master_motor_left_ticks, 
                slave_motor_right_ticks, master_pwm);

        motor_right_set_pwm_compare((uint8) updated_slave_pwm);
        master_motor_left_ticks = -motor_left_get_count_QuadDec();
        slave_motor_right_ticks = -motor_right_get_count_QuadDec();
        
        //printValue("%d %d\n", master_motor_left_ticks, slave_motor_right_ticks);
        
       
        int left = master_motor_left_ticks - last_master_ticks;
        int right = slave_motor_right_ticks - slave_motor_right_ticks;
        
        computePosition(left, right);
        
        last_master_ticks = master_motor_left_ticks;
        last_slave_ticks = slave_motor_right_ticks;
    
    }
    
    period++;
    
}

void control() {
    
    if (period > 4) {
    
        master_pwm = motor_left_get_pwm();
            
        uint8 updated_slave_pwm = computePController(master_motor_left_ticks, 
                slave_motor_right_ticks, master_pwm);

        motor_right_set_pwm_compare((uint8) updated_slave_pwm);
        master_motor_left_ticks = -motor_left_get_count_QuadDec();
        slave_motor_right_ticks = -motor_right_get_count_QuadDec();
        
        //printValue("%d %d\n", master_motor_left_ticks, slave_motor_right_ticks);
        
       
        int left = master_motor_left_ticks - last_master_ticks;
        int right = slave_motor_right_ticks - slave_motor_right_ticks;
        
        computePosition(left, right);
        
        last_master_ticks = master_motor_left_ticks;
        last_slave_ticks = slave_motor_right_ticks;
    
    }
    
    period++;
    
}

void wheel_controller_start() {
    Wheel_Timer_Start();
    isr_wheel_controller_StartEx(ISR_Handler_wheel_controller);
    
    CyDelay(START_MOTOR_DELAY);
    
    period = 0;
    
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
    
    //flag_distance_moving = 1;
    wheel_controller_start();
    motor_on(pwm);
    
    
    
    
    last_master_ticks = 0;
    last_slave_ticks = 0;
    
    master_pwm = motor_left_get_pwm();
    slave_pwm = motor_right_get_pwm();
    
    master_motor_left_ticks = -motor_left_get_count_QuadDec();
    slave_motor_right_ticks = -motor_right_get_count_QuadDec();
    
    motor_motion_set(motion);
    
    while (abs(master_motor_left_ticks) < target_ticks);
    
    printValue("LEFT: %d\t RIGHT: %d\n ", master_motor_left_ticks, slave_motor_right_ticks);
    printValue("Master PWM - %d Slave PWM - %d\n", master_pwm, slave_pwm);
    
    printValue("%d, %d, %d\n", pos_x, pos_y, heading_angle);
    
    wheel_controller_stop();
    motor_off();
    
    CyDelay(EMF_BUFFER_DELAY);
}

void wheel_move_by_metrics (MOTION motion, uint8 pwm, double metrics) {
    
    //flag_distance_moving = 1;
    last_master_ticks = 0;
    last_slave_ticks = 0;
    int ticks = 0;
    
    if (motion == FORWARD || motion == BACKWARD) {
        double linear_ticks = (metrics * TICKS_PER_REVOLUTION)/(2 * CY_M_PI * WHEEL_RADIUS);
        ticks = (int) linear_ticks;
        
        printValue("TARGET: %d\n", ticks);
    } else if (motion == LEFT || motion == RIGHT) {
        double circum_distance = metrics * CY_M_PI / 360 * WHEEL_DISTANCE;
        double n_revs = circum_distance/(2 * CY_M_PI * WHEEL_RADIUS);
        ticks = (int) TICKS_PER_REVOLUTION * n_revs;

    } else {
        ticks = 0;
    }
    
    //wheel_controller_start();
    motor_on(pwm);
    
    
    master_pwm = motor_left_get_pwm();
    slave_pwm = motor_right_get_pwm();
    
    master_motor_left_ticks = -motor_left_get_count_QuadDec();
    slave_motor_right_ticks = -motor_right_get_count_QuadDec();
    
    motor_motion_set(motion);
    
    while (abs(master_motor_left_ticks) < ticks) {
        //printValue("%d\n", master_motor_left_ticks);  
        control();
    }
    
    printValue("LEFT: %d\t RIGHT: %d\n ", master_motor_left_ticks, slave_motor_right_ticks);
    printValue("Master PWM - %d Slave PWM - %d\n", master_pwm, slave_pwm);
    
    last_master_ticks = master_motor_left_ticks;
    last_slave_ticks = slave_motor_right_ticks;
    
    //wheel_controller_stop();
    motor_off();
    
    CyDelay(EMF_BUFFER_DELAY);
}

void wheel_move (MOTION motion, uint8 pwm) {
    
    //flag_distance_moving = 0;
    last_master_ticks = 0;
    last_slave_ticks = 0;
    
    if (motion == LEFT || motion == RIGHT) return;
    
    if (motion == STOP) {
        
        //flag_distance_moving = 1;
    
        wheel_controller_stop();
        motor_off();
        
        
        CyDelay(EMF_BUFFER_DELAY);
    }
    
    wheel_controller_start();
    motor_on(pwm);
    
    
    master_pwm = motor_left_get_pwm();
    slave_pwm = motor_right_get_pwm();
    
    master_motor_left_ticks = -motor_left_get_count_QuadDec();
    slave_motor_right_ticks = -motor_right_get_count_QuadDec();
    
    motor_motion_set(motion);
    
}

void angle_correction_with_ticks (MOTION motion, uint8 pwm) {
    
    last_master_ticks = 0;
    last_slave_ticks = 0;
    
    if (motion == STOP) return;
    if (pwm < 200) return;
    
    if (abs(last_master_ticks) - abs(last_slave_ticks) > 10) {
        switch (motion) {
            case FORWARD:
                wheel_move_by_ticks(LEFT, pwm, abs(last_master_ticks) - abs(last_slave_ticks));
            break;
            case BACKWARD:
                wheel_move_by_ticks(RIGHT, pwm, abs(last_master_ticks) - abs(last_slave_ticks));
            break;
            case LEFT:
                wheel_move_by_ticks(LEFT, pwm, abs(last_master_ticks) - abs(last_slave_ticks));
            break;
            case RIGHT:
                wheel_move_by_ticks(RIGHT, pwm, abs(last_master_ticks) - abs(last_slave_ticks));
            break;
            default:
            break;
        }
        
    } else if (abs(last_master_ticks) - abs(last_slave_ticks) < -10) {
        switch (motion) {
            case FORWARD:
                wheel_move_by_ticks(RIGHT, pwm, abs(last_master_ticks) - abs(last_slave_ticks));
            break;
            case BACKWARD:
                wheel_move_by_ticks(LEFT, pwm, abs(last_master_ticks) - abs(last_slave_ticks));
            break;
            case LEFT:
                wheel_move_by_ticks(LEFT, pwm, abs(last_master_ticks) - abs(last_slave_ticks));
            break;
            case RIGHT:
                wheel_move_by_ticks(RIGHT, pwm, abs(last_master_ticks) - abs(last_slave_ticks));
            break;
            default:
            break;
        }
        
    } else return;
    
}

void angle_correction(uint8 pwm, double flu, double fru) {
    
    last_master_ticks = 0;
    last_slave_ticks = 0;
    
    double lr = (double) (flu - fru);
    
    if (fabs(lr) > 1) {
        double angle = atan2(0.01*fabs(lr), 0.2) * 180 / CY_M_PI;
        
        if (angle > 15) angle = 15;
        else if (angle < -15) angle = -15;
        
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
