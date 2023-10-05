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

#ifndef MOTOR_CONTROLLER_H
#define MOTOR_CONTROLLER_H

    
    #define PWM_MAX 255
    #define PWM_MIN 0

    void MotorController_StartLeftQuadDec();
    void MotorController_StopLeftQuadDec();
    void MotorController_ResetLeftQuadDec();
    void MotorController_SetLeftQuadDecCount(int count);
    int MotorController_GetLeftQuadDecCount();

    void MotorController_StartRightQuadDec();
    void MotorController_StopRightQuadDec();
    void MotorController_ResetRightQuadDec();
    void MotorController_SetRightQuadDecCount(int count);
    int MotorController_GetRightQuadDecCount();

    void MotorController_StartPwm();
    void MotorController_StopPwm();

    void MotorController_SetLeftPwmCompare(int compareValue);
    int MotorController_GetLeftPwm();
    void MotorController_SetRightPwmCompare(int compareValue);
    int MotorController_GetRightPwm();


#endif /* MOTOR_CONTROLLER_H */

/* [] END OF FILE */
