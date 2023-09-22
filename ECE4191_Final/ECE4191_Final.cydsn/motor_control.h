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
#ifndef MOTOR_CONTROL_H
#define MOTOR_CONTROL_H
    
    #include "cytypes.h"
    
    typedef enum {
        Enable,
        Disable
    } MoveStatus;

    typedef enum {
        Forward,
        Backward,
        Left,
        Right,
        StopMotion
    } MotionDirection;

    typedef enum {
        ProportionalControl,
        ProportionalIntegralControl,
        ProportionalDerivativeControl,
        ProportionalIntegralDerivativeControl
    } ControllerType;

    extern MoveStatus moveStatus;
    extern MotionDirection currentMotionDirection;
    extern ControllerType ctrlType;

    void turnMotorOn(uint8 pwm);
    void turnMotorOff();
    void startMotor();
    void stopMotor();
    void setMotionDirection(MotionDirection motionDirection);

    void initializeWheelController(ControllerType ctrlType_name);
    void stopWheelController();

    CY_ISR(ISR_Handler_Wheel_Controller);
    
#endif
/* [] END OF FILE */
