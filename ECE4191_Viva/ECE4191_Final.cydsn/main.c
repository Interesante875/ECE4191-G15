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


int wait = 1;

CY_ISR (ISR_Handler_Press_Pin) {
    
    wait = 0;
}

void wait_press_button() {
    while (wait);
    printValue("Start\n");
}

int main(void)
{
    CyGlobalIntEnable; /* Enable global interrupts. */

    int puck_count = 0;
    int puck_color[3] = {0};
    
    StartingBase base_color = RedBase;
    initializePosition(base_color);
    initializeBluetooth();
    
    isr_press_pin_StartEx(ISR_Handler_Press_Pin);
    wait_press_button();
//    InitalizeUltrasonicSensor();

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
        
        wheel_move(Forward, 250);
        
        while (infraredDetectionStatus == Absence);
        
        wheel_move(StopMotion, 250);
        stopIR();
        wheel_move_by_metrics(Backward, 190, 0.015);
        
        ColorDetection_Run(1);

        printColor();
        
        if (detectedColor == RedColor) {
            puck_color[0] += 1;    
            puck_count += 1;
        } else if (detectedColor == GreenColor) {
            puck_color[1] += 1;    
            puck_count += 1;
        } else if (detectedColor == BlueColor) {
            puck_color[2] += 1;   
            puck_count += 1;
        }
        
        if (puck_count == 2) {
            
            int max_count = 0;
            
            for (int i =0; i < 3; i++) {
                if (puck_color[i] == 2) max_count = 1;
            }
            
            finish = 1;
            if (!max_count) {
                // FLICK   
                GripperHand_Open();
                GripperArm_Retract();
                
                FlickerLock_Lock();
                FlickerRecoil_Load();
                FlickerRecoil_Unload();
                wheel_move_by_metrics(Forward, 210, 0.085);
                FlickerLock_Unlock();
                FlickerLock_Lock();
                
                printValue("Current Distance: %lf, move: %lf\n", pos_x, pos_x - 0.05);
                double distance = pos_x - 0.05;
                wheel_move_by_metrics(Backward, 250, distance);
                
            } else {
                // RETREAT   
                GripperArm_Retract();
                GripperHand_Close();
                printValue("Current Distance: %lf, move: %lf\n", pos_x, pos_x - 0.05);
                double distance = pos_x - 0.05;
                wheel_move_by_metrics(Backward, 230, distance);
            }
            
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
