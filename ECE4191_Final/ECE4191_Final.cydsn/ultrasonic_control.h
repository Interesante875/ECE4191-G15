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
    extern int zoneCount[6];
    
    extern TargetPuckColor targetPuckColor;
    extern double distanceToPin;
    extern double initialPinDistance[8];
    
    void startingBowlingPinWhichStripe();

    
#endif
/* [] END OF FILE */
