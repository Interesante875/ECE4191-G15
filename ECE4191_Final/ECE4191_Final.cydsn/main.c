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

//    CyDelay(1000);
//    GripperHand_GripPuck();
//    GripperArm_Extend();
//    GripperHand_Open();
//    GripperArm_Retract();
//    GripperHand_Close();
//    FlickerLock_Lock();
//    FlickerRecoil_Load();
//    FlickerRecoil_Unload();
//    wheel_move_by_metrics(Forward, 230, 0.07);
//    FlickerLock_Unlock();
//    FlickerLock_Lock();
    
    
    GripperHand_Open();
    GripperArm_Extend();
    shutdown_Gripper();
    
    int finish = 0;
    
    while (!finish) {
        startIR();
        
        wheel_move(Forward, 230);
        
        while (infraredDetectionStatus == Absence);
        
        wheel_move(StopMotion, 250);
        stopIR();
        wheel_move_by_metrics(Backward, 200, 0.01);
        
        ColorDetection_Run(1);
        
        if (detectedColor == RedColor) {
            GripperHand_GripPuck();   
            GripperArm_Retract();
            shutdown_Gripper();
            finish = 1;
        } else if (detectedColor != GreyColor) {
            GripperHand_GripPuck();   
            GripperArm_Retract();
//            GripperHand_Open();
//            GripperArm_Hurl();
//            GripperArm_SmallExtend();
            shutdown_Gripper();
            
            wheel_move_by_metrics(Right, 250, 20);
            GripperArm_Extend();
            GripperHand_Open();
            GripperArm_Retract();
            shutdown_Gripper();
            wheel_move_by_metrics(Left, 250, 20);
            GripperArm_Extend();
            shutdown_Gripper();
        } else {
            
            
        }
    
    }

    for(;;)
    {
//        ColorDetection_Run(1);
//        
//        CyDelay(2000);
    }
}

/* [] END OF FILE */
