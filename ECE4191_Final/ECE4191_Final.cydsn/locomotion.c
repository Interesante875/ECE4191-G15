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
#include "gyroscope.h"
#include "ultrasonic.h"
#include "ultrasonic_control.h"
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <stdbool.h>

#define EMF_BUFFER_DELAY 50
#define TICKS_PER_REVOLUTION 4300
#define WHEEL_RADIUS 0.02745
#define WHEEL_DISTANCE 0.1536
#define HALF_WIDTH 0.122

// 12.96 + 2.4/2 = 
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
MotionDirection currentMotion = StopMotion;
int debug_err = 0;
int debug_eff = 0;

ControllerType ctrlType;

void initializeWheelController(ControllerType ctrlType_name, int pwm) {
    ctrlType = ctrlType_name;
    switch (ctrlType) {
        case ProportionalControl:
            resetPController();
//            if (pwm > 240) initializePController(0.9);
//            else if (pwm > 220) initializePController(0.7);
//            else if (pwm >= 200) initializePController(0.65);
//            else initializePController(0.05);
            // printValue("Proportional Controller is used\n");
        break;
        
        case ProportionalIntegralDerivativeControl:
            //printValue("PID Controller is used\n");
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
    
    //if (currentMotion == Forward)
    MotorController_SetRightPwmCompare((uint8) updated_slave_pwm);
    MotorController_SetLeftPwmCompare((uint8) masterPWM);
//    else if (currentMotion == Backward)
//    MotorController_SetLeftPwmCompare((uint8) updated_slave_pwm);
//    else if (currentMotion == Left)
//    MotorController_SetLeftPwmCompare((uint8) updated_slave_pwm);
//    else if (currentMotion == Right)
//    MotorController_SetRightPwmCompare((uint8) updated_slave_pwm);
    
    masterLeftTicks = MotorController_GetLeftQuadDecCount();
    slaveRightTicks = MotorController_GetRightQuadDecCount();
    
    period = (period + 1) % 10;
    if (period == 9)
    {
        computePosition(masterLeftTicks - lastMasterTicks, slaveRightTicks - lastSlaveTicks);
        lastMasterTicks = masterLeftTicks;
        lastSlaveTicks = slaveRightTicks;
        // printPositionToBluetooth();
//        printValue("L:%d R: %d, LPWM:%d RPWM: %d\n", masterLeftTicks, slaveRightTicks, masterPWM, updated_slave_pwm);
//        if (masterLeftTicks > 0 && slaveRightTicks > 0) {
//            debug_err = masterLeftTicks - slaveRightTicks;
//        } else if (masterLeftTicks < 0 && slaveRightTicks < 0) {
//            debug_err = -(masterLeftTicks - slaveRightTicks);
//        } else if (masterLeftTicks > 0 && slaveRightTicks < 0) {
//            debug_err = abs(masterLeftTicks) - abs(slaveRightTicks);
//        } else if (masterLeftTicks < 0 && slaveRightTicks > 0) {
//            debug_err = abs(masterLeftTicks) - abs(slaveRightTicks);
//        }
//        debug_eff = (int)(0.75 * (double)debug_err);
//        int SLV = masterPWM + debug_eff;
//        SLV = (uint8)(SLV > 255 ? 255 : (SLV < (masterPWM - 20) ? (masterPWM - 20) : SLV));
//        printValue("Err: %d ctrl: %d SLV:%d \n", debug_err, debug_eff, SLV);
    }
    

}

void wheel_move_by_ticks(MotionDirection motion, int pwm, int target_ticks) {


    currentMotion = motion;
    turnMotorOn(pwm);
    
    masterPWM = MotorController_GetLeftPwm();
    slavePWM = MotorController_GetRightPwm();

    masterLeftTicks = MotorController_GetLeftQuadDecCount();
    slaveRightTicks = MotorController_GetRightQuadDecCount();
    
    target_move_ticks = target_ticks;
    targetMasterPWM = masterPWM;
    
    setMotionDirection(motion);
    initializeWheelController(USE_CONTROLLER, pwm);
    
    
    // printValue("Set Controller\n");
    while (abs(masterLeftTicks) < target_ticks){
        //computePosition(masterLeftTicks - lastMasterTicks, slaveRightTicks - lastSlaveTicks); 
    }
    
//    printValue("DONE\n");
//    printValue("LEFT: %d RIGHT: %d\n ", masterLeftTicks, slaveRightTicks);
//    printValue("Master PWM: %d Slave PWM: %d\n", masterPWM, slavePWM);
    currentMotion = StopMotion;
    stopMotor();
    stopWheelController();
    
    
    CyDelay(EMF_BUFFER_DELAY);
}

void wheel_move_by_metrics_with_gyro (MotionDirection motion, uint8 pwm, double metrics) {
    
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
    
    double initHeadingAngleSE = heading_angle;
    double initHeadingAngleGyro = gyroHeading;
    
    #if USE_GYRO_CONTROL
        while (abs(masterLeftTicks) < ticks); 
        // while (fabs(heading) < metrics);
        CyDelay(200);
        printValue("H: %.2lf\n", gyroHeading);
        
        if (motion == Left || motion == Right) {
            if (fabs(gyroHeading) >= metrics + 5) {
                stopWheelController();
                initializeWheelController(USE_CONTROLLER, pwm);
                double ang_correct = fabs(gyroHeading) - metrics;
                printValue("CH: %.2lf\n", ang_correct);
                if (motion == Left) {
                    wheel_move_by_metrics(Right, 220, ang_correct); 
                } else if (motion == Right) {
                    wheel_move_by_metrics(Left, 220, ang_correct);
                }
                
            } else if (fabs(gyroHeading) <= metrics - 5) {
                stopWheelController();
               initializeWheelController(USE_CONTROLLER, pwm);
                double ang_correct = metrics - fabs(gyroHeading);
                printValue("CH: %.2lf\n", ang_correct);
                if (motion == Left) {
                    wheel_move_by_metrics(Left, 220, ang_correct); 
                } else if (motion == Right) {
                    wheel_move_by_metrics(Right, 220, ang_correct);
                }
            }
            
        }
        
        
    #else
        while (abs(masterLeftTicks) < ticks); 
    #endif
    
//    printValue("LEFT: %d RIGHT: %d\n ", masterLeftTicks, slaveRightTicks);
//    printValue("Master PWM: %d Slave PWM: %d\n", masterPWM, slavePWM);
    stopWheelController();
    stopMotor();
    // printToBluetooth();

    CyDelay(EMF_BUFFER_DELAY);
}

int computeTicks(int L, int R) {
    if (L > 0 && R > 0) {
        return (L+R)/2;
    } else if (L < 0 && R < 0) {
        return (L+R)/2;
    } else if (L > 0 && R < 0) {
        return (L-R)/2;
    } else if (L < 0 && R > 0) {
        return (R-L)/2;
    } else {
        return 0;   
    }
    
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
    
    currentMotion = motion;
    turnMotorOn(pwm);

    masterPWM = MotorController_GetLeftPwm();
    slavePWM = MotorController_GetRightPwm();
    
    masterLeftTicks = MotorController_GetLeftQuadDecCount();
    slaveRightTicks = MotorController_GetRightQuadDecCount();
    
    setMotionDirection(motion);
    initializeWheelController(USE_CONTROLLER, pwm);
    // printValue("Initial Heading: %lf\n",gyroHeading);
    while (abs(masterLeftTicks) < ticks); 

//    printValue("LEFT: %d RIGHT: %d\n ", masterLeftTicks, slaveRightTicks);
//    printValue("Master PWM: %d Slave PWM: %d\n", masterPWM, slavePWM);
    
    stopWheelController();
    stopMotor();
    currentMotion = StopMotion;
    // printToBluetooth();
    // printValue("After Heading: %lf\n",gyroHeading);
    CyDelay(EMF_BUFFER_DELAY);
}

void wheel_move (MotionDirection motion, uint8 pwm) {

    if (motion == Left || motion == Right) return;
    
    if (motion == StopMotion) {
        
        lastMasterTicks = 0;
        lastSlaveTicks = 0;

        stopWheelController();
        stopMotor();
        currentMotion = StopMotion;
        CyDelay(EMF_BUFFER_DELAY);
    }

    turnMotorOn(pwm);
 
    masterPWM = MotorController_GetLeftPwm();
    slavePWM = MotorController_GetRightPwm();
    
    masterLeftTicks = MotorController_GetLeftQuadDecCount();
    slaveRightTicks = MotorController_GetRightQuadDecCount();
    
    setMotionDirection(motion);
    currentMotion = motion;
    initializeWheelController(USE_CONTROLLER, pwm);
}

void angle_correction_with_sides(uint8 pwm, int dir) {
    
    // 0 - Front 1 - Back 2 - RIght 3 - Left
    
    CyDelay(360);
    read_U();
    
    double arr[] = {FLU, FRU, BLU, BRU, RFU, RBU, LFU, LBU};
    
    int minPairIndex = dir*2;
    
    if (arr[minPairIndex] >= 40 || arr[minPairIndex+1] >= 40) return;
    
    double delta_dist = (arr[minPairIndex] - arr[minPairIndex + 1])/100;
    printValue("DELTA: %.2lf %.2lf %.2lf\n", delta_dist, arr[minPairIndex], arr[minPairIndex + 1]);
    double angle = 0;
    double threshold = 0.02;
    
    double L = 0;
    
    if (dir == 0 || dir == 1) {
        L = 0.10;   
    } else if (dir == 2) {
        L = 0.160;   
    } else if (dir == 3) {
        L = 0.14;   
    }
    
    angle = atan2(delta_dist, L) * 180 / CY_M_PI;
    
    if (dir <= 1) {
        angle *= 2;
    }
    
    printValue("ANGLE: %.2lf\n", angle);
    if (dir != 1) {
        if (angle > 30) angle = 30;
        else if (angle < -30) angle = -30;
    }
    else {
        if (angle > 50) angle = 50;
        else if (angle < -50) angle = -50;
        
    }
    printValue("CANGLE: %.2lf\n", angle);
    switch (minPairIndex) {
        case 0:
            printValue("FRONT\n");
            if (angle > 0) {
                wheel_move_by_metrics(Right, pwm, fabs(angle));
            }
            else {
                wheel_move_by_metrics(Left, pwm, fabs(angle));
            }
            break;
        case 2:
            printValue("BACK\n");
            if (angle > 0) {
                wheel_move_by_metrics(Left, pwm, fabs(angle));
            }
            else {
                wheel_move_by_metrics(Right, pwm, fabs(angle));
            }
            break;
        case 4:
            printValue("RIGHT\n");
            if (angle > 0) {
                wheel_move_by_metrics(Right, pwm, fabs(angle));
            }
            else {
                wheel_move_by_metrics(Left, pwm, fabs(angle));
            }
            break;
        case 6:
            printValue("LEFT\n");
            if (angle > 0) {
                wheel_move_by_metrics(Left, pwm, fabs(angle));
            }
            else {
                wheel_move_by_metrics(Right, pwm, fabs(angle));
            }
            break;
  
    }
    
    
}

double angle_correction_with_sides_return(uint8 pwm, int dir) {
    
    // 0 - Front 1 - Back 2 - RIght 3 - Left
    
    CyDelay(100);
    read_U();
    
    double arr[] = {FLU, FRU, BLU, BRU, RFU, RBU, LFU, LBU};
    
    int minPairIndex = dir*2;
    
    if (arr[minPairIndex] >= 40 || arr[minPairIndex+1] >= 40) return 0;
    
    double delta_dist = (arr[minPairIndex] - arr[minPairIndex + 1])/100;
    printValue("DELTA: %.2lf %.2lf %.2lf\n", delta_dist, arr[minPairIndex], arr[minPairIndex + 1]);
    double angle = 0;
    double threshold = 0.02;
    
    double L = 0;
    
    if (dir == 0 || dir == 1) {
        L = 0.10;   
    } else if (dir == 2) {
        L = 0.160;   
    } else if (dir == 3) {
        L = 0.14;   
    }
    
    angle = atan2(delta_dist, L) * 180 / CY_M_PI;
    
    if (dir <= 1) {
        angle *= 2;
    }
    
    printValue("ANGLE: %.2lf\n", angle);
    if (dir != 1) {
        if (angle > 30) angle = 30;
        else if (angle < -30) angle = -30;
    }
    else {
        if (angle > 50) angle = 50;
        else if (angle < -50) angle = -50;
        
    }
    printValue("CANGLE: %.2lf\n", angle);
    switch (minPairIndex) {
        case 0:
            printValue("FRONT\n");
            if (angle > 0) {
                wheel_move_by_metrics(Right, pwm, fabs(angle));
            }
            else {
                wheel_move_by_metrics(Left, pwm, fabs(angle));
            }
            break;
        case 2:
            printValue("BACK\n");
            if (angle > 0) {
                wheel_move_by_metrics(Left, pwm, fabs(angle));
            }
            else {
                wheel_move_by_metrics(Right, pwm, fabs(angle));
            }
            break;
        case 4:
            printValue("RIGHT\n");
            if (angle > 0) {
                wheel_move_by_metrics(Right, pwm, fabs(angle));
            }
            else {
                wheel_move_by_metrics(Left, pwm, fabs(angle));
            }
            break;
        case 6:
            printValue("LEFT\n");
            if (angle > 0) {
                wheel_move_by_metrics(Left, pwm, fabs(angle));
            }
            else {
                wheel_move_by_metrics(Right, pwm, fabs(angle));
            }
            break;
  
    }
    
    return angle;
}

void angle_correction(uint8 pwm) {
    
    CyDelay(360);
    
    read_U();
    
    double arr[] = {FLU, FRU, BLU, BRU, RFU, RBU, LFU, LBU};
    
    int minPairIndex = 0;

    for (int i = 2; i < 8; i += 2) {
        if (arr[i] + arr[i + 1] < arr[minPairIndex] + arr[minPairIndex + 1]) {
            minPairIndex = i;
        }
    }

    int pair1 = arr[minPairIndex];
    int pair2 = arr[minPairIndex + 1];

    printValue("The lowest value pair is (%d, %d)\n", pair1, pair2);
    
    if (pair1 >= 38 || pair2 >= 38) {
        return;   
        
    }
    
    lastMasterTicks = 0;
    lastSlaveTicks = 0;
    
    double delta_dist = (arr[minPairIndex] - arr[minPairIndex + 1])/100;
    printValue("DELTA: %.2lf\n", delta_dist);
    double angle = 0;
    double threshold = 0.02;
    
    angle = atan2(delta_dist, 0.1005) * 180 / CY_M_PI;
    
    printValue("ANGLE: %.2lf\n", angle);
    
    if (angle > 30) angle = 30;
    else if (angle < -30) angle = -30;
    
    switch (minPairIndex) {
        case 0:
            printValue("FRONT\n");
            if (angle > 0) {
                wheel_move_by_metrics(Right, pwm, fabs(angle));
            }
            else {
                wheel_move_by_metrics(Left, pwm, fabs(angle));
            }
            break;
        case 2:
            printValue("BACK\n");
            if (angle > 0) {
                wheel_move_by_metrics(Left, pwm, fabs(angle));
            }
            else {
                wheel_move_by_metrics(Right, pwm, fabs(angle));
            }
            break;
        case 4:
            printValue("RIGHT\n");
            if (angle > 0) {
                wheel_move_by_metrics(Right, pwm, fabs(angle));
            }
            else {
                wheel_move_by_metrics(Left, pwm, fabs(angle));
            }
            break;
        case 6:
            printValue("LEFT\n");
            if (angle > 0) {
                wheel_move_by_metrics(Left, pwm, fabs(angle));
            }
            else {
                wheel_move_by_metrics(Right, pwm, fabs(angle));
            }
            break;
  
    }

}

double inverseVarianceWeighting(double ticks_h, double gyro_h) {
    double ang = 0.5 * ticks_h + 0.5 * gyro_h; 
    printValue("%lf\n", ang);
    return ang;
}
/* [] END OF FILE */
