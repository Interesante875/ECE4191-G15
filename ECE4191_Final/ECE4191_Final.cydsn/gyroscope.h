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
#ifndef GYROSCOPE_H
#define GYROSCOPE_H
    
    #include "project.h"
    
    
    
    extern double heading;
    
    void initializeGyroscope();
    void shutdownGyroscope();
    void wakeupGyroscope();
    void setScalingFactorGyroscope();
    void activateFilterGyroscope();
    
    void startGyroscope();
    
    double yawRateToYawAngleDerivative(double yawRate);
    double rungeKuttaIntegration(double initialValue, double yawRate, double timeStep);
    double roundToDecimalPlaces(double value, int decimalPlaces);
    
    CY_ISR(ISR_Handler_Gyroscope);
    
#endif
/* [] END OF FILE */
