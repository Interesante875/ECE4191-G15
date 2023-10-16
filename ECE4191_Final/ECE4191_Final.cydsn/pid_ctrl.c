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
#include "pid_ctrl.h"

#define PID_CTRL_P_CONST 0.95
#define PID_CTRL_I_CONST 0.1
#define PID_CTRL_D_CONST 0.2
#define CTRL_MAX 255
#define CTRL_MIN 0

static int pidError = 0;
static double pidKp = PID_CTRL_P_CONST;
static double pidKi = PID_CTRL_I_CONST;
static double pidKd = PID_CTRL_D_CONST;
static int pidIntegral = 0;
static int pidLastError = 0;
int correctionSignal = 0;

void initializePIDController(double proportionalConstant, double integralConstant, double derivativeConstant) {
    pidError = 0;
    pidKp = proportionalConstant;
    pidKi = integralConstant;
    pidKd = derivativeConstant;
    pidIntegral = 0;
    pidLastError = 0;
}

void resetPIDController() {
    pidError = 0;
    pidKp = PID_CTRL_P_CONST;
    pidKi = PID_CTRL_I_CONST;
    pidKd = PID_CTRL_D_CONST;
    pidIntegral = 0;
    pidLastError = 0;
}

void computePIDControllerError(int masterTick, int slaveTick) {
    if (masterTick > 0 && slaveTick > 0) {
        pidError = masterTick - slaveTick;
    } else if (masterTick < 0 && slaveTick < 0) {
        pidError = -(masterTick - slaveTick);
    } else if (masterTick > 0 && slaveTick < 0) {
        pidError = abs(masterTick) - abs(slaveTick);
    } else if (masterTick < 0 && slaveTick > 0) {
        pidError = abs(masterTick) - abs(slaveTick);
    }
}

void computePIDControllerCorrectionSignal() {
    int pidProportional = (int)(pidKp * (double)pidError);
    pidIntegral += pidError;
    int pidDerivative = pidError - pidLastError;

    // Calculate the PID correction signal
    int pidCorrectionSignal = (int)(pidProportional + pidKi * (double)pidIntegral + pidKd * (double)pidDerivative);

    // Update the last error
    pidLastError = pidError;

    // Ensure the correction signal is within the allowed range

    // Assign the correction signal
    correctionSignal = pidCorrectionSignal;
}

uint8 computePIDControllerOutput(uint8 masterPwm) {
    int slavePwm = masterPwm + correctionSignal;

    // Ensure the output is within the allowed range
    if (slavePwm > CTRL_MAX) {
        slavePwm = CTRL_MAX;
    } else if (slavePwm < CTRL_MIN + 10) {
        slavePwm = CTRL_MIN + 10;
    }

    return (uint8)slavePwm;
}

uint8 computePIDController(int masterTick, int slaveTick, uint8 masterPwm) {
    computePIDControllerError(masterTick, slaveTick);
    computePIDControllerCorrectionSignal();
    return computePIDControllerOutput(masterPwm);
}

/* [] END OF FILE */
