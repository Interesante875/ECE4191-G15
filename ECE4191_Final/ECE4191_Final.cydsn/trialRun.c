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
#include "p_ctrl.h"
#include "analog.h"
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
#include "servo.h"
#include "math.h"
#include "moveoutfrombase.h"
#include "movetorasterstart.h"
#include "returntobase.h"
#include "scanpindeck.h"

#define MOVING_PWM 250
#define DETETC_PWM 200
#define ADJUST_PWM 230

#define TEST_RUN 6
#define ENABLE_BT 0 
// BlueBase, RedBase, YellowBase, GreenBase
StartingBase base_color;
Color requiredColor;

void test_run() {
    CyDelay(1000);
    #if TEST_RUN == 1 // Shooter
        shoot();
    #elif TEST_RUN == 2
        
        GripperHand_Open();
        GripperArm_Extend();
        GripperHand_GripPuck();
        GripperArm_Retract();
        
        shutdown_Gripper();
        
    #elif TEST_RUN == 3
        
        GripperHand_Open();
        GripperArm_Extend();
        GripperHand_GripPuck();
        GripperArm_Retract();
        GripperHand_Open();
        GripperArm_Hurl();
        GripperArm_SmallExtend();
        
        shutdown_Gripper();
        
    #elif TEST_RUN == 4
        turnOn_FlickerLock();
        for(;;) {
            FlickerLock_SetCompare(980);
            CyDelay(1000);
            FlickerLock_SetCompare(870);
            CyDelay(1000);
        }
    #elif TEST_RUN == 5
        initializeSharpIR(0);
        
        double dist;
        while(1){
            for (int i = 0; i < 10; i++) {
//                dist = SharpIR_ReadDistance();
//                printValue("(%d): %.2lf\n", 0, dist);
                readSharpIR();
                CyDelay(500);
            }
//            selectSharpIR(1);
//            for (int i = 0; i < 10; i++) {
//                dist = SharpIR_ReadDistance();
//                printValue("(%d): %.2lf\n", 1, dist);
//                CyDelay(500);
//            }
        }
    #elif TEST_RUN == 6
        wheel_move_by_metrics(Forward, 250, 0.5);
        wheel_move_by_metrics(Backward, 250, 0.5);
//        wheel_move_by_metrics(Left, 250, 90);
        wheel_move_by_metrics(Left, 230, 90);
        wheel_move_by_metrics(Right, 230, 90);
//        wheel_move_by_metrics(Right, 250, 90);
//        wheel_move_by_metrics(Right, 250, 90);
    #elif TEST_RUN == 7
        uniturningAlignment(230, FrontAlign);
        
    #elif TEST_RUN == 8
        for(;;) {
            read_U();
            print_U();
            
        }
        
    #elif TEST_RUN == 9
        bt_moving_out_of_base();
        bt_move_to_raster_start();
//        bt_return_to_base();
//        bt_scan_pin_deck();
    #else
        // moveOutofBaseHighLevel(250, 0.25);
//        detectWhereIsThePinZone(210);
        runLevelOne_v2();
    #endif
    
}


void initializeRobot() {
    base_color = BlueBase;
    
    initializePosition(base_color);
    initializeBluetooth();
    
    #if !TEST_RUN && ENABLE_BT
        waitingHandshake();
    #endif
    InitalizeUltrasonicSensor();
}

void moveOutofBaseHighLevel(uint8 pwm, double dist_in_metre) {
    wheel_move_by_metrics(Forward, pwm, dist_in_metre);
}


void detectWhereIsThePinZone(uint8 detectPWM) {

    
    if (base_color == YellowBase || base_color == BlueBase) {
        wheel_move_by_metrics(Right, 220, 90);
        CyDelay(300);
        read_U();
        printValue("START SEACHING DISTANCE TO WALL\n");
        print_U();
        // angle_correction(ADJUST_PWM, RFU, RBU);
        
        bool notSeen = true;

        while (notSeen) {
            
            wheel_move(Backward, 195);
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
//                printValue("RFU: %.2lf RBU: %.2lf DETECTED: %d\n", RFU, RBU, notSeen);
//                if ((RBU <= 15))
//                    notSeen = false;
                
                if (!notSeen) break;
            }
            
            wheel_move(StopMotion, 200);
            
            read_U();
            CyDelay(500);
            // angle_correction(ADJUST_PWM, BLU, BRU);
            read_U();
            
            if (!notSeen) {

                printValue("NOW SEEN: DISTANCE TO WALL\n");
                printValue("BLU: %.2lf BRU: %.2lf DETECTED: %d\n", BLU, BRU);
//                
//                print_U();
                // DETERMINE ZONE
                double distanceToWall = (BLU + BRU) / 2;
                if (distanceToWall <= 28) {
                    levelThreePinDeckNum = 6;
                    levelThreeZoneColor = PinZoneColorRed;
                    printValue("ZONE RED: %d\n", levelThreePinDeckNum);
                } 
                else if (distanceToWall <= 32) {
                    levelThreePinDeckNum = 5;
                    levelThreeZoneColor = PinZoneColorGreen;
                    printValue("ZONE GREEN: %d\n", levelThreePinDeckNum);
                    
                }
                else if (distanceToWall <= 37) {
                    levelThreePinDeckNum = 4;
                    levelThreeZoneColor = PinZoneColorBlue;
                    printValue("ZONE BLUE: %d\n", levelThreePinDeckNum);
                    
                }
                else if (distanceToWall <= 42) {
                    levelThreePinDeckNum = 3;
                    levelThreeZoneColor = PinZoneColorRed;
                    printValue("ZONE RED: %d\n", levelThreePinDeckNum);
                    
                }
                else if (distanceToWall <= 47) {
                    levelThreePinDeckNum = 2;
                    levelThreeZoneColor = PinZoneColorGreen;
                    printValue("ZONE GREEN: %d\n", levelThreePinDeckNum);
                    
                }
                else if (distanceToWall <= 52) {
                    levelThreePinDeckNum = 1;
                    levelThreeZoneColor = PinZoneColorBlue;
                    printValue("ZONE BLUE: %d\n", levelThreePinDeckNum);
                    
                }
                // THEN RETURN
                return;
            } 
            
            read_U();
            printValue("Forward DISTANCE TO WALL\n");
            print_U();
            // angle_correction(ADJUST_PWM, FLU, FRU);
            
            arenaWallNotTooFar = (BLU <= 50) && (BRU <= 50);
            
            wheel_move(Forward, 195);
            
            while (arenaWallNotTooFar) {
                read_U();
                arenaWallNotTooFar = (BLU <= 50) && (BRU <= 50);
                if (!(fabs(RFU - RBU) <= 10))
                    notSeen = RFU < RBU;
                
//                if ((RBU <= 15))
//                    notSeen = false;
                
                if (!notSeen) break;
            }
            
            wheel_move(StopMotion, 200);
            
            CyDelay(300);
            read_U();
            // angle_correction(ADJUST_PWM, RFU, RBU);
            read_U();
            if (!notSeen) {

                CyDelay(200);

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
                // angle_correction(ADJUST_PWM, BLU, BRU);
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
                // angle_correction(ADJUST_PWM, BLU, BRU);
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

void runLevelOne_v2() {
    
    if (zoneColor == PinZoneColorBlue) {
        requiredColor = BlueColor;
    } else if (zoneColor == PinZoneColorRed) {
        requiredColor = RedColor;
    } else if (zoneColor == PinZoneColorGreen) {
        requiredColor = GreenColor;
    }
    
    int facingRight;
    
    wheel_move_by_metrics(Forward, 250, 0.25);
    
    if (base_color == YellowBase || base_color == BlueBase) {
        wheel_move_by_metrics(Left, 220, 60);
        wheel_move_by_metrics(Forward, 250, 0.9);
        wheel_move_by_metrics(Right, 220, 135);
        facingRight = 1;
    } else {
        wheel_move_by_metrics(Right, 220, 60);
        wheel_move_by_metrics(Forward, 250, 0.9);
        wheel_move_by_metrics(Left, 220, 135);
        facingRight = 0;
    }

    CyDelay(300);
    read_U();
    bool obstacle_not_met = (FLU >= 22) && (FRU >= 22);
    
    GripperHand_Open();
    GripperArm_Extend();
    CyDelay(100);
    shutdown_Gripper();
    
    int rasterFinish = 0;
    
    while (!rasterFinish) {
        startIR();
        
        wheel_move(Forward, 230);
        
        while (infraredDetectionStatus == Absence && obstacle_not_met) {
            read_U();
            obstacle_not_met = (FLU >= 22) && (FRU >= 22);
        }
        
        wheel_move(StopMotion, 230);
        stopIR();

        if (infraredDetectionStatus == Presence) {
            
            wheel_move_by_metrics(Backward, 200, 0.015);
            ColorDetection_Run(1);
            GripperHand_GripPuck();   
            GripperArm_Retract();
            CyDelay(100);
            // ATTENTION
            if (detectedColor == RedColor) {
                
                shutdown_Gripper();
                rasterFinish = 1;
            } else {
                
                wheel_move_by_metrics_with_gyro(Right, 220, 20);
                GripperArm_Extend();
                GripperHand_Open();
                CyDelay(100);
                GripperArm_Retract();
                GripperHand_GripPuck();
                wheel_move_by_metrics_with_gyro(Left, 220, 20);
                CyDelay(100);
                shutdown_Gripper();
            }
            
        } else if (!obstacle_not_met) {
            GripperArm_Retract();
            GripperHand_GripPuck();
            CyDelay(100);
            shutdown_Gripper();
            // raster it
            
            if (facingRight) {
                wheel_move_by_metrics(Left, 230, 90);
                wheel_move_by_metrics(Forward, 230, 0.1);
                wheel_move_by_metrics(Left, 230, 90);
            } else {
                wheel_move_by_metrics(Right, 230, 90);
                wheel_move_by_metrics(Forward, 230, 0.1);
                wheel_move_by_metrics(Right, 230, 90);
            }
            
            facingRight = !facingRight;
            
            CyDelay(100);
            read_U();
            obstacle_not_met = true;
        }
        
    }
    
    wheel_move(StopMotion, 250);

    read_U();
    
    if (facingRight && (base_color == YellowBase || base_color == BlueBase)) {
        obstacle_not_met = (BLU >= 10) && (BRU >= 10);
        
        wheel_move(Backward, 250);
        
        while (obstacle_not_met) {
            read_U();
            obstacle_not_met = (BLU >= 10) && (BRU >= 10);
        }
        
        wheel_move(StopMotion, 250);
        wheel_move_by_metrics(Right, 250, 90);
        
        read_U();
        
        obstacle_not_met = (FLU >= 28) && (FRU >= 28);
        
        wheel_move(Forward, 220);
        
        while (obstacle_not_met) {
            read_U();
            obstacle_not_met = (FLU >= 28) && (FRU >= 28);
        }
        
        wheel_move(StopMotion, 220);
        wheel_move_by_metrics(Left, 250, 90);
    }
    else if (facingRight && (base_color == RedBase || base_color == GreenBase)) {
        obstacle_not_met = (FLU >= 10) && (FRU >= 10);
        
        wheel_move(Forward, 250);
        
        while (obstacle_not_met) {
            read_U();
            obstacle_not_met = (FLU >= 10) && (FRU >= 10);
        }
        
        wheel_move(StopMotion, 250);
        wheel_move_by_metrics(Right, 250, 90);
        
        read_U();
        
        obstacle_not_met = (FLU >= 28) && (FRU >= 28);
        
        wheel_move(Forward, 220);
        
        while (obstacle_not_met) {
            read_U();
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
            read_U();
            obstacle_not_met = (FLU >= 10) && (FRU >= 10);
        }
        
        wheel_move(StopMotion, 250);
        wheel_move_by_metrics_with_gyro(Left, 250, 90);
        
        read_U();
        
        obstacle_not_met = (FLU >= 28) && (FRU >= 28);
        
        wheel_move(Forward, 220);
        
        while (obstacle_not_met) {
            read_U();
            obstacle_not_met = (FLU >= 28) && (FRU >= 28);
        }
        
        wheel_move(StopMotion, 220);
        wheel_move_by_metrics(Left, 250, 90);
        facingRight = !facingRight;
    }
    else if (!facingRight && (base_color == RedBase || base_color == GreenBase)) {
        obstacle_not_met = (BLU >= 10) && (BRU >= 10);
        
        wheel_move(Backward, 250);
        
        while (obstacle_not_met) {
            read_U();
            obstacle_not_met = (BLU >= 10) && (BRU >= 10);
        }
        
        wheel_move(StopMotion, 250);
        wheel_move_by_metrics(Left, 250, 90);
        
        read_U();
        
        obstacle_not_met = (FLU >= 28) && (FRU >= 28);
        
        wheel_move(Forward, 220);
        
        while (obstacle_not_met) {
            read_U();
            obstacle_not_met = (FLU >= 28) && (FRU >= 28);
        }
        
        wheel_move(StopMotion, 220);
        wheel_move_by_metrics(Right, 250, 90);
    }
    
    
    double FU, BU;
    
    read_U();
    
    wheel_move(Forward, 210);
    bool notSeen = true;
    bool obstacleNotMet = true;
    
    while (notSeen) {
        if (facingRight) {
            read_U();
        }
        else {
            read_U();
        }
        
        if (!(fabs(FU - BU) <= 10))
            notSeen = FU < BU;
        
    }
    
    wheel_move(StopMotion, 210);
    
//    if (facingRight) {
//        wheel_move_by_metrics(Right, 250, 90);
//        FU = UltrasonicSensor_ReadDistanceData(3);
//    } else {
//        wheel_move_by_metrics(Left, 250, 90);
//        FU = UltrasonicSensor_ReadDistanceData(2);
//    }
//    
//    if (FU - 25 > 0) {
//        wheel_move_by_metrics(Forward, 210, (FU-25/100));   
//    }
//    
//    GripperArm_Extend();
//    GripperHand_Open();
//    GripperArm_Retract();
//    GripperHand_GripPuck();
//    
//    FlickerLock_Lock();
//    CyDelay(100);
//    FlickerRecoil_Load();
//    CyDelay(200);
//    FlickerRecoil_Unload();
//    CyDelay(200);
//    
//    wheel_move_by_metrics(Forward, 210, 0.065);
//    FlickerLock_Unlock();
//    CyDelay(200);
//    FlickerLock_Lock();
//    
//    if (facingRight) {
//        wheel_move_by_metrics_with_gyro(Left, 250, 90);
//    } else {
//        wheel_move_by_metrics_with_gyro(Right, 250, 90);  
//    }
//    
//    BLU = UltrasonicSensor_ReadDistanceData(4);
//    BRU = UltrasonicSensor_ReadDistanceData(5);
//    bool distance_not_met = (BLU <= 70) && (BRU <= 70);
//    
//    wheel_move(Forward, 250);
//    
//    while (distance_not_met) {
//        BLU = UltrasonicSensor_ReadDistanceData(4);
//        BRU = UltrasonicSensor_ReadDistanceData(5);
//        distance_not_met = (BLU <= 70) && (BRU <= 70);
//    }
//    
//    wheel_move(StopMotion, 250);
//    
//    if (facingRight) {
//        wheel_move_by_metrics_with_gyro(Left, 250, 90);
//    } else {
//        wheel_move_by_metrics_with_gyro(Right, 250, 90);  
//    }
//    
//    BLU = UltrasonicSensor_ReadDistanceData(4);
//    BRU = UltrasonicSensor_ReadDistanceData(5);
//    
//    obstacle_not_met = (BLU >= 5) && (BRU >= 5);
//        
//    wheel_move(Backward, 250);
//        
//    while (obstacle_not_met) {
//        BLU = UltrasonicSensor_ReadDistanceData(4);
//        BRU = UltrasonicSensor_ReadDistanceData(5);
//        obstacle_not_met = (BLU >= 5) && (BRU >= 5);
//    }
//    
//    
    
}

void runLevelOne() {
    
    if (zoneColor == PinZoneColorBlue) {
        requiredColor = BlueColor;
    } else if (zoneColor == PinZoneColorRed) {
        requiredColor = RedColor;
    } else if (zoneColor == PinZoneColorGreen) {
        requiredColor = GreenColor;
    }
    
    int facingRight;
    
    wheel_move_by_metrics(Forward, 250, 0.25);
    
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
