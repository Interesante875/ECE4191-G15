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
#include "navigation.h"
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <stdbool.h>

#define EMF_BUFFER_DELAY 50
#define TICKS_PER_REVOLUTION 4300
#define WHEEL_RADIUS 0.02695
#define WHEEL_DISTANCE 0.1475
#define HALF_WIDTH 0.122

static volatile int masterPWM = 0;
static volatile int slavePWM = 0;
static volatile int targetMasterPWM = 0;
static volatile int lastSlavePWM = 0; // not implemented
static volatile int masterLeftTicks = 0;
static volatile int slaveRightTicks = 0;
static volatile int errorTicks = 0;
const int errorPWMThreshold = 500;
int target_move_ticks = 0;
int stop_flag = 0;
static volatile int lastMasterTicks = 0;
static volatile int lastSlaveTicks = 0;
uint8 updated_slave_pwm = 0;
int period = 0;

ControllerType ctrlType;

void initializeWheelController(ControllerType ctrlType_name, int pwm) {
    ctrlType = ctrlType_name;
    switch (ctrlType) {
        case ProportionalControl:
        
            if (pwm > 240) initializePController(0.9);
            else if (pwm > 220) initializePController(0.7);
            else if (pwm > 200) initializePController(0.5);
            else initializePController(0.5);
            // printValue("Proportional Controller is used\n");
        break;
        
        case ProportionalIntegralDerivativeControl:
            printValue("PID Controller is used\n");
            resetPIDController();
        break; 
        default:
            // printValue("Proportional Controller is used\n");
            resetPController();
        break;
    }
    
    updated_slave_pwm = 0;
    
    lastMasterTicks = 0;
    lastSlaveTicks = 0;
    
    Timer_Wheel_Start();
    isr_wheel_controller_StartEx(ISR_Handler_Wheel_Controller);
}

void stopWheelController() {
    
    lastMasterTicks = 0;
    lastSlaveTicks = 0;
    
    Timer_Wheel_Stop();
    isr_wheel_controller_Stop();
}


CY_ISR(ISR_Handler_Wheel_Controller) {
    
    Timer_Wheel_ReadStatusRegister();
    
    masterPWM = MotorController_GetLeftPwm();

    updated_slave_pwm = 0;
    
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
    
    period = (period + 1) % 10;
    if (period == 9)
    {
        computePosition(masterLeftTicks - lastMasterTicks, slaveRightTicks - lastSlaveTicks);
        lastMasterTicks = masterLeftTicks;
        lastSlaveTicks = slaveRightTicks;
    }
    

}

void wheel_move_by_ticks(MotionDirection motion, int pwm, int target_ticks) {

    turnMotorOn(pwm);
    
    masterPWM = MotorController_GetLeftPwm();
    slavePWM = MotorController_GetRightPwm();

    masterLeftTicks = MotorController_GetLeftQuadDecCount();
    slaveRightTicks = MotorController_GetRightQuadDecCount();

    target_move_ticks = target_ticks;
    targetMasterPWM = masterPWM;
    
    setMotionDirection(motion);

    initializeWheelController(ProportionalControl, pwm);
    
//    printValue("Set Controller\n");
    while (abs(masterLeftTicks) < target_ticks){
        //computePosition(masterLeftTicks - lastMasterTicks, slaveRightTicks - lastSlaveTicks); 
    }
    
//    printValue("DONE\n");
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
    
    
    turnMotorOn(pwm);

    masterPWM = MotorController_GetLeftPwm();
    slavePWM = MotorController_GetRightPwm();
    
    masterLeftTicks = MotorController_GetLeftQuadDecCount();
    slaveRightTicks = MotorController_GetRightQuadDecCount();
    
    setMotionDirection(motion);
    initializeWheelController(USE_CONTROLLER, pwm);
    
    while (abs(masterLeftTicks) < ticks) {
    }

//    printValue("LEFT: %d RIGHT: %d\n ", masterLeftTicks, slaveRightTicks);
//    printValue("Master PWM: %d Slave PWM: %d\n", masterPWM, slavePWM);
    
    stopWheelController();
    stopMotor();
    printToBluetooth();

    CyDelay(EMF_BUFFER_DELAY);
}

void wheel_move (MotionDirection motion, uint8 pwm) {

    if (motion == Left || motion == Right) return;
    
    if (motion == StopMotion) {
        
        lastMasterTicks = 0;
        lastSlaveTicks = 0;

        stopWheelController();
        stopMotor();

        CyDelay(EMF_BUFFER_DELAY);
    }

    turnMotorOn(pwm);
 
    masterPWM = MotorController_GetLeftPwm();
    slavePWM = MotorController_GetRightPwm();
    
    masterLeftTicks = MotorController_GetLeftQuadDecCount();
    slaveRightTicks = MotorController_GetRightQuadDecCount();
    
    setMotionDirection(motion);
    initializeWheelController(USE_CONTROLLER, pwm);
}

/* [] END OF FILE */
