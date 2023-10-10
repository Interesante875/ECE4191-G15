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
#include <stdlib.h>
#include <math.h>
#include "bluetooth.h"
#include "gyroscope.h"

#define GYRO_THRESHOLD 0.5
#define MPU6050 0x68

uint8 array_1[6] = {0};
int16 temp = 0;
double yaw_rate = 0;
double yaw_angle = 0;
double previous_reading = 0;
double current_reading = 0;
double gyroHeading = 0;
int cycles = 0;
const double timeStep = 0.1;
int cal = 1;
int count = 0;
int16 error = 0; //to find the error in gyroscope readings when stationary
int16 error_avg = 0; //average error in gyroscope readings when stationary
double threshold = 0.1; //to prevent accumulation of error due to integration

CY_ISR(ISR_Handler_Gyroscope)
{
    Timer_Gyroscope_ReadStatusRegister();
    
    //Writing the register number from where the read operation starts
    I2C_1_MasterSendStart(MPU6050,0); //Steps 1-2
    I2C_1_MasterWriteByte(0x43); //Step 3
    I2C_1_MasterSendStop(); //Step 4
    CyDelay(1);
    
    //Read data starting from register 0x43
    I2C_1_MasterSendStart(MPU6050,1); //Steps 5-6
    
    //Step 7
    array_1[0] = I2C_1_MasterReadByte(I2C_1_ACK_DATA); //GYRO_XOUT_H
    array_1[1] = I2C_1_MasterReadByte(I2C_1_ACK_DATA); //GYRO_XOUT_L
    array_1[2] = I2C_1_MasterReadByte(I2C_1_ACK_DATA); //GYRO_YOUT_H
    array_1[3] = I2C_1_MasterReadByte(I2C_1_ACK_DATA); //GYRO_YOUT_L
    array_1[4] = I2C_1_MasterReadByte(I2C_1_ACK_DATA); //GYRO_ZOUT_H
    array_1[5] = I2C_1_MasterReadByte(I2C_1_NAK_DATA); //GYRO_ZOUT_L
    
    I2C_1_MasterSendStop(); //Step 8
    CyDelay(1);
    
     temp = array_1[4] << 8 | array_1[5];
        
    if (cal == 1 && count < 20) //while calibrating
    {
        printValue("Calibrating\n");
        error += temp;
        count++;
    }
    else if (cal == 1 && count == 20) //end of calibration and computation of average error
    {
        error_avg = error/20;
        cal = 0;
    }
    else //after calibration and with average error computed
    {
        temp = temp - error_avg;
        yaw_rate = temp/65.535;

        current_reading = yaw_rate;   
       
        if (fabs(current_reading - previous_reading) > threshold)
        {
            //integration of yaw_rate
            yaw_angle = yaw_angle + 0.5*0.1*(previous_reading + current_reading);
            // yaw_angle = rungeKuttaIntegration(yaw_angle, current_reading, timeStep);
            previous_reading = current_reading;
            
            //counting cycles beyond +360 or -360
            cycles = fabs(yaw_angle)/180;
            
            if (cycles > 0)
            {
                if (yaw_angle < 0)
                {
                    gyroHeading = yaw_angle + cycles*360;
                }
                else
                {
                    gyroHeading = yaw_angle - cycles*360;
                }
            }
            else
            {
                gyroHeading = yaw_angle;
            }
            
            //wrapping the heading within 0 - 360 degrees
            if (heading > 180)
            {
                gyroHeading -= 360;
            }
            else if (heading < -180)
            {
                gyroHeading += 360;
            }
            
            //if (++count % 20)
            //printValue("Heading: %lf\n",heading);
        }
        else
        {
            previous_reading = current_reading;
            //if (++count % 20)
            //printValue("Heading: %lf\n",heading);
        }
            
    }
    
//    temp = array_1[4] << 8 | array_1[5];
//    yaw_rate = (double) temp/65.535;
//    
//    //integration of yaw_rate
//    current_reading = yaw_rate;
//    if (fabs(current_reading - previous_reading)>0.5)
//    {
//        yaw_angle = yaw_angle + 0.5*0.1*(previous_reading + current_reading);
//        // yaw_angle = rungeKuttaIntegration(yaw_angle, current_reading, timeStep);
//        //counting cycles beyond +360 or -360
//        cycles = fabs(yaw_angle)/360;
//        
//        if (cycles > 0)
//        {
//            if (yaw_angle < 0)
//            {
//                heading = yaw_angle + cycles*360;
//            }
//            else
//            {
//                heading = yaw_angle - cycles*360;
//            }
//        }
//        else
//        {
//            heading = yaw_angle;
//        }
//        
//        //wrapping the heading within 0 - 360 degrees
//        if (heading < 0)
//        {
//            heading = heading + 360;
//        }
//    }  
//    previous_reading = current_reading;
//    
//    printValue("Yaw Rate %d Angle: %d\n", temp, (int) heading);
    
}

void initializeGyroscope() {
    I2C_1_Start();
    Timer_Gyroscope_Start();
    isr_gyroscope_StartEx(ISR_Handler_Gyroscope);
    
    yaw_rate = 0;
    yaw_angle = 0;
    previous_reading = 0;
    current_reading = 0;
    heading = 0;
    cycles = 0;
}

void shutdownGyroscope() {
    I2C_1_Stop();
    Timer_Gyroscope_Stop();
    isr_gyroscope_Stop();
}

void wakeupGyroscope() {
    //Bring the sensor out of sleep mode
    I2C_1_MasterSendStart(MPU6050,0); //Steps 1-2
    I2C_1_MasterWriteByte(0x6B); //Step 3
    I2C_1_MasterWriteByte(0x00); //Step 4
    I2C_1_MasterSendStop(); //Step 5
    CyDelay(1);
}

void setScalingFactorGyroscope() {
    //Set the scaling factor of the sensor
    I2C_1_MasterSendStart(MPU6050,0); //Steps 1-2
    I2C_1_MasterWriteByte(0x1B); //Step 3
    I2C_1_MasterWriteByte(0x08); //Step 4
    I2C_1_MasterSendStop(); //Step 5
    CyDelay(1);
    
}

void activateFilterGyroscope() {
    //Activating the low-pass filter
    I2C_1_MasterSendStart(MPU6050,0); //Steps 1-2
    I2C_1_MasterWriteByte(0x1A); //Step 3
    I2C_1_MasterWriteByte(0x05); //Step 4
    I2C_1_MasterSendStop(); //Step 5    
    CyDelay(1);
    
}

void startGyroscope() {
    printValue("Starting gyro\n");
    initializeGyroscope();
    wakeupGyroscope();
    setScalingFactorGyroscope();
    activateFilterGyroscope();
}

// Function to calculate the derivative of yaw angle with respect to time
double yawRateToYawAngleDerivative(double yawRate) {
    // This function should represent the relationship between yaw rate and yaw angle
    // In this example, we assume that yawRate is in degrees per second
    return yawRate; // This is a simple case, actual implementation might be more complex
}

// Runge-Kutta integration function
double rungeKuttaIntegration(double initialValue, double yawRate, double timeStep) {
    double k1, k2, k3, k4;
    double kSum;

    // Initialize the result
    double result = initialValue;
    
    // Perform the Runge-Kutta integration
    k1 = timeStep * yawRate;
    k2 = timeStep * yawRate + 0.5 * k1;
    k3 = timeStep * yawRate + 0.5 * k2;
    k4 = timeStep * yawRate + k3;

    kSum = (k1 + 2 * k2 + 2 * k3 + k4) / 6.0;
    result += kSum;

    return result;
}

double roundToDecimalPlaces(double value, int decimalPlaces) {
    double multiplier = pow(10, decimalPlaces);
    double roundedValue = round(value * multiplier) / multiplier;
    return roundedValue;
}




/* [] END OF FILE */
