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
#include "p_ctrl.h"
#include "bluetooth.h"

#define P_CTRL_P_CONST 0.95
#define CTRL_MAX 255
#define CTRL_MIN 0

static int pError = 0;
static double pKp = P_CTRL_P_CONST;
static int pCorrectionSignal = 0;

void initializePController(double proportionalConstant) {
    pError = 0;
    pKp = proportionalConstant;
    pCorrectionSignal = 0;
}

void resetPController() {
    pError = 0;
    pKp = P_CTRL_P_CONST;
    pCorrectionSignal = 0;
}

void computePControllerError(int masterTick, int slaveTick) {
    if (masterTick > 0 && slaveTick > 0) {
        pError = masterTick - slaveTick;
    } else if (masterTick < 0 && slaveTick < 0) {
        pError = -(masterTick - slaveTick);
    } else if (masterTick > 0 && slaveTick < 0) {
        pError = abs(masterTick) - abs(slaveTick);
    } else if (masterTick < 0 && slaveTick > 0) {
        pError = abs(masterTick) - abs(slaveTick);
    }
}

void computePControllerCorrectionSignal() {
    pCorrectionSignal = (int)(pKp * (double)pError);
    
    //printValue("Error: %d\n", pCorrectionSignal);
}

uint8 computePControllerOutput(uint8 masterPwm) {
    int slavePwm = masterPwm + pCorrectionSignal;
    return (uint8)(slavePwm > CTRL_MAX ? CTRL_MAX : (slavePwm < (masterPwm - 20) ? (masterPwm - 20) : slavePwm));
}

uint8 computePController(int masterTick, int slaveTick, uint8 masterPwm) {
    computePControllerError(masterTick, slaveTick);
    computePControllerCorrectionSignal();
    return computePControllerOutput(masterPwm);
}
/* [] END OF FILE */
