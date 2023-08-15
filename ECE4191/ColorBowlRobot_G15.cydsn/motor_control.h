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

#ifndef MOTOR_CONTROL_H
#define MOTOR_CONTROL_H

#define ENABLE 1
#define DISABLE 0
#define FORWARD 0
#define BACKWARD 1
#define LEFT 2
#define RIGHT 3
#define WHEEL_FULL_SPEED 255
#define WHEEL_HALF_SPEED 160
#define WHEEL_SLOW 127

void motor_move(int mode, int direction, int speed);

#endif
