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
    #include "motor.h"
    
    #define P_CONST 0.75
    #define I_CONST 0.0095
    #define D_CONST 0.00075
    
    void initializePController(double prop_constant);
    void resetPController();
    
    uint8 computePController(int master_tick, int slave_tick, uint8 master_pwm); 
    
    void initializePIDController(double prop_constant, double int_constant, double de_constant);
    void resetPIDController();
    
    uint8 computePIDController(int master_tick, int slave_tick, uint8 master_pwm);
    
#endif /* CONTROLLER_H */
/* [] END OF FILE */
