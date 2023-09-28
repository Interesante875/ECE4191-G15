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
#include "locomotion.h"
#include "motor_control.h"
#include "bluetooth.h"
#include "motor.h"
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <stdbool.h>

volatile int masterPWM = 0;
volatile int slavePWM = 0;
volatile int masterLeftTicks = 0;
volatile int slaveRightTicks = 0;

int lastMasterTicks = 0;
int lastSlaveTicks = 0;

int period = 0;

void wheel_move_by_ticks(MotionDirection motion, int pwm, int target_ticks) {
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
    initializeWheelController(ProportionalControl);

    turnMotorOn(pwm);


    lastMasterTicks = 0;
    lastSlaveTicks = 0;
    
    masterPWM = MotorController_GetLeftPwm();
    slavePWM = MotorController_GetRightPwm();
    
    masterLeftTicks = MotorController_GetLeftQuadDecCount();
    slaveRightTicks = MotorController_GetRightQuadDecCount();
    
    setMotionDirection(motion);
    
    while (abs(masterLeftTicks) < target_ticks);
    
    
    printValue("LEFT: %d\t RIGHT: %d\n ", masterLeftTicks, slaveRightTicks);
    printValue("Master PWM - %d Slave PWM - %d\n", masterPWM, slaveRightTicks);
//    
//    printValue("%d, %d, %d\n", pos_x, pos_y, heading_angle);
    
    stopWheelController();
    stopMotor();
    
    CyDelay(EMF_BUFFER_DELAY);
}

/* [] END OF FILE */
