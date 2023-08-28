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
#ifndef CONTROLLER_H
#define CONTROLLER_H
    
    #include "project.h"
    #include "cytypes.h"
    
    #define PWM_MAX 255
    #define PWM_MIN 0 
    #define P_CONST 0.75
    
    void initializePController(double prop_constant);
    void resetPController();
    
    uint8 computePController(int master_tick, int slave_tick, uint8 master_pwm);    
    
#endif /* CONTROLLER_H */
/* [] END OF FILE */
