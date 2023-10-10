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
#include "trialRun.h"
#include "locomotion.h"
#include "navigation.h"
#include "ultrasonic.h"
#include "ultrasonic_control.h"
#include "bluetooth.h"
#include "colorsensor.h"
#include "gyroscope.h"
#include "irsensor.h"
#include "servo_control.h"
#include "behavior_tree.h"
#include "behavior.h"

// BlueBase, RedBase, YellowBase, GreenBase
StartingBase base_color;
Color requiredColor;
void initializeRobot() {
    base_color = RedBase;
    
    initializePosition(base_color);
    initializeBluetooth();
    InitalizeUltrasonicSensor();
    startGyroscope();

    waitingHandshake();
    
}

void run();

void runLevelOne() {
    
    if (zoneColor == PinZoneColorBlue) {
        requiredColor = BlueColor;
    } else if (zoneColor == PinZoneColorRed) {
        requiredColor = RedColor;
    } else if (zoneColor == PinZoneColorGreen) {
        requiredColor = GreenColor;
    }
    
    int facingRight;
    
    wheel_move_by_metrics(Forward, 250, 0.8);
    
    if (base_color == RedBase || base_color == BlueBase) {
        wheel_move_by_metrics_with_gyro(Right, 250, 90);
        facingRight = 1;
    } else {
        wheel_move_by_metrics_with_gyro(Left, 250, 90);
        facingRight = 0;
    }
    
    GripperHand_Open();
    GripperArm_Extend();
    shutdown_Gripper();
    
    int rasterFinish = 0;
    double FLU = 0, FRU = 0;
    
    FLU = UltrasonicSensor_ReadDistanceData(0);
    FRU = UltrasonicSensor_ReadDistanceData(1);
    
    // IF a robot is in front, it will have same problem
    
    bool obstacle_not_met = (FLU >= 15) && (FRU >= 15);
    
    
    while (!rasterFinish) {
        startIR();
        
        wheel_move(Forward, 250);
        
        while (infraredDetectionStatus == Absence && obstacle_not_met) {
            FLU = UltrasonicSensor_ReadDistanceData(0);
            FRU = UltrasonicSensor_ReadDistanceData(1);
            obstacle_not_met = (FLU >= 15) && (FRU >= 15);
        }
        
        wheel_move(StopMotion, 250);
        stopIR();

        if (infraredDetectionStatus == Presence) {
            
            wheel_move_by_metrics(Backward, 200, 0.015);
            ColorDetection_Run(2);
            GripperHand_GripPuck();   
            GripperArm_Retract();
            
            if (detectedColor == requiredColor) {
                shutdown_Gripper();
                rasterFinish = 1;
            } else {
                
                wheel_move_by_metrics_with_gyro(Right, 250, 20);
                GripperArm_Extend();
                GripperHand_Open();
                GripperArm_Retract();
                GripperHand_GripPuck();
                wheel_move_by_metrics_with_gyro(Left, 250, 20);
                shutdown_Gripper();
            }
            
        } else if (!obstacle_not_met) {
            GripperArm_Retract();
            GripperHand_GripPuck();
            shutdown_Gripper();
            // raster it
            
            if (facingRight) {
                wheel_move_by_metrics_with_gyro(Left, 250, 90);
                wheel_move_by_metrics_with_gyro(Forward, 250, 0.1);
                wheel_move_by_metrics_with_gyro(Left, 250, 90);
            } else {
                wheel_move_by_metrics_with_gyro(Right, 250, 90);
                wheel_move_by_metrics_with_gyro(Forward, 250, 0.1);
                wheel_move_by_metrics_with_gyro(Right, 250, 90);
            }
            
            facingRight = !facingRight;
        }
        
    }
    
    // Return to pin deck
    double BLU = 0, BRU = 0;
    
    BLU = UltrasonicSensor_ReadDistanceData(4);
    BRU = UltrasonicSensor_ReadDistanceData(5);
    FLU = UltrasonicSensor_ReadDistanceData(0);
    FRU = UltrasonicSensor_ReadDistanceData(1);
    
    if (facingRight) {
        obstacle_not_met = (BLU >= 10) && (BRU >= 10);
        
        wheel_move(Backward, 250);
        
        while (obstacle_not_met) {
            BLU = UltrasonicSensor_ReadDistanceData(4);
            BRU = UltrasonicSensor_ReadDistanceData(5);
            obstacle_not_met = (BLU >= 10) && (BRU >= 10);
        }
        
        wheel_move(StopMotion, 250);
        wheel_move_by_metrics_with_gyro(Right, 250, 90);
        
        FLU = UltrasonicSensor_ReadDistanceData(0);
        FRU = UltrasonicSensor_ReadDistanceData(1);
        
        obstacle_not_met = (FLU >= 28) && (FRU >= 28);
        
        wheel_move(Forward, 220);
        
        while (obstacle_not_met) {
            FLU = UltrasonicSensor_ReadDistanceData(0);
            FRU = UltrasonicSensor_ReadDistanceData(1);
            obstacle_not_met = (FLU >= 28) && (FRU >= 28);
        }
        
        wheel_move(StopMotion, 220);
        
        
    } else {
        obstacle_not_met = (FLU >= 10) && (FRU >= 10);
        wheel_move(Forward, 250);
        
    }
    
    
    
}

/* [] END OF FILE */
