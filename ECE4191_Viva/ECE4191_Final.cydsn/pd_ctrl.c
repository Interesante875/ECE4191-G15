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
#include "pd_ctrl.h"
#include <math.h>

// Constants for PD controller
#define PD_CTRL_P_CONST 0.75
#define PD_CTRL_D_CONST 0.05
#define CTRL_MAX 255
#define CTRL_MIN 0

static int pdError = 0;
static double pdKp = PD_CTRL_P_CONST;
static double pdKd = PD_CTRL_D_CONST;
static int pdLastError = 0;
static int pdCorrectionSignal = 0;

void initializePDController(double proportionalConstant, double derivativeConstant) {
    pdError = 0;
    pdKp = proportionalConstant;
    pdKd = derivativeConstant;
    pdLastError = 0;
    pdCorrectionSignal = 0;
}

void resetPDController() {
    pdError = 0;
    pdKp = PD_CTRL_P_CONST;
    pdKd = PD_CTRL_D_CONST;
    pdLastError = 0;
    pdCorrectionSignal = 0;
}

void computePDControllerError(int masterTick, int slaveTick) {
    pdError = masterTick - slaveTick;
}

void computePDControllerCorrectionSignal() {
    int pdDerivative = pdError - pdLastError;

    // Calculate the PD correction signal
    pdCorrectionSignal = (int)(pdKp * (double)pdError + pdKd * (double)pdDerivative);

    // Update the last error
    pdLastError = pdError;

    // Ensure the correction signal is within the allowed range
    if (pdCorrectionSignal > CTRL_MAX) {
        pdCorrectionSignal = CTRL_MAX;
    } else if (pdCorrectionSignal < CTRL_MIN) {
        pdCorrectionSignal = CTRL_MIN;
    }
}

uint8 computePDControllerOutput(uint8 masterPwm) {
    int slavePwm = masterPwm + pdCorrectionSignal;

    // Ensure the output is within the allowed range
    if (slavePwm > CTRL_MAX) {
        slavePwm = CTRL_MAX;
    } else if (slavePwm < CTRL_MIN) {
        slavePwm = CTRL_MIN;
    }

    return (uint8)slavePwm;
}

uint8 computePDController(int masterTick, int slaveTick, uint8 masterPwm) {
    computePDControllerError(masterTick, slaveTick);
    computePDControllerCorrectionSignal();
    return computePDControllerOutput(masterPwm);
}

/* [] END OF FILE */
