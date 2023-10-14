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
#define MAX_SPEED 250
#define HALF_SPEED 200
#define TURN_SPEED 250
#define FACTOR_SURFACE 1.2

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
    state_1_2();
    state_1_3();
    state_1_4();
    state_1_5();
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
    
    wheel_move_by_metrics(Forward, MAX_SPEED, 0.4);
    
    if (base_color == YellowBase || base_color == BlueBase) {
        wheel_move_by_metrics(Right, TURN_SPEED, 90 * FACTOR_SURFACE);
        
        uniturningAlignment(TURN_SPEED, RightAlign);
        
        // biturningAlignment();
        
        //moveBackwardUntilObs(MAX_SPEED);
//        wheel_move_by_metrics(Backward, MAX_SPEED, 0.6);
        moveUntilObs(0, MAX_SPEED, 25);
        // biturningAlignment();
        uniturningAlignment(TURN_SPEED, BackAlign);
        
        wheel_move_by_metrics(Left, TURN_SPEED, 90 * FACTOR_SURFACE);
        
        uniturningAlignment(TURN_SPEED, LeftAlign);
        
        wheel_move_by_metrics(Forward, MAX_SPEED, 0.62);
        
        // uniturningAlignment(TURN_SPEED, LeftAlign);
        
        wheel_move_by_metrics(Right, TURN_SPEED, 90 * FACTOR_SURFACE);
        
        uniturningAlignment(TURN_SPEED, BackAlign);
        
        facingRight = 1;
    } else {
        wheel_move_by_metrics(Left, TURN_SPEED, 90 * FACTOR_SURFACE);
        
//        moveBackwardUntilObs(MAX_SPEED);
        
        wheel_move_by_metrics(Backward, MAX_SPEED, 0.6);
        
        biturningAlignment();

        wheel_move_by_metrics(Right, TURN_SPEED, 90 * FACTOR_SURFACE);
        
        wheel_move_by_metrics(Forward, MAX_SPEED, 0.6);
 
        wheel_move_by_metrics(Left, TURN_SPEED, 90 * FACTOR_SURFACE);
        
        biturningAlignment();
        facingRight = 0;
    }
    
    state = 1;
   
}

void state_1_1() {

    
    read_U();
    bool obstacle_not_met = (FLU >= 22) || (FRU >= 22);
    // bool wall_not_met = (pos_x >= 0.1) && (pos_x <= 2.3) && (pos_y >= 0.1) && (pos_y <= 2.3);
    bool wall_not_met = true;
    
    int line_not_finish = 1;
    int raster_finish = 0;
    
    while (!raster_finish) {
        printValue("IN RASTER\n");
        line_not_finish = 1;
        
        lowerGripperToGround();
    
        while (line_not_finish) {
            printValue("IN LINE\n");
            startIR();
            
            wheel_move(Forward, MAX_SPEED);
            
            while (infraredDetectionStatus == Absence && line_not_finish) {
                read_U();
                line_not_finish = (FLU >= 27) || (FRU >= 27); 
                
            }
            
            wheel_move(StopMotion, MAX_SPEED);
            
            
            if (infraredDetectionStatus == Presence) {
                stopIR();
                wheel_move_by_metrics(Backward, TURN_SPEED, 0.025);
                ColorDetection_Run(1);
                printValue("DETECTED COLOR: %d\n", detectedColor);
                if (detectedColor == requiredColor_L1) {
                    printValue("OBTAINED WANTED PUCK\n");
                    grabPuckAndHold();
                    line_not_finish = 0;
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
                uniturningAlignment(TURN_SPEED, FrontAlign);
                wheel_move_by_metrics(Left, MAX_SPEED, 90 * FACTOR_SURFACE);   
                wheel_move_by_metrics(Forward, MAX_SPEED, 0.15);
                wheel_move_by_metrics(Left, MAX_SPEED, 90 * FACTOR_SURFACE);  
                uniturningAlignment(TURN_SPEED, BackAlign);
            }
            else {
                uniturningAlignment(TURN_SPEED, FrontAlign);
                wheel_move_by_metrics(Right, MAX_SPEED, 90 * FACTOR_SURFACE);   
                wheel_move_by_metrics(Forward, MAX_SPEED, 0.15);
                wheel_move_by_metrics(Right, MAX_SPEED, 90 * FACTOR_SURFACE);  
                uniturningAlignment(TURN_SPEED, BackAlign);
            }
        
            facingRight = !facingRight;
            line_not_finish = 1;
        }
    
    }
}


void state_1_2 () {
    
    if (base_color == YellowBase || base_color == BlueBase) {
        if (facingRight) {
            moveUntilObs(0, MAX_SPEED, 20);
            uniturningAlignment(TURN_SPEED, BackAlign);
            wheel_move_by_metrics(Right, TURN_SPEED, 90 * FACTOR_SURFACE);
            uniturningAlignment(TURN_SPEED, RightAlign);
            moveUntilObs(1, MAX_SPEED, 28);
            wheel_move_by_metrics(Left, TURN_SPEED, 90 * FACTOR_SURFACE);
            
        } else {
            moveUntilObs(1, MAX_SPEED, 20);
            uniturningAlignment(TURN_SPEED, FrontAlign);
            wheel_move_by_metrics(Left, TURN_SPEED, 90 * FACTOR_SURFACE);
            uniturningAlignment(TURN_SPEED, LeftAlign);
            moveUntilObs(1, MAX_SPEED, 28);
            wheel_move_by_metrics(Left, TURN_SPEED, 90 * FACTOR_SURFACE);
            
            facingRight = 1;
        }
        
    }
    
    else {
        
        if (facingRight) {
            moveUntilObs(1, MAX_SPEED, 10);
            biturningAlignment();
            wheel_move_by_metrics(Left, TURN_SPEED, 90);
            biturningAlignment();
            moveUntilObs(1, HALF_SPEED, 25);
            wheel_move_by_metrics(Right, TURN_SPEED, 90);
            
            facingRight = 0;
        } else {
            moveUntilObs(0, MAX_SPEED, 10);
            biturningAlignment();
            wheel_move_by_metrics(Right, TURN_SPEED, 90);
            biturningAlignment();
            moveUntilObs(1, HALF_SPEED, 25);
            wheel_move_by_metrics(Left, TURN_SPEED, 90);
        }
        
    }
    
}

void state_1_3() {
    
    biturningAlignment();
    
    read_U();
    
    bool notSeen = true;
    bool arenaWallNotTooFar = true;
    bool arenaWallNotTooClose = false;
    
    if (base_color == YellowBase || base_color == BlueBase) {
        while (notSeen) {
            uniturningAlignment(TURN_SPEED, BackAlign);
            
            arenaWallNotTooFar = (BLU <= 55) && (BRU <= 55);
        
            wheel_move(Forward, HALF_SPEED);
            
            while (arenaWallNotTooFar) {
                read_U();
                arenaWallNotTooFar = (BLU <= 55) && (BRU <= 55);
                if (!(fabs(RFU - RBU) <= 10))
                    notSeen = RFU < RBU;
                
                if (!notSeen) break;
            }
            
            wheel_move(StopMotion, HALF_SPEED);
        
            CyDelay(300);
            
            
            if (!notSeen) return;
            
            read_U();
            arenaWallNotTooClose = (BLU >= 14) || (BRU >= 14);
            
            uniturningAlignment(TURN_SPEED, RightAlign);
            
            wheel_move(Backward, HALF_SPEED);
 
            while (arenaWallNotTooClose) {
                read_U();
                arenaWallNotTooClose = (BLU >= 14) || (BRU >= 14);
                if (!(fabs(RFU - RBU) <= 10) && arenaWallNotTooClose)
                {
                    notSeen = RFU < RBU;
                    
                }

                
                if (!notSeen) break;
            }
            
            wheel_move(StopMotion, HALF_SPEED);
        
            if (!notSeen) return;

            uniturningAlignment(TURN_SPEED, BackAlign);
        }
        
    }
    
    else {
        while (notSeen) {
            
            
        }
  
    }
  
}

void state_1_4() {
    
    if (base_color == YellowBase || base_color == BlueBase) {
        
        wheel_move_by_metrics(Right, TURN_SPEED, 90 * FACTOR_SURFACE);
        uniturningAlignment(TURN_SPEED, FrontAlign);
        
        read_U();
        
        double dist = FLU > FRU ? FLU : FRU;
        
        dist = (dist - 25 > 0)? dist - 25 : 25 - dist;
        
        if (dist > 0) {
            wheel_move_by_metrics(Forward, HALF_SPEED, dist/100);
        } else {
            wheel_move_by_metrics(Backward, HALF_SPEED, dist/100);
        }
        
        placePuckAtDeck();
        shoot();
        
    } else {
        
        
    }
}

void state_1_5() {
    
    if (base_color == YellowBase || base_color == BlueBase) {
        
        wheel_move_by_metrics(Left, TURN_SPEED, 90 * FACTOR_SURFACE);
        uniturningAlignment(TURN_SPEED, RightAlign);
        
        read_U();
        
        double dist = 0.8 - (BLU + BRU)/200;
        
        wheel_move_by_metrics(Forward, MAX_SPEED, dist);
        
        wheel_move_by_metrics(Left, TURN_SPEED, 90 * FACTOR_SURFACE);
        
        uniturningAlignment(TURN_SPEED, BackAlign);
    }
    
    
    read_U();
    
    double backDist = (BLU + BRU)/200;
    
    wheel_move_by_metrics(Backward, MAX_SPEED, backDist);
    
}

/* [] END OF FILE */
