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
#include "pi_ctrl.h"
#include <math.h>

// Constants for PI controller
#define PI_CTRL_P_CONST 0.75
#define PI_CTRL_I_CONST 0.1
#define CTRL_MAX 255
#define CTRL_MIN 0

static int piError = 0;
static double piKp = PI_CTRL_P_CONST;
static double piKi = PI_CTRL_I_CONST;
static int piIntegral = 0;
static int piCorrectionSignal = 0;

void initializePIController(double proportionalConstant, double integralConstant) {
    piError = 0;
    piKp = proportionalConstant;
    piKi = integralConstant;
    piIntegral = 0;
    piCorrectionSignal = 0;
}

void resetPIController() {
    piError = 0;
    piKp = PI_CTRL_P_CONST;
    piKi = PI_CTRL_I_CONST;
    piIntegral = 0;
    piCorrectionSignal = 0;
}

void computePIControllerError(int masterTick, int slaveTick) {
    piError = masterTick - slaveTick;
}

void computePIControllerCorrectionSignal() {
    piIntegral += piError;

    // Calculate the PI correction signal
    piCorrectionSignal = (int)(piKp * (double)piError + piKi * (double)piIntegral);

    // Ensure the correction signal is within the allowed range
    if (piCorrectionSignal > CTRL_MAX) {
        piCorrectionSignal = CTRL_MAX;
    } else if (piCorrectionSignal < CTRL_MIN) {
        piCorrectionSignal = CTRL_MIN;
    }
}

uint8 computePIControllerOutput(uint8 masterPwm) {
    int slavePwm = masterPwm + piCorrectionSignal;

    // Ensure the output is within the allowed range
    if (slavePwm > CTRL_MAX) {
        slavePwm = CTRL_MAX;
    } else if (slavePwm < CTRL_MIN) {
        slavePwm = CTRL_MIN;
    }

    return (uint8)slavePwm;
}

uint8 computePIController(int masterTick, int slaveTick, uint8 masterPwm) {
    computePIControllerError(masterTick, slaveTick);
    computePIControllerCorrectionSignal();
    return computePIControllerOutput(masterPwm);
}

/* [] END OF FILE */
