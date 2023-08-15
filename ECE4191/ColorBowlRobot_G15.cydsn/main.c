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
#include "project.h"
#include "motor_control.h"
#include <stdio.h>
#include <math.h>

int main(void)
{
    CyGlobalIntEnable; /* Enable global interrupts. */

    /* Place your initialization/startup code here (e.g. MyInst_Start()) */
    UART_1_Start();
    QuadDec_1_Start();
    QuadDec_2_Start();
    
    motor_move(ENABLE, RIGHT, WHEEL_FULL_SPEED);
    int counter_1 = 0;
    int counter_2 = 0;
    char string_1[20];
    char string_2[20];

    for(;;)
    {
        /* Place your application code here. */
        counter_1 = QuadDec_1_GetCounter();
        counter_2 = QuadDec_2_GetCounter();
        
        sprintf(string_1, "m1 %d\n", counter_1);
        sprintf(string_2, "m2 %d\n", counter_2);
        
        UART_1_PutString(string_1);
        UART_1_PutString(string_2);
        
        CyDelay(100);
    }
}

/* [] END OF FILE */
