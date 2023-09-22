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
#ifndef IRSENSOR_H
#define IRSENSOR_H
    
    #include "project.h"
    #include <stdio.h>

    typedef enum {
        Presence,
        Absence
    } DetectionStatus;

    extern volatile DetectionStatus infraredDetectionStatus;

    void startIR();
    void stopIR();

    CY_ISR(ISR_Handler_IR);
    
#endif /* IRSENSOR_H */
/* [] END OF FILE */
