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
#include "bluetooth.h"
#include "ultrasonic.h"
#include "ultrasonic_control.h"
#include "locomotion.h"
#include "colorsensor.h"
#include "servo_control.h"
#include "irsensor.h"
#include "gyroscope.h"
#include "navigation.h"

int main(void)
{
    CyGlobalIntEnable; /* Enable global interrupts. */
    
    StartingBase base_color = RedBase;
    /* Place your initialization/startup code here (e.g. MyInst_Start()) */
    initializeBluetooth();
    InitalizeUltrasonicSensor();
    

    for(;;)
    {
        bowlingPinWhichStripe();
        CyDelay(1500);
    }
}

/* [] END OF FILE */
