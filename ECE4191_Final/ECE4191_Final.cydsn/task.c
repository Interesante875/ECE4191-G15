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

double minUltrasonicDistance;
// BlueBase, RedBase, YellowBase, GreenBase
Alignment findMinUltrasonic(int arrayIndex) {
    float maxUltrasonic[] = {(FLU > FRU)?FLU:FRU, (BLU > BRU)?BLU:BRU, (LFU > LBU)?LFU:LBU, (RFU > RBU)?RFU:RBU};
    Alignment alignArray[] = {FrontAlign, BackAlign, LeftAlign, RightAlign};
    int size = sizeof(maxUltrasonic) / sizeof(maxUltrasonic[0]); // Calculate the size of the array

    int min = maxUltrasonic[0]; // Assume the first element is the minimum
    int minIndex = 0; // Index of the minimum element

    // Iterate through indices 1, 2, and 3 to find the minimum value and its index
    for (int i = 1; i < size; ++i) {
        if (i != arrayIndex && maxUltrasonic[i] < min) {
            min = maxUltrasonic[i];
            minIndex = i;
        }
    }
    minUltrasonicDistance = min;
    return alignArray[minIndex];
}

void retractGripper() {
    GripperHand_GripPuck();
    CyDelay(300);
    GripperArm_Retract();
    
    CyDelay(300);
    
    shutdown_Gripper();
}

void lowerGripperToGround() {
    
    GripperHand_Open();
    CyDelay(300);
    GripperArm_Extend();   
    CyDelay(300);
    
    shutdown_Gripper();
    
}

void grabPuckAndHold() {
    
    GripperHand_GripPuck();  
    CyDelay(300);
    GripperArm_Retract();
    CyDelay(300);
    
    shutdown_Gripper();
    
}

void grabPuckAndDiscard() {

    GripperHand_GripPuck(); 
    
    GripperArm_Retract();
    shutdown_Gripper();
    
    wheel_move_by_metrics(Right, 235, 30);
    GripperArm_Extend();
    GripperHand_Open();
    
    CyDelay(300);
    
    GripperArm_Retract();

    wheel_move_by_metrics(Left, 235, 30);
    GripperArm_Extend();
    
    CyDelay(300);
    
    shutdown_Gripper();
    
}

void grabPuckAndHurl() {
    
    // NOT IMPLEMENTED
    return;
}

void placePuckAtDeck() {
    
    GripperHand_GripPuck();
    GripperArm_Extend();
    GripperHand_Open();
    
    CyDelay(400);
    
    GripperArm_Retract();
    GripperHand_GripPuck();
    
    CyDelay(400);
    
    shutdown_Gripper();
    
}


void shoot() {
    
    FlickerLock_Lock();
    CyDelay(100);
    FlickerRecoil_Load();
    CyDelay(500);
    FlickerRecoil_Unload();
    CyDelay(500);
    
    wheel_move_by_metrics(Forward, 210, 0.0625);
    CyDelay(1000);
    FlickerLock_Unlock();
    CyDelay(200);
    FlickerLock_Lock();
    
}

void biturningAlignment() {
    angle_correction(245);
    angle_correction(225);
}

void uniturningAlignment(int speed, Alignment align) {
    CyDelay(180);
    if (align == LeftAlign) {
        angle_correction_with_sides(speed, 3);
    } 
    else if (align == RightAlign) {
        angle_correction_with_sides(speed, 2);
    } 
    else if(align == FrontAlign) {
        angle_correction_with_sides(speed, 0);
    } 
    else if (align == BackAlign) {
        angle_correction_with_sides(speed, 1);
    } 
    
}


void multiTurningAlignment(int speed, Alignment align, int times) {
    CyDelay(180);
    for (int i = 0; i < times; i++) {
        if (align == LeftAlign) {
            angle_correction_with_sides(speed, 3);
        } 
        else if (align == RightAlign) {
            angle_correction_with_sides(speed, 2);
        } 
        else if(align == FrontAlign) {
            angle_correction_with_sides(speed, 0);
        } 
        else if (align == BackAlign) {
            angle_correction_with_sides(speed, 1);
        } 
    }
    
}

void infiniteTurningAlignment(int speed, Alignment align) {
    CyDelay(180);
    double currAng = 0;
    double prevAng = 0;
    double deviation = 360;
    int count = 0;
    while(deviation > 5 && count++<=6) {
        if (align == LeftAlign) {
            currAng = angle_correction_with_sides_return(speed, 3);
        } 
        else if (align == RightAlign) {
            currAng = angle_correction_with_sides_return(speed, 2);
        } 
        else if(align == FrontAlign) {
            currAng = angle_correction_with_sides_return(speed, 0);
        } 
        else if (align == BackAlign) {
            currAng = angle_correction_with_sides_return(speed, 1);
        } 
        
        deviation = fabs(currAng - prevAng);
        prevAng = currAng;
    }
}


void moveUntilObs(int dir, int speed, double dist) {
    
    CyDelay(180);
    
    printValue("MOVING BACK");
    
    if (dir == 0) {

        wheel_move(Backward, speed);
        bool obsNotMet = true;
        
        while (obsNotMet) {
            read_U();
            obsNotMet = (BLU >= dist) || (BRU >= dist);
        }
        
        wheel_move(StopMotion, speed); 
        
    } else {
        wheel_move(Forward, speed);
        bool obsNotMet = true;
        
        while (obsNotMet) {
            read_U();
            obsNotMet = (FLU >= dist) || (FRU >= dist);
        }
        
        wheel_move(StopMotion, speed);    
        
    }

}


int computeZoneNum(double dist2Wall, int front_or_back) {
    
    int deckNum = 0;
    
    if (front_or_back) {
        if (dist2Wall <= 20) {
            deckNum = 6;
            printValue("DECK NUM: %d\n", deckNum);
            return deckNum;
        }
        else if (dist2Wall <= 25) {
            deckNum = 5;
            printValue("DECK NUM: %d\n", deckNum);
            return deckNum;
        }
        else if (dist2Wall <= 30) {
            deckNum = 4;
            printValue("DECK NUM: %d\n", deckNum);
            return deckNum;
        }
        else if (dist2Wall <= 35) {
            deckNum = 3;
            printValue("DECK NUM: %d\n", deckNum);
            return deckNum;
        }
        else if (dist2Wall <= 40) {
            deckNum = 2;
            printValue("DECK NUM: %d\n", deckNum);
            return deckNum;
        }
        else if (dist2Wall <= 45) {
            deckNum = 1;
            printValue("DECK NUM: %d\n", deckNum);
            return deckNum;
        }
        else {
            return deckNum;
        }  
    }
    else {
        if (dist2Wall <= 28) {
            deckNum = 6;
            printValue("DECK NUM: %d\n", deckNum);
            return deckNum;
        } 
        else if (dist2Wall <= 32) {
            deckNum = 5;
            printValue("DECK NUM: %d\n", deckNum);
            return deckNum; 
        }
        else if (dist2Wall <= 37) {
            deckNum = 4;
            printValue("DECK NUM: %d\n", deckNum);
            return deckNum; 
        }
        else if (dist2Wall <= 42) {
            deckNum = 3;
            printValue("DECK NUM: %d\n", deckNum);
            return deckNum; 
        }
        else if (dist2Wall <= 47) {
            deckNum = 2;
            printValue("DECK NUM: %d\n", deckNum);
            return deckNum; 
        }
        else if (dist2Wall <= 52) {
            deckNum = 1;
            printValue("DECK NUM: %d\n", deckNum);
            return deckNum;  
        }
        else {
            return deckNum;  
        }  
    }
    
    return 0;
}

PinZoneColor computePinZoneColor (int deckNum) {
    switch (deckNum) {
        case 1:
            return PinZoneColorBlue;
        case 2:
            return PinZoneColorGreen;
        case 3:
            return PinZoneColorRed;
        case 4:
            return PinZoneColorBlue;
        case 5:
            return PinZoneColorGreen;
        case 6:
            return PinZoneColorRed;
        default:
            return PinZoneColorInvalid;
    }
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
