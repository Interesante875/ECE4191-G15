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
#ifndef PI_CTRL_H
#define PI_CTRL_H

#include "cytypes.h"



// Function prototypes
void initializePIController(double proportionalConstant, double integralConstant);
void resetPIController();
void computePIControllerError(int masterTick, int slaveTick);
void computePIControllerCorrectionSignal();
uint8 computePIControllerOutput(uint8 masterPwm);
uint8 computePIController(int masterTick, int slaveTick, uint8 masterPwm);

#endif // PI_CTRL_H

/* [] END OF FILE */
