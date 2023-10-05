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
#ifndef PD_CTRL_H
#define PD_CTRL_H

#include "cytypes.h"

// Function prototypes
void initializePDController(double proportionalConstant, double derivativeConstant);
void resetPDController();
void computePDControllerError(int masterTick, int slaveTick);
void computePDControllerCorrectionSignal();
uint8 computePDControllerOutput(uint8 masterPwm);
uint8 computePDController(int masterTick, int slaveTick, uint8 masterPwm);

#endif // PD_CTRL_H

/* [] END OF FILE */
