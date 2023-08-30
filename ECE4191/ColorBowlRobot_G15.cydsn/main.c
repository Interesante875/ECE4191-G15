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
#include <stdio.h>
#include <math.h>
#include "ultrasonic.h"
#include "bluetooth.h"

int main(void)
{
    CyGlobalIntEnable; /* Enable global interrupts. */
    bluetooth_start();
    handshake();
    ultrasonic_measuring();
    /* Place your initialization/startup code here (e.g. MyInst_Start()) */
    char string_print[64];
    for(;;)
    {
        ultrasonic_measuring();
        for (int i = 0; i < NUMBER_OF_UDS; i++) {
            sprintf(string_print, "Distance: %d\t", (int) kaldist_measure[i]);
            UART_1_PutString(string_print);
        }
        UART_1_PutString("\n");
        
        CyDelay(500);
        

    }
}

/* [] END OF FILE */
