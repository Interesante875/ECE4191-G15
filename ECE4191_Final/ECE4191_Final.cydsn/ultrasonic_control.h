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
    
    extern double FLU, FRU, LFU, RFU, BLU, BRU, LBU, RBU;
    extern double distanceMeasured[8];
    extern int pinInWhichZone;
    extern int zoneCount[6];
    
    extern TargetPuckColor targetPuckColor;
    extern double distanceToPin;
    extern double initialPinDistance[8];
    
    void read_U();
    void read_FLU();
    void read_FRU();
    void read_LFU();
    void read_RFU();
    void read_BLU();
    void read_BRU();
    void read_LBU();
    void read_RBU();
    void read_FU();
    void read_LU();
    void read_RU();
    void read_BU();
    void print_U();
    
    void startingBowlingPinWhichStripe();

    
#endif
/* [] END OF FILE */
