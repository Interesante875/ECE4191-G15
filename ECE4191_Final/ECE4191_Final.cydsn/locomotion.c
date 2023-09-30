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
#include "p_ctrl.h"
#include "pid_ctrl.h"
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <stdbool.h>

#define EMF_BUFFER_DELAY 50

static volatile int masterPWM = 0;
static volatile int slavePWM = 0;
static volatile int masterLeftTicks = 0;
static volatile int slaveRightTicks = 0;
int target_move_ticks = 0;
int stop_flag = 0;
int lastMasterTicks = 0;
int lastSlaveTicks = 0;

int period = 0;

ControllerType ctrlType;

void initializeWheelController(ControllerType ctrlType_name) {
    ctrlType = ctrlType_name;
    switch (ctrlType) {
        case ProportionalControl:
            resetPController();
            printValue("Proportional Controller is used\n");
        break;
        
        case ProportionalIntegralDerivativeControl:
            printValue("PID Controller is used\n");
            resetPIDController();
        break; 
        default:
            printValue("Proportional Controller is used\n");
            resetPController();
        break;
    }
    
    Timer_Wheel_Start();
    isr_wheel_controller_StartEx(ISR_Handler_Wheel_Controller);
}

void stopWheelController() {
    
    Timer_Wheel_Stop();
    isr_wheel_controller_Stop();
}

CY_ISR(ISR_Handler_Wheel_Controller) {
    
    Timer_Wheel_ReadStatusRegister();
    
    masterPWM = MotorController_GetLeftPwm();
    uint8 updated_slave_pwm = 0;
    
    switch (ctrlType) {
        case ProportionalControl:
            updated_slave_pwm = computePController(masterLeftTicks, 
                slaveRightTicks, 
                masterPWM);  
        break;
        
        case ProportionalIntegralDerivativeControl:
            updated_slave_pwm = computePIDController(masterLeftTicks, 
                slaveRightTicks, 
                masterPWM);
        break;
        
        default:
            updated_slave_pwm = computePController(masterLeftTicks, 
                slaveRightTicks, 
                masterPWM);
        break; 
    }
    MotorController_SetLeftPwmCompare((uint8) updated_slave_pwm);
    masterLeftTicks = MotorController_GetLeftQuadDecCount();
    slaveRightTicks = MotorController_GetRightQuadDecCount();
    
    
//    printValue("Target: %d LEFT: %d RIGHT: %d\n ", target_move_ticks, masterLeftTicks, slaveRightTicks);
//    printValue("Master PWM: %d Slave PWM: %d\n", masterPWM, updated_slave_pwm);

}

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

    turnMotorOn(pwm);
//    printValue("Turn Motor On\n");
    lastMasterTicks = 0;
    lastSlaveTicks = 0;
    
    masterPWM = MotorController_GetLeftPwm();
    slavePWM = MotorController_GetRightPwm();
//    printValue("Got PWM\n");
    
    masterLeftTicks = MotorController_GetLeftQuadDecCount();
    slaveRightTicks = MotorController_GetRightQuadDecCount();
//    printValue("Got Counts\n");
    target_move_ticks = target_ticks;
    
    setMotionDirection(motion);
//    printValue("Set motion\n");
    initializeWheelController(ProportionalControl);
    
    printValue("Set Controller\n");
    while (abs(masterLeftTicks) < target_ticks){
        //masterLeftTicks = MotorController_GetLeftQuadDecCount();
        //slaveRightTicks = MotorController_GetRightQuadDecCount();
        //printValue("LEFT: %d RIGHT: %d\n ", masterLeftTicks, slaveRightTicks);
        // printValue("Master PWM: %d Slave PWM: %d\n", masterPWM, updated_slave_pwm);   
    }
    printValue("DONE\n");
    printValue("LEFT: %d RIGHT: %d\n ", masterLeftTicks, slaveRightTicks);
    printValue("Master PWM: %d Slave PWM: %d\n", masterPWM, slavePWM);
 
    stopWheelController();
    stopMotor();
    
    CyDelay(EMF_BUFFER_DELAY);
}

void wheel_move_by_metrics (MotionDirection motion, uint8 pwm, double metrics) {
    
    int ticks = 0;
    
    if (motion == Forward || motion == Backward) {
        double linear_ticks = (metrics * TICKS_PER_REVOLUTION)/(2 * CY_M_PI * WHEEL_RADIUS);
        ticks = (int) linear_ticks;
        
    } else if (motion == Left || motion == Right) {
        double circum_distance = metrics * CY_M_PI / 360 * WHEEL_DISTANCE;
        double n_revs = circum_distance/(2 * CY_M_PI * WHEEL_RADIUS);
        ticks = (int) TICKS_PER_REVOLUTION * n_revs;

    } else {
        ticks = 0;
    }
    
    initializeWheelController(USE_CONTROLLER);
    turnMotorOn(pwm);

    masterPWM = MotorController_GetLeftPwm();
    slavePWM = MotorController_GetRightPwm();
    
    masterLeftTicks = MotorController_GetLeftQuadDecCount();
    slaveRightTicks = MotorController_GetRightQuadDecCount();
    
    setMotionDirection(motion);
    
    while (abs(masterLeftTicks) < ticks);
    
    stopWheelController();
    stopMotor();

    CyDelay(EMF_BUFFER_DELAY);
}

void wheel_move (MotionDirection motion, uint8 pwm) {

    if (motion == Left || motion == Right) return;
    
    if (motion == StopMotion) {

        stopWheelController();
        stopMotor();

        CyDelay(EMF_BUFFER_DELAY);
    }
    
    initializeWheelController(USE_CONTROLLER);
    turnMotorOn(pwm);
    
    
    masterPWM = MotorController_GetLeftPwm();
    slavePWM = MotorController_GetRightPwm();
    
    masterLeftTicks = MotorController_GetLeftQuadDecCount();
    slaveRightTicks = MotorController_GetRightQuadDecCount();
    
    setMotionDirection(motion);
    
}

/* [] END OF FILE */
