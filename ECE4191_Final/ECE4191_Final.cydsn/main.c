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
#include <math.h>
#include "bluetooth.h"
#include "ultrasonic.h"
#include "ultrasonic_control.h"
#include "locomotion.h"
#include "colorsensor.h"
#include "servo_control.h"
#include "irsensor.h"
#include "gyroscope.h"
#include "navigation.h"
#include "behavior_tree.h"

int main(void)
{
    CyGlobalIntEnable; /* Enable global interrupts. */

    StartingBase base_color = RedBase;
    initializePosition(base_color);
    initializeBluetooth();
    InitalizeUltrasonicSensor();

//    GripperHand_Open();
//    GripperArm_Extend();
//    shutdown_Gripper();
//    
//    int finish = 0;
//    
//    while (!finish) {
//        startIR();
//        
//        wheel_move(Forward, 220);
//        
//        while (infraredDetectionStatus == Absence);
//        
//        wheel_move(StopMotion, 250);
//        stopIR();
//        wheel_move_by_metrics(Backward, 200, 0.0125);
//        ColorDetection_Run(1);
//        
//        if (detectedColor == RedColor) {
//            GripperHand_GripPuck();   
//            GripperArm_Retract();
//            shutdown_Gripper();
//            finish = 1;
//        } else if (detectedColor != GreyColor) {
//            GripperHand_GripPuck();
//            GripperArm_Retract();
//            GripperHand_Open();
//            GripperArm_Hurl();
//            //shutdown_Gripper();
//            GripperArm_SmallExtend();
//            shutdown_Gripper();
//            CyDelay(250);
//        } else {
//            
//            
//        }
//    
//    }

    for(;;)
    {
        ColorDetection_Run(1);
        
        CyDelay(2000);
    }
}

/* [] END OF FILE */
