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
#ifndef LOCOMOTION_H
    #define LOCOMOTION_H
    
    #define USE_CONTROLLER ProportionalControl
    
    #include "motor_control.h"
    
    typedef enum {
        ProportionalControl,
        ProportionalIntegralControl,
        ProportionalDerivativeControl,
        ProportionalIntegralDerivativeControl
    } ControllerType;
    
    extern ControllerType ctrlType;
    
    void wheel_move_by_ticks(MotionDirection motion, int pwm, int target_ticks);
    void wheel_move_by_metrics (MotionDirection motion, uint8 pwm, double metrics);
    void wheel_move (MotionDirection motion, uint8 pwm);
    
    void initializeWheelController(ControllerType ctrlType_name, int pwm);
    void stopWheelController();

    CY_ISR(ISR_Handler_Wheel_Controller);
    
    
#endif

/* [] END OF FILE */
