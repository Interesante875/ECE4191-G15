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

#include "math.h"
#define MOVING_PWM 250
#define DETETC_PWM 200
#define ADJUST_PWM 230
// BlueBase, RedBase, YellowBase, GreenBase
StartingBase base_color;
Color requiredColor;

void test_run() {
    initializeRobot();
    moveOutofBaseHighLevel(250, 0.25);
    detectWhereIsThePinZone(210);
    
}

void initializeRobot() {
    base_color = YellowBase;
    
    initializePosition(base_color);
    initializeBluetooth();
    InitalizeUltrasonicSensor();

    waitingHandshake();
    
}

void moveOutofBaseHighLevel(uint8 pwm, double dist_in_metre) {
    wheel_move_by_metrics(Forward, pwm, dist_in_metre);
}


void detectWhereIsThePinZone(uint8 detectPWM) {
    
    read_U();
    
    print_U();
    
    if (base_color == YellowBase || base_color == BlueBase) {
        wheel_move_by_metrics(Right, 240, 90);
        CyDelay(300);
        read_U();
        // printValue("START SEACHING DISTANCE TO WALL\n");
        // print_U();
        angle_correction(ADJUST_PWM, RFU, RBU);
        
        bool notSeen = true;

        while (notSeen) {
            
            wheel_move(Backward, detectPWM);
            bool arenaWallNotMet = (BLU >= 14) && (BRU >= 14);
            bool arenaWallNotTooFar = (BLU <= 35) && (BRU <= 35);
            
            while (arenaWallNotMet) {
                read_U();
//                print_U();
                arenaWallNotMet = (BLU >= 14) && (BRU >= 14);
                if (!(fabs(RFU - RBU) <= 10) && arenaWallNotMet)
                {
                    notSeen = RFU < RBU;
                    
                }
                printValue("RFU: %.2lf RBU: %.2lf DETECTED: %d\n", RFU, RBU, notSeen);
//                if ((RBU <= 15))
//                    notSeen = false;
                
                if (!notSeen) break;
            }
            
            wheel_move(StopMotion, 200);
            
            if (!notSeen) {
                read_U();
                angle_correction(ADJUST_PWM, BLU, BRU);
                CyDelay(500);
                read_U();
//                printValue("NOW SEEN: DISTANCE TO WALL\n");
//                print_U();
                // DETERMINE ZONE
                double distanceToWall = (BLU + BRU) / 2;
                if (distanceToWall <= 14) {
                    levelThreePinDeckNum = 6;
                    levelThreeZoneColor = PinZoneColorRed;
                    printValue("ZONE: %d\n", levelThreeZoneColor);
                } 
                else if (distanceToWall <= 19) {
                    levelThreePinDeckNum = 5;
                    levelThreeZoneColor = PinZoneColorGreen;
                    printValue("ZONE: %d\n", levelThreeZoneColor);
                    
                }
                else if (distanceToWall <= 24) {
                    levelThreePinDeckNum = 4;
                    levelThreeZoneColor = PinZoneColorBlue;
                    printValue("ZONE: %d\n", levelThreeZoneColor);
                    
                }
                else if (distanceToWall <= 29) {
                    levelThreePinDeckNum = 3;
                    levelThreeZoneColor = PinZoneColorRed;
                    printValue("ZONE: %d\n", levelThreeZoneColor);
                    
                }
                else if (distanceToWall <= 34) {
                    levelThreePinDeckNum = 2;
                    levelThreeZoneColor = PinZoneColorRed;
                    printValue("ZONE: %d\n", levelThreeZoneColor);
                    
                }
                else if (distanceToWall <= 39) {
                    levelThreePinDeckNum = 1;
                    levelThreeZoneColor = PinZoneColorBlue;
                    printValue("ZONE: %d\n", levelThreeZoneColor);
                    
                }
                // THEN RETURN
                return;
            } 
            
            read_U();
            printValue("Forward DISTANCE TO WALL\n");
            print_U();
            // angle_correction(ADJUST_PWM, FLU, FRU);
            
            arenaWallNotTooFar = (BLU <= 35) && (BRU <= 35);
            
            wheel_move(Forward, detectPWM);
            
            while (arenaWallNotTooFar) {
                read_U();
                arenaWallNotTooFar = (BLU <= 35) && (BRU <= 35);
                if (!(fabs(RFU - RBU) <= 10))
                    notSeen = RFU < RBU;
                
//                if ((RBU <= 15))
//                    notSeen = false;
                
                if (!notSeen) break;
            }
            
            wheel_move(StopMotion, 200);
            
            if (!notSeen) {
                read_U();
                angle_correction(ADJUST_PWM, BLU, BRU);
                read_U();
//                printValue("DISTANCE TO WALL\n");
//                print_U();
                CyDelay(200);
                // DETERMINE ZONE
                double distanceToWall = (BLU + BRU) / 2;
                if (distanceToWall <= 14) {
                    levelThreePinDeckNum = 6;
                    levelThreeZoneColor = PinZoneColorRed;
                    printValue("ZONE: %d\n", levelThreeZoneColor);
                } 
                else if (distanceToWall <= 19) {
                    levelThreePinDeckNum = 5;
                    levelThreeZoneColor = PinZoneColorGreen;
                    printValue("ZONE: %d\n", levelThreeZoneColor);
                    
                }
                else if (distanceToWall <= 24) {
                    levelThreePinDeckNum = 4;
                    levelThreeZoneColor = PinZoneColorBlue;
                    printValue("ZONE: %d\n", levelThreeZoneColor);
                    
                }
                else if (distanceToWall <= 29) {
                    levelThreePinDeckNum = 3;
                    levelThreeZoneColor = PinZoneColorRed;
                    printValue("ZONE: %d\n", levelThreeZoneColor);
                    
                }
                else if (distanceToWall <= 34) {
                    levelThreePinDeckNum = 2;
                    levelThreeZoneColor = PinZoneColorRed;
                    printValue("ZONE: %d\n", levelThreeZoneColor);
                    
                }
                else if (distanceToWall <= 39) {
                    levelThreePinDeckNum = 1;
                    levelThreeZoneColor = PinZoneColorBlue;
                    printValue("ZONE: %d\n", levelThreeZoneColor);
                    
                }
                // THEN RETURN
                return;
            } 
            
        }
         
    }
    else {
        wheel_move_by_metrics(Left, detectPWM, 90);
        CyDelay(120);
        read_U();
        printValue("DISTANCE TO WALL\n");
        print_U();
        // angle_correction(ADJUST_PWM, RFU, RBU);
        
        bool notSeen = true;

        while (notSeen) {
            
            wheel_move(Backward, detectPWM);
            bool arenaWallNotMet = (BLU >= 14) && (BRU >= 14);
            bool arenaWallNotTooFar = (BLU <= 35) && (BRU <= 35);
            
            while (arenaWallNotMet) {
                read_U();
                arenaWallNotMet = (BLU >= 14) && (BRU >= 14);
                if (!(fabs(LFU - LBU) <= 10))
                    notSeen = LFU < LBU;
                //printValue("RBU: %.2lf\n", RBU);
//                if ((RBU <= 15))
//                    notSeen = false;
                
                if (!notSeen) break;
            }
            
            wheel_move(StopMotion, 200);
            
            if (!notSeen) {
                read_U();
                angle_correction(ADJUST_PWM, BLU, BRU);
                read_U();
                printValue("DISTANCE TO WALL\n");
                print_U();
                // DETERMINE ZONE
                double distanceToWall = (BLU + BRU) / 2;
                if (distanceToWall <= 14) {
                    levelThreePinDeckNum = 6;
                    levelThreeZoneColor = PinZoneColorRed;
                    printValue("ZONE: %d\n", levelThreeZoneColor);
                } 
                else if (distanceToWall <= 19) {
                    levelThreePinDeckNum = 5;
                    levelThreeZoneColor = PinZoneColorGreen;
                    printValue("ZONE: %d\n", levelThreeZoneColor);
                    
                }
                else if (distanceToWall <= 24) {
                    levelThreePinDeckNum = 4;
                    levelThreeZoneColor = PinZoneColorBlue;
                    printValue("ZONE: %d\n", levelThreeZoneColor);
                    
                }
                else if (distanceToWall <= 29) {
                    levelThreePinDeckNum = 3;
                    levelThreeZoneColor = PinZoneColorRed;
                    printValue("ZONE: %d\n", levelThreeZoneColor);
                    
                }
                else if (distanceToWall <= 34) {
                    levelThreePinDeckNum = 2;
                    levelThreeZoneColor = PinZoneColorRed;
                    printValue("ZONE: %d\n", levelThreeZoneColor);
                    
                }
                else if (distanceToWall <= 39) {
                    levelThreePinDeckNum = 1;
                    levelThreeZoneColor = PinZoneColorBlue;
                    printValue("ZONE: %d\n", levelThreeZoneColor);
                    
                }
                // THEN RETURN
                return;
            } 
            
            read_U();
            printValue("DISTANCE TO WALL\n");
            print_U();
            // angle_correction(ADJUST_PWM, FLU, FRU);
            
            arenaWallNotTooFar = (BLU <= 35) && (BRU <= 35);
            
            while (arenaWallNotTooFar) {
                read_FU();
                read_LU();
                arenaWallNotTooFar = (BLU <= 35) && (BRU <= 35);
                if (!(fabs(LFU - LBU) <= 10))
                    notSeen = LFU < LBU;
                
//                if ((RBU <= 15))
//                    notSeen = false;
                
                if (!notSeen) break;
            }
            
            if (!notSeen) {
                read_U();
                angle_correction(ADJUST_PWM, BLU, BRU);
                read_U();
                printValue("DISTANCE TO WALL\n");
                print_U();
                // DETERMINE ZONE
                double distanceToWall = (BLU + BRU) / 2;
                if (distanceToWall <= 14) {
                    levelThreePinDeckNum = 6;
                    levelThreeZoneColor = PinZoneColorRed;
                    printValue("ZONE: %d\n", levelThreeZoneColor);
                } 
                else if (distanceToWall <= 19) {
                    levelThreePinDeckNum = 5;
                    levelThreeZoneColor = PinZoneColorGreen;
                    printValue("ZONE: %d\n", levelThreeZoneColor);
                    
                }
                else if (distanceToWall <= 24) {
                    levelThreePinDeckNum = 4;
                    levelThreeZoneColor = PinZoneColorBlue;
                    printValue("ZONE: %d\n", levelThreeZoneColor);
                    
                }
                else if (distanceToWall <= 29) {
                    levelThreePinDeckNum = 3;
                    levelThreeZoneColor = PinZoneColorRed;
                    printValue("ZONE: %d\n", levelThreeZoneColor);
                    
                }
                else if (distanceToWall <= 34) {
                    levelThreePinDeckNum = 2;
                    levelThreeZoneColor = PinZoneColorRed;
                    printValue("ZONE: %d\n", levelThreeZoneColor);
                    
                }
                else if (distanceToWall <= 39) {
                    levelThreePinDeckNum = 1;
                    levelThreeZoneColor = PinZoneColorBlue;
                    printValue("ZONE: %d\n", levelThreeZoneColor);
                    
                }
                // THEN RETURN
                return;
            } 
            
        }
         
         
    }
    
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
    
    if (base_color == YellowBase || base_color == BlueBase) {
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
    
    if (facingRight && (base_color == YellowBase || base_color == BlueBase)) {
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
        wheel_move_by_metrics_with_gyro(Left, 250, 90);
    }
    else if (facingRight && (base_color == RedBase || base_color == GreenBase)) {
        obstacle_not_met = (FLU >= 10) && (FRU >= 10);
        
        wheel_move(Forward, 250);
        
        while (obstacle_not_met) {
            FLU = UltrasonicSensor_ReadDistanceData(0);
            FRU = UltrasonicSensor_ReadDistanceData(1);
            obstacle_not_met = (FLU >= 10) && (FRU >= 10);
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
        wheel_move_by_metrics_with_gyro(Right, 250, 90);
        
        facingRight = !facingRight;
    }
    else if (!facingRight && (base_color == YellowBase || base_color == BlueBase)) {
        obstacle_not_met = (FLU >= 10) && (FRU >= 10);
        
        wheel_move(Forward, 250);
        
        while (obstacle_not_met) {
            FLU = UltrasonicSensor_ReadDistanceData(0);
            FRU = UltrasonicSensor_ReadDistanceData(1);
            obstacle_not_met = (FLU >= 10) && (FRU >= 10);
        }
        
        wheel_move(StopMotion, 250);
        wheel_move_by_metrics_with_gyro(Left, 250, 90);
        
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
        wheel_move_by_metrics_with_gyro(Left, 250, 90);
        facingRight = !facingRight;
    }
    else if (!facingRight && (base_color == RedBase || base_color == GreenBase)) {
        obstacle_not_met = (BLU >= 10) && (BRU >= 10);
        
        wheel_move(Backward, 250);
        
        while (obstacle_not_met) {
            BLU = UltrasonicSensor_ReadDistanceData(4);
            BRU = UltrasonicSensor_ReadDistanceData(5);
            obstacle_not_met = (BLU >= 10) && (BRU >= 10);
        }
        
        wheel_move(StopMotion, 250);
        wheel_move_by_metrics_with_gyro(Left, 250, 90);
        
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
        wheel_move_by_metrics_with_gyro(Right, 250, 90);
    }
    
    
    double FU, BU;
    
    if (facingRight) {
        FU = UltrasonicSensor_ReadDistanceData(3);
        BU = UltrasonicSensor_ReadDistanceData(7);
    }
    else {
        FU = UltrasonicSensor_ReadDistanceData(2);
        BU = UltrasonicSensor_ReadDistanceData(6);
    }
    
    wheel_move(Forward, 210);
    bool notSeen = true;
    bool obstacleNotMet = true;
    
    while (notSeen) {
        if (facingRight) {
            FU = UltrasonicSensor_ReadDistanceData(3);
            BU = UltrasonicSensor_ReadDistanceData(7);
        }
        else {
            FU = UltrasonicSensor_ReadDistanceData(2);
            BU = UltrasonicSensor_ReadDistanceData(6);
        }
        
        if (!(fabs(FU - BU) <= 10))
            notSeen = FU < BU;
        
    }
    
    wheel_move(StopMotion, 210);
    
    if (facingRight) {
        wheel_move_by_metrics(Right, 250, 90);
        FU = UltrasonicSensor_ReadDistanceData(3);
    } else {
        wheel_move_by_metrics(Left, 250, 90);
        FU = UltrasonicSensor_ReadDistanceData(2);
    }
    
    if (FU - 25 > 0) {
        wheel_move_by_metrics(Forward, 210, (FU-25/100));   
    }
    
    GripperArm_Extend();
    GripperHand_Open();
    GripperArm_Retract();
    GripperHand_GripPuck();
    
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
    
    if (facingRight) {
        wheel_move_by_metrics_with_gyro(Left, 250, 90);
    } else {
        wheel_move_by_metrics_with_gyro(Right, 250, 90);  
    }
    
    BLU = UltrasonicSensor_ReadDistanceData(4);
    BRU = UltrasonicSensor_ReadDistanceData(5);
    bool distance_not_met = (BLU <= 70) && (BRU <= 70);
    
    wheel_move(Forward, 250);
    
    while (distance_not_met) {
        BLU = UltrasonicSensor_ReadDistanceData(4);
        BRU = UltrasonicSensor_ReadDistanceData(5);
        distance_not_met = (BLU <= 70) && (BRU <= 70);
    }
    
    wheel_move(StopMotion, 250);
    
    if (facingRight) {
        wheel_move_by_metrics_with_gyro(Left, 250, 90);
    } else {
        wheel_move_by_metrics_with_gyro(Right, 250, 90);  
    }
    
    BLU = UltrasonicSensor_ReadDistanceData(4);
    BRU = UltrasonicSensor_ReadDistanceData(5);
    
    obstacle_not_met = (BLU >= 5) && (BRU >= 5);
        
    wheel_move(Backward, 250);
        
    while (obstacle_not_met) {
        BLU = UltrasonicSensor_ReadDistanceData(4);
        BRU = UltrasonicSensor_ReadDistanceData(5);
        obstacle_not_met = (BLU >= 5) && (BRU >= 5);
    }
    
}

/* [] END OF FILE */
