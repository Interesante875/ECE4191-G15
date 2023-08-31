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
#ifndef BLUETOOTH_H
#define BLUETOOTH_H
    
    #include "project.h"
    #include "cytypes.h"
    
    
    
    CY_ISR(ISR_Handler_Input);
    void bluetooth_start();
    void bluetooth_stop();
    void printValue(const char *format, ...);
    void handshake();
    
    
#endif /* BLUETOOTH_H */
/* [] END OF FILE */
