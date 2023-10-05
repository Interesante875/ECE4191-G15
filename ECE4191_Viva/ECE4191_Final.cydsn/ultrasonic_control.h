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
#ifndef ULTRASONIC_CONTROL_H
    #define ULTRASONIC_CONTROL_H
    
    typedef enum {
        RedPuck,
        GreenPuck,
        BluePuck
    } TargetPuckColor;
    
    extern int pinInWhichZone;
    extern TargetPuckColor targetPuckColor;
    extern double distanceToPin;
    void bowlingPinWhichStripe();

    
#endif
/* [] END OF FILE */
