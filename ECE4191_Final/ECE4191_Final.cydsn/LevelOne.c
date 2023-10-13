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
#include "task.h"
#include "p_ctrl.h"
#include "pid_ctrl.h"
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
#include "task.h"
#include "LevelOne.h"
#include "math.h"

#define ENABLE_BT 1
#define HND_SHAKE 0 
#define MAX_SPEED 230
#define HALF_SPEED 230
#define TURN_SPEED 220

StartingBase base_color;
Color requiredColor_L1;
Color requiredColor_L2;

int STATE_CURR_LEVEL = 0;
int state = 0;
int facingRight = 0;


void run_L1() {
    initializeRobotBase();
    state_1_0();
    state_1_1();
    
}

void printState() {
    printValue("STATE: [%d] \n", state);   
}

void initializeRobotBase() {
    base_color = YellowBase;
    
    initializePosition(base_color);
    
    #if ENABLE_BT
        initializeBluetooth();
    #endif
    
    #if HND_SHAKE
        waitingHandshake();
        requiredColor_L1 = zoneColor;
        requiredColor_L2 = levelTwoZoneColor;
        STATE_CURR_LEVEL = currentLevel;
    #else
        requiredColor_L1 = BlueColor;
        requiredColor_L2 = RedColor;
        STATE_CURR_LEVEL = 1;
    #endif
    
    printValue("FINDING: %d\n", requiredColor_L1);
    InitalizeUltrasonicSensor();
}


void state_1_0() {
    
    printState();
    
    #if HND_SHAKE
    if (zoneColor == PinZoneColorBlue) {
        requiredColor_L1 = BlueColor;
    } else if (zoneColor == PinZoneColorRed) {
        requiredColor_L1 = RedColor;
    } else if (zoneColor == PinZoneColorGreen) {
        requiredColor_L1 = GreenColor;
    }   
    #endif
    
    wheel_move_by_metrics(Forward, 250, 0.25);
    
    if (base_color == YellowBase || base_color == BlueBase) {
        wheel_move_by_metrics(Left, 240, 30);
        wheel_move_by_metrics(Forward, 250, 0.7);
        wheel_move_by_metrics(Right, 240, 120);
        facingRight = 1;
    } else {
        wheel_move_by_metrics(Right, 240, 30);
        wheel_move_by_metrics(Forward, 250, 0.7);
        wheel_move_by_metrics(Left, 240, 120);
        facingRight = 0;
    }
    
    state = 1;
   
}

void state_1_1() {

    
    printState();
    
    biturningAlignment();
    CyDelay(300);
    read_U();
    bool obstacle_not_met = (FLU >= 22) && (FRU >= 22);
    // bool wall_not_met = (pos_x >= 0.1) && (pos_x <= 2.3) && (pos_y >= 0.1) && (pos_y <= 2.3);
    bool wall_not_met = true;
    
    int line_finish = 0;
    int raster_finish = 0;
    
    while (!raster_finish) {
        printValue("IN RASTER\n");
        line_finish = 0;
        
        lowerGripperToGround();
    
        while (!line_finish) {
            printValue("IN LINE\n");
            startIR();
            
            wheel_move(Forward, MAX_SPEED);
            
            while (infraredDetectionStatus == Absence && !line_finish) {
                read_U();
                line_finish = (FLU <= 22) && (FRU <= 22); 
                
            }
            
            wheel_move(StopMotion, MAX_SPEED);
            
            
            if (infraredDetectionStatus == Presence) {
                stopIR();
                wheel_move_by_metrics(Backward, TURN_SPEED, 0.015);
                ColorDetection_Run(1);
                printValue("DETECTED COLOR: %d\n", detectedColor);
                if (detectedColor == requiredColor_L1) {
                    printValue("OBTAINED WANTED PUCK\n");
                    grabPuckAndHold();
                    line_finish = 1;
                    raster_finish = 1;
                } else {
                    printValue("DISCARD\n");
                    grabPuckAndDiscard();
                }
            }
             
   
        }
        
        if (!raster_finish) {
            printValue("IN TURNING\n");
            stopIR();
            retractGripper();
            if (facingRight) {
                wheel_move_by_metrics(Left, MAX_SPEED, 90);   
                wheel_move_by_metrics(Forward, MAX_SPEED, 0.12);
                wheel_move_by_metrics(Left, MAX_SPEED, 90);  
                biturningAlignment();
            }
            else {
                wheel_move_by_metrics(Right, MAX_SPEED, 90);   
                wheel_move_by_metrics(Forward, MAX_SPEED, 0.12);
                wheel_move_by_metrics(Right, MAX_SPEED, 90);  
                biturningAlignment();
            }
        
            facingRight = !facingRight;
            line_finish = 0;
        }
    
    }
}

/* [] END OF FILE */
