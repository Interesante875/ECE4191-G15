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
    
    #include "project.h"
    #include "cytypes.h"
    #include "motor.h"
    
    typedef enum {
        ENABLE,
        DISABLE
    } MOVE_STATUS;
    
    typedef enum {
        FORWARD,
        BACKWARD,
        LEFT,
        RIGHT,
        STOP
    } MOTION;
    
    extern MOVE_STATUS moveStatus;
    extern MOTION currentMotion;
   
    
    void motor_on(uint8);
    void motor_off();
    void motor_start();
    void motor_stop();
    void motor_motion_set(MOTION motion);
    
#endif
/* [] END OF FILE */
