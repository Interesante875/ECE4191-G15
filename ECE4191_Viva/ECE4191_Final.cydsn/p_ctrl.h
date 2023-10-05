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
#ifndef P_CTRL_H
#define P_CTRL_H
    
    #include "cytypes.h"
    
    
    
    void initializePController(double proportionalConstant);
    void resetPController();
    void computePControllerError(int masterTick, int slaveTick); 
    void computePControllerCorrectionSignal();
    uint8 computePControllerOutput(uint8 masterPwm);
    uint8 computePController(int masterTick, int slaveTick, uint8 masterPwm);
    
#endif

/* [] END OF FILE */
