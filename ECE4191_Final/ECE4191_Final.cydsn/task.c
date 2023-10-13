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


#include "math.h"

#define TEST_RUN 1
#define ENABLE_BT_HND 0 
#define ENABLE_BT 1
// BlueBase, RedBase, YellowBase, GreenBase


void retractGripper() {
    GripperHand_GripPuck();
    GripperArm_Retract();
    
    CyDelay(100);
    
    shutdown_Gripper();
}

void lowerGripperToGround() {
    
    GripperHand_Open();
    GripperArm_Extend();
    
    CyDelay(100);
    
    shutdown_Gripper();
    
}

void grabPuckAndHold() {
    
    GripperHand_GripPuck();   
    GripperArm_Retract();
    
    CyDelay(100);
    
    shutdown_Gripper();
    
}

void grabPuckAndDiscard() {

    GripperHand_GripPuck();   
    GripperArm_Retract();
    wheel_move_by_metrics(Right, 240, 20);
    GripperArm_Extend();
    GripperHand_Open();
    
    CyDelay(100);
    
    GripperArm_Retract();

    wheel_move_by_metrics(Left, 240, 20);
    GripperArm_Extend();
    
    CyDelay(100);
    
    shutdown_Gripper();
    
}

void grabPuckAndHurl() {
    
    // NOT IMPLEMENTED
    return;
}

void placePuckAtDeck() {
    
    GripperArm_Extend();
    GripperHand_Open();
    
    CyDelay(100);
    
    GripperArm_Retract();
    GripperHand_GripPuck();
    
    CyDelay(100);
    
    shutdown_Gripper();
    
}


void shoot() {
    
    FlickerLock_Lock();
    CyDelay(100);
    FlickerRecoil_Load();
    CyDelay(200);
    FlickerRecoil_Unload();
    CyDelay(200);
    
    wheel_move_by_metrics(Forward, 210, 0.065);
    FlickerLock_Unlock();
    CyDelay(200);
    FlickerLock_Lock();
    
}

void biturningAlignment() {
    angle_correction(245);
    angle_correction(225);
}

int wallIsSeen() {
    int proximity_left = (pos_x < 0.1);
    int proximity_right = (pos_x > 2.3);
    int proximity_top = (pos_y < 0.1);
    int proximity_bottom = (pos_y > 2.3);
    
    int proximity = (pos_x < 0.1) || (pos_x > 2.3) || (pos_y < 0.1) || (pos_y > 2.3);
    
    if (heading_angle > 0 && proximity_right) {
        printValue("SEEN WALL\n");
        return 1;     
    }
    if (heading_angle < 0 && proximity_left) {
        printValue("SEEN WALL\n");
        return 1;     
    }
    return 0;
}

//void detectPin_LeftUDS() {
//    
//    if (!(base_color == RedBase || base_color == GreenBase)) {
//        return;
//    }
//        
//
//    wheel_move_by_metrics(Left, 220, 90);
//
//    CyDelay(300);
//    read_U();
//    
//    #if ENABLE_BT
//        printValue("START SEACHING DISTANCE TO WALL\n");
//        print_U();
//    #endif
//    
//    bool notSeen = true;
//
//    while (notSeen) {
//        
//        wheel_move(Backward, 195);
//        bool arenaWallNotMet = true;
//        bool arenaWallNotTooFar = true;
//        
//        while (arenaWallNotMet) {
//            read_U();
//
//            arenaWallNotMet = (BLU >= 18) && (BRU >= 18);
//            if (!(fabs(LFU - LBU) <= 10) && arenaWallNotMet)
//            {
//                notSeen = LFU < LBU;                 
//            }
//            
//            if (!notSeen) break;
//        }
//        
//        wheel_move(StopMotion, 200);
//        
//        read_U();
//        CyDelay(500);
//        read_U();
//        
//        if (!notSeen) {
//
//            printValue("NOW SEEN: DISTANCE TO WALL\n");
//            printValue("BLU: %.2lf BRU: %.2lf DETECTED: %d\n", BLU, BRU);
//
//            // DETERMINE ZONE
//            
//            double distanceToWall = (BLU + BRU) / 2;
//            if (distanceToWall <= 28) {
//                levelThreePinDeckNum = 6;
//                levelThreeZoneColor = PinZoneColorRed;
//                printValue("ZONE RED: %d\n", levelThreePinDeckNum);
//            } 
//            else if (distanceToWall <= 32) {
//                levelThreePinDeckNum = 5;
//                levelThreeZoneColor = PinZoneColorGreen;
//                printValue("ZONE GREEN: %d\n", levelThreePinDeckNum);
//                
//            }
//            else if (distanceToWall <= 37) {
//                levelThreePinDeckNum = 4;
//                levelThreeZoneColor = PinZoneColorBlue;
//                printValue("ZONE BLUE: %d\n", levelThreePinDeckNum);
//                
//            }
//            else if (distanceToWall <= 42) {
//                levelThreePinDeckNum = 3;
//                levelThreeZoneColor = PinZoneColorRed;
//                printValue("ZONE RED: %d\n", levelThreePinDeckNum);
//                
//            }
//            else if (distanceToWall <= 47) {
//                levelThreePinDeckNum = 2;
//                levelThreeZoneColor = PinZoneColorGreen;
//                printValue("ZONE GREEN: %d\n", levelThreePinDeckNum);
//                
//            }
//            else if (distanceToWall <= 52) {
//                levelThreePinDeckNum = 1;
//                levelThreeZoneColor = PinZoneColorBlue;
//                printValue("ZONE BLUE: %d\n", levelThreePinDeckNum);
//                
//            }
//            // THEN RETURN
//            return;
//        } 
//        
//        read_U();
//        printValue("Forward DISTANCE TO WALL\n");
//        print_U();
//        
//        // angle_correction(ADJUST_PWM, FLU, FRU);
//        
//        arenaWallNotTooFar = (BLU <= 50) && (BRU <= 50);
//        
//        wheel_move(Forward, 195);
//        
//        while (arenaWallNotTooFar) {
//            read_U();
//            arenaWallNotTooFar = (BLU <= 50) && (BRU <= 50);
//            if (!(fabs(LFU - LBU) <= 10))
//                notSeen = LFU < LBU;
//            
//
//            if (!notSeen) break;
//        }
//        
//        wheel_move(StopMotion, 200);
//        
//        CyDelay(300);
//        read_U();
//        // angle_correction(ADJUST_PWM, RFU, RBU);
//        read_U();
//        
//        if (!notSeen) {
//
//            CyDelay(200);
//
//            double distanceToWall = (BLU + BRU) / 2;
//            
//            if (distanceToWall <= 28) {
//                levelThreePinDeckNum = 6;
//                levelThreeZoneColor = PinZoneColorRed;
//                printValue("ZONE RED: %d\n", levelThreePinDeckNum);
//            } 
//            else if (distanceToWall <= 32) {
//                levelThreePinDeckNum = 5;
//                levelThreeZoneColor = PinZoneColorGreen;
//                printValue("ZONE GREEN: %d\n", levelThreePinDeckNum);
//                
//            }
//            else if (distanceToWall <= 37) {
//                levelThreePinDeckNum = 4;
//                levelThreeZoneColor = PinZoneColorBlue;
//                printValue("ZONE BLUE: %d\n", levelThreePinDeckNum);
//                
//            }
//            else if (distanceToWall <= 42) {
//                levelThreePinDeckNum = 3;
//                levelThreeZoneColor = PinZoneColorRed;
//                printValue("ZONE RED: %d\n", levelThreePinDeckNum);
//                
//            }
//            else if (distanceToWall <= 47) {
//                levelThreePinDeckNum = 2;
//                levelThreeZoneColor = PinZoneColorGreen;
//                printValue("ZONE GREEN: %d\n", levelThreePinDeckNum);
//                
//            }
//            else if (distanceToWall <= 52) {
//                levelThreePinDeckNum = 1;
//                levelThreeZoneColor = PinZoneColorBlue;
//                printValue("ZONE BLUE: %d\n", levelThreePinDeckNum);
//                
//            }
//            // THEN RETURN
//            return;
//        } 
//        
//    }
//    
//}

//void detectPin_RightUDS() {
//    
//    if (!(base_color == YellowBase || base_color == BlueBase)) {
//        return;
//    }
//    
//    
//    wheel_move_by_metrics(Right, 220, 90);
//
//    CyDelay(300);
//    read_U();
//    
//    #if ENABLE_BT
//        printValue("START SEACHING DISTANCE TO WALL\n");
//        print_U();
//    #endif
//    
//    bool notSeen = true;
//
//    while (notSeen) {
//        
//        wheel_move(Backward, 195);
//        bool arenaWallNotMet = true;
//        bool arenaWallNotTooFar = true;
//        
//        while (arenaWallNotMet) {
//            read_U();
//
//            arenaWallNotMet = (BLU >= 18) && (BRU >= 18);
//            if (!(fabs(RFU - RBU) <= 10) && arenaWallNotMet)
//            {
//                notSeen = RFU < RBU;                 
//            }
//            
//            if (!notSeen) break;
//        }
//        
//        wheel_move(StopMotion, 200);
//        
//        read_U();
//        CyDelay(500);
//        read_U();
//        
//        if (!notSeen) {
//
//            printValue("NOW SEEN: DISTANCE TO WALL\n");
//            printValue("BLU: %.2lf BRU: %.2lf DETECTED: %d\n", BLU, BRU);
//
//            // DETERMINE ZONE
//            
//            double distanceToWall = (BLU + BRU) / 2;
//            if (distanceToWall <= 28) {
//                levelThreePinDeckNum = 6;
//                levelThreeZoneColor = PinZoneColorRed;
//                printValue("ZONE RED: %d\n", levelThreePinDeckNum);
//            } 
//            else if (distanceToWall <= 32) {
//                levelThreePinDeckNum = 5;
//                levelThreeZoneColor = PinZoneColorGreen;
//                printValue("ZONE GREEN: %d\n", levelThreePinDeckNum);
//                
//            }
//            else if (distanceToWall <= 37) {
//                levelThreePinDeckNum = 4;
//                levelThreeZoneColor = PinZoneColorBlue;
//                printValue("ZONE BLUE: %d\n", levelThreePinDeckNum);
//                
//            }
//            else if (distanceToWall <= 42) {
//                levelThreePinDeckNum = 3;
//                levelThreeZoneColor = PinZoneColorRed;
//                printValue("ZONE RED: %d\n", levelThreePinDeckNum);
//                
//            }
//            else if (distanceToWall <= 47) {
//                levelThreePinDeckNum = 2;
//                levelThreeZoneColor = PinZoneColorGreen;
//                printValue("ZONE GREEN: %d\n", levelThreePinDeckNum);
//                
//            }
//            else if (distanceToWall <= 52) {
//                levelThreePinDeckNum = 1;
//                levelThreeZoneColor = PinZoneColorBlue;
//                printValue("ZONE BLUE: %d\n", levelThreePinDeckNum);
//                
//            }
//            // THEN RETURN
//            return;
//        } 
//        
//        read_U();
//        printValue("Forward DISTANCE TO WALL\n");
//        print_U();
//        
//        // angle_correction(ADJUST_PWM, FLU, FRU);
//        
//        arenaWallNotTooFar = (BLU <= 50) && (BRU <= 50);
//        
//        wheel_move(Forward, 195);
//        
//        while (arenaWallNotTooFar) {
//            read_U();
//            arenaWallNotTooFar = (BLU <= 50) && (BRU <= 50);
//            if (!(fabs(RFU - RBU) <= 10))
//                notSeen = RFU < RBU;
//            
//
//            if (!notSeen) break;
//        }
//        
//        wheel_move(StopMotion, 200);
//        
//        CyDelay(300);
//        read_U();
//        // angle_correction(ADJUST_PWM, RFU, RBU);
//        read_U();
//        
//        if (!notSeen) {
//
//            CyDelay(200);
//
//            double distanceToWall = (BLU + BRU) / 2;
//            
//            if (distanceToWall <= 28) {
//                levelThreePinDeckNum = 6;
//                levelThreeZoneColor = PinZoneColorRed;
//                printValue("ZONE RED: %d\n", levelThreePinDeckNum);
//            } 
//            else if (distanceToWall <= 32) {
//                levelThreePinDeckNum = 5;
//                levelThreeZoneColor = PinZoneColorGreen;
//                printValue("ZONE GREEN: %d\n", levelThreePinDeckNum);
//                
//            }
//            else if (distanceToWall <= 37) {
//                levelThreePinDeckNum = 4;
//                levelThreeZoneColor = PinZoneColorBlue;
//                printValue("ZONE BLUE: %d\n", levelThreePinDeckNum);
//                
//            }
//            else if (distanceToWall <= 42) {
//                levelThreePinDeckNum = 3;
//                levelThreeZoneColor = PinZoneColorRed;
//                printValue("ZONE RED: %d\n", levelThreePinDeckNum);
//                
//            }
//            else if (distanceToWall <= 47) {
//                levelThreePinDeckNum = 2;
//                levelThreeZoneColor = PinZoneColorGreen;
//                printValue("ZONE GREEN: %d\n", levelThreePinDeckNum);
//                
//            }
//            else if (distanceToWall <= 52) {
//                levelThreePinDeckNum = 1;
//                levelThreeZoneColor = PinZoneColorBlue;
//                printValue("ZONE BLUE: %d\n", levelThreePinDeckNum);
//                
//            }
//            // THEN RETURN
//            return;
//        } 
//        
//    }
//}
//
//void detectPin() {
//    
//    if (base_color == RedBase || base_color == GreenBase) {
//        detectPin_LeftUDS();
//    }   
//    else {
//        detectPin_RightUDS();
//    }
//    
//}




/* [] END OF FILE */
