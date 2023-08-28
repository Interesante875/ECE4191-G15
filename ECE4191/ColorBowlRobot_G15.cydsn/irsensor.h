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
        PRESENCE,
        ABSENCE
    } DETECTED;
    
    DETECTED ir_LEFT = ABSENCE;
    DETECTED ir_RIGHT = ABSENCE;
    
    void ir_read();
    void ir_read_left();
    void ir_read_right();
    
    
#endif /* IRSENSOR_H */
/* [] END OF FILE */
