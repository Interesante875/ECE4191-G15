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
#ifndef PID_CTRL_H
#define PID_CTRL_H

#include <stdint.h>
#include "cytypes.h"

// Function prototypes
void initializePIDController(double proportionalConstant, double integralConstant, double derivativeConstant);
void resetPIDController();
void computePIDControllerError(int masterTick, int slaveTick);
void computePIDControllerCorrectionSignal();
uint8 computePIDControllerOutput(uint8 masterPwm);
uint8 computePIDController(int masterTick, int slaveTick, uint8 masterPwm);

#endif // PID_CTRL_H
/* [] END OF FILE */
