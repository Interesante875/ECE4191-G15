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
#ifndef TRIALRUN_H
    #define TRIALRUN_H
    
    #include "cytypes.h"
    
    void initializeRobot();
    void moveOutofBaseHighLevel(uint8 pwm, double dist_in_metre);
    void detectWhereIsThePinZone(uint8 detectPWM);
    void test_run();
    void test_sensors();
    void run();
    void runLevelOne();
    void runLevelTwo();
    void runLevelThree();
    void runLevelFour();
    
    void rasterScan();
    void detectOpponent();
    
    
    
#endif
/* [] END OF FILE */
