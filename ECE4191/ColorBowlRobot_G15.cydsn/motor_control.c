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

/* [] END OF FILE */
#include "motor_control.h"
#include "project.h" // Include the necessary project-specific header file

void motor_move(int mode, int direction, int speed) {
    if (mode == ENABLE) {
        PWM_1_Start();
        PWM_1_WriteCompare(speed);
    } else if (mode == DISABLE) {
        PWM_1_Stop();
    }

    if (direction == FORWARD || direction == BACKWARD) {
        MOTOR_1_IN_LEFT_Write(direction);
        MOTOR_1_IN_RIGHT_Write(!direction);

        MOTOR_2_IN_LEFT_Write(MOTOR_1_IN_LEFT_Read());
        MOTOR_2_IN_RIGHT_Write(MOTOR_1_IN_RIGHT_Read());
    } else {
        direction -= 2;
        MOTOR_1_IN_LEFT_Write(!direction);
        MOTOR_1_IN_RIGHT_Write(direction);

        MOTOR_2_IN_LEFT_Write(!MOTOR_1_IN_LEFT_Read());
        MOTOR_2_IN_RIGHT_Write(!MOTOR_1_IN_RIGHT_Read());
    }
}
