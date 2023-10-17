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
#include "levels.h"
#include "LevelThree.h"
#include "returntobase.h"
#include "math.h"


#define MAX_SPEED 235
#define HALF_SPEED 185
#define TURN_SPEED 210
#define ALIGN_SPEED 200
#define FACTOR_SURFACE 1.15


int facingRight;

void run_L3() {

    state_3_0();
    state_3_1();
    state_3_2();
    state_3_3();
    state_3_4();
    state_3_5();  
    state_3_6();
    state_3_7();
//    state_3_5_v2();
//    state_3_6_v2();
//    state_3_7();
}

void state_3_0() {

    //firstly make sure that the back is aligned to the back wall
    //move front
    wheel_move_by_metrics(Forward, MAX_SPEED, 0.3);
    infiniteTurningAlignment(ALIGN_SPEED, BackAlign);

    //turn right and align with the left/right wall depending on the base
    if (base_color == YellowBase || base_color == BlueBase) {
        wheel_move_by_metrics(Left, TURN_SPEED, 90 * FACTOR_SURFACE);
        infiniteTurningAlignment(ALIGN_SPEED, LeftAlign);
        facingRight = 0;
    } else {
        wheel_move_by_metrics(Right, TURN_SPEED, 90 * FACTOR_SURFACE);
        infiniteTurningAlignment(ALIGN_SPEED, RightAlign);
        facingRight = 1;
    }

}

void state_3_1() {
    
    // infiniteTurningAlignment(ALIGN_SPEED, LeftAlign);
    
    read_U();
    
    bool notSeen = true;
    bool arenaWallNotTooFar = true;
    bool arenaWallNotTooClose = false;
    
    initializeSharpIR(0);
    
    if (base_color == YellowBase || base_color == BlueBase) {
        
        double curr_ADC_Level = 0;
        double prev_ADC_Level = 0;
            
        while (notSeen) {
            // infiniteTurningAlignment(HALF_SPEED, FrontAlign);
            printValue("IN Loop\n");
            read_U();
            
            arenaWallNotTooClose = true;
            
            curr_ADC_Level = SharpIR_ReadDistance(0);
            prev_ADC_Level = SharpIR_ReadDistance(0);
            
            wheel_move(Forward, HALF_SPEED);
 
            while (arenaWallNotTooClose) {
                read_U();
                arenaWallNotTooClose = (FLU >= 14) || (FRU >= 14);
                
                curr_ADC_Level = SharpIR_ReadDistance(0);
                
                if (curr_ADC_Level - prev_ADC_Level >= 4.5) {
                    notSeen = false;                  
                } else {
                    prev_ADC_Level = curr_ADC_Level;   
                }
                
                if (!notSeen) {
                    printValue("SEEN\n");
                    break;
                }            
            }
            
            wheel_move(StopMotion, HALF_SPEED);
            
            if (!notSeen) {
                break;
            }  
            
            infiniteTurningAlignment(ALIGN_SPEED, FrontAlign);
            
            arenaWallNotTooFar = true;
            curr_ADC_Level = SharpIR_ReadDistance(0);
            prev_ADC_Level = SharpIR_ReadDistance(0);
            
            wheel_move(Backward, HALF_SPEED);
            
            while (arenaWallNotTooFar) { 
                read_U();
                arenaWallNotTooFar = (FLU <= 45) && (FRU <= 45);
                curr_ADC_Level = SharpIR_ReadDistance(0);
                if (curr_ADC_Level - prev_ADC_Level >= 4.5) {
                    notSeen = false;                  
                } else {
                    prev_ADC_Level = curr_ADC_Level;   
                }
                
                if (!notSeen) {
                    printValue("SEEN\n");
                    break;
                }
            }
            
            wheel_move(StopMotion, HALF_SPEED);
                
            if (!notSeen){
                return;   
            }
            
            CyDelay(300);

            
        }
    }
    else {
        
        double curr_ADC_Level = 0;
        double prev_ADC_Level = 0;
        
        while (notSeen) {
            infiniteTurningAlignment(ALIGN_SPEED, FrontAlign);
            arenaWallNotTooFar = true;
            curr_ADC_Level = SharpIR_ReadDistance(1);
            prev_ADC_Level = SharpIR_ReadDistance(1);
            
            wheel_move(Forward, HALF_SPEED);
            
            while (arenaWallNotTooFar) { 
                read_U();
                arenaWallNotTooFar = (FLU <= 40) && (FRU <= 40);
                curr_ADC_Level = SharpIR_ReadDistance(1);
                if (curr_ADC_Level - prev_ADC_Level >= 4.5) {
                    notSeen = false;                  
                } else {
                    prev_ADC_Level = curr_ADC_Level;   
                }
                
                if (!notSeen) {
                    printValue("SEEN\n");
                    break;
                }
            }
            
            wheel_move(StopMotion, HALF_SPEED);
                
            if (!notSeen){
                break;   
            }
            
            CyDelay(300);

            read_U();
            arenaWallNotTooClose = (FLU >= 11) || (FRU >= 11);
            
            curr_ADC_Level = SharpIR_ReadDistance(1);
            prev_ADC_Level = SharpIR_ReadDistance(1);
            
            wheel_move(Forward, ALIGN_SPEED);
 
            while (arenaWallNotTooClose) {
                read_U();
                arenaWallNotTooClose = (FLU >= 11) || (FRU >= 11);
                
                curr_ADC_Level = SharpIR_ReadDistance(1);
                
                if (curr_ADC_Level - prev_ADC_Level >= 4.5) {
                    notSeen = false;                  
                } else {
                    prev_ADC_Level = curr_ADC_Level;   
                }
                
                if (!notSeen) {
                    printValue("SEEN\n");
                    break;
                }            
            }
            
            wheel_move(StopMotion, HALF_SPEED);
            
        }
    }
    
    stopSharpIR();

    
    read_U();
    
    
    double distanceToWall = (FLU + FRU)/2;
    if (distanceToWall <= 20) {
        levelThreePinDeckNum = 6;
        levelThreeZoneColor = PinZoneColorRed;
        requiredColor_L3 = RedColor;
        printValue("ZONE RED: %d\n", levelThreePinDeckNum);
    } 
    else if (distanceToWall <= 25) {
        levelThreePinDeckNum = 5;
        levelThreeZoneColor = PinZoneColorGreen;
        requiredColor_L3 = GreenColor;
        printValue("ZONE GREEN: %d\n", levelThreePinDeckNum);
        
    }
    else if (distanceToWall <= 30) {
        levelThreePinDeckNum = 4;
        levelThreeZoneColor = PinZoneColorBlue;
        requiredColor_L3 = BlueColor;
        printValue("ZONE BLUE: %d\n", levelThreePinDeckNum);
        
    }
    else if (distanceToWall <= 35) {
        levelThreePinDeckNum = 3;
        levelThreeZoneColor = PinZoneColorRed;
        requiredColor_L3 = RedColor;
        printValue("ZONE RED: %d\n", levelThreePinDeckNum);
        
    }
    else if (distanceToWall <= 40) {
        levelThreePinDeckNum = 2;
        levelThreeZoneColor = PinZoneColorGreen;
        requiredColor_L3 = GreenColor;
        printValue("ZONE GREEN: %d\n", levelThreePinDeckNum);
        
    }
    else if (distanceToWall <= 45) {
        levelThreePinDeckNum = 1;
        levelThreeZoneColor = PinZoneColorBlue;
        requiredColor_L3 = BlueColor;
        printValue("ZONE BLUE: %d\n", levelThreePinDeckNum);
        
    }
    
}


void state_3_2() {
    if (base_color == YellowBase || base_color == BlueBase) {
         
        moveUntilObs(1, MAX_SPEED, 25);
        infiniteTurningAlignment(ALIGN_SPEED, FrontAlign);
        wheel_move_by_metrics(Right, TURN_SPEED, 90 * FACTOR_SURFACE);
        infiniteTurningAlignment(ALIGN_SPEED, LeftAlign);
        wheel_move_by_metrics(Forward, MAX_SPEED, 0.7);
        infiniteTurningAlignment(ALIGN_SPEED, LeftAlign);
        wheel_move_by_metrics(Right, TURN_SPEED, 90 * FACTOR_SURFACE);
        wheel_move_by_metrics(Backward, MAX_SPEED, 0.05);
        infiniteTurningAlignment(ALIGN_SPEED, BackAlign);
        facingRight = 1;
    } else {
        
        moveUntilObs(0, MAX_SPEED, 22);
        uniturningAlignment(TURN_SPEED, BackAlign);
        wheel_move_by_metrics(Right, TURN_SPEED, 90 * FACTOR_SURFACE);
        uniturningAlignment(TURN_SPEED, RightAlign); 
        wheel_move_by_metrics(Forward, MAX_SPEED, 0.6);
        wheel_move_by_metrics(Left, TURN_SPEED, 90 * FACTOR_SURFACE);
        uniturningAlignment(TURN_SPEED, BackAlign);
        facingRight = 0;
    }
    
}

void state_3_3() {

    bool obstacle_not_met = true;   
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
                line_not_finish = (FLU >= 32) || (FRU >= 32); 
                
            }
            
            wheel_move(StopMotion, MAX_SPEED);
            
            
            if (infraredDetectionStatus == Presence) {
                stopIR();
                wheel_move_by_metrics(Backward, TURN_SPEED, 0.031);
                ColorDetection_Run(1);
                printValue("DETECTED COLOR: %d\n", detectedColor);
                if (detectedColor == requiredColor_L3) {
                    printValue("OBTAINED WANTED PUCK\n");
                    grabPuckAndHold();
                    line_not_finish = 0;
                    raster_finish = 1;
                } else {
                    printValue("DISCARD\n");
                    grabPuckAndDiscard();
                }
            } 
            
//            else {
////              
//                int period = 0;
//                bool isWall = false;
//                int obstacleCount = 0;  // Track the number of consecutive obstacles
//                obstacle_not_met = (FLU >= 34) && (FRU >= 34);
//                while (period++<=10) {
//                    read_U();
//                    obstacle_not_met = (FLU >= 34) || (FRU >= 34);
//                    if (obstacle_not_met) {
//                        isWall = false;
//                        line_not_finish = 1;
//                        obstacleCount = 0;  // Reset obstacle count if no obstacle
//                        printValue("MIGHT NOT BE THO %d\n", obstacleCount);
//                    } else {
//                        isWall = true;  
//                        obstacleCount++;  // Increment obstacle count
//                        printValue("STILL DECIDING? %d\n", obstacleCount);
//                        CyDelay(100);
//                    }
//                    
//                    if (!isWall) {
//                        CyDelay(100);
//                        printValue("NOT SURE IF IT IS WALL!\n");
//                    } else if (obstacleCount > 9) {
//                        printValue("IT IS A WALL\n");
//                        line_not_finish = 0;
//                        break;
//                    }    
//                }
////                
//            }
             
        }
        
        if (!raster_finish) {
            printValue("IN TURNING\n");
            stopIR();
            retractGripper();
            
            wheel_move_by_metrics(Forward, TURN_SPEED, 0.1);
            if (facingRight) {
                
                infiniteTurningAlignment(ALIGN_SPEED, FrontAlign);
                wheel_move_by_metrics(Left, TURN_SPEED, 90 * FACTOR_SURFACE);  
                infiniteTurningAlignment(ALIGN_SPEED, RightAlign);
                wheel_move_by_metrics(Forward, MAX_SPEED, 0.22);
                infiniteTurningAlignment(ALIGN_SPEED, RightAlign);
                wheel_move_by_metrics(Left, TURN_SPEED, 90 * FACTOR_SURFACE);  
                wheel_move_by_metrics(Backward, MAX_SPEED, 0.05);
                infiniteTurningAlignment(ALIGN_SPEED, BackAlign);
            }
            else {
                infiniteTurningAlignment(ALIGN_SPEED, FrontAlign);
                wheel_move_by_metrics(Right, TURN_SPEED, 90 * FACTOR_SURFACE);
                infiniteTurningAlignment(ALIGN_SPEED, LeftAlign);
                wheel_move_by_metrics(Forward, MAX_SPEED, 0.22);
                infiniteTurningAlignment(ALIGN_SPEED, LeftAlign);
                wheel_move_by_metrics(Right, TURN_SPEED, 90 * FACTOR_SURFACE);  
                wheel_move_by_metrics(Backward, MAX_SPEED, 0.05);
                infiniteTurningAlignment(TURN_SPEED, BackAlign);
            }
        
            facingRight = !facingRight;
            line_not_finish = 1;
        }
    
    }
}

void state_3_4 () {
    
    if (base_color == YellowBase || base_color == BlueBase) {
        if (facingRight) {
            moveUntilObs(0, MAX_SPEED, 34);
            
            infiniteTurningAlignment(ALIGN_SPEED, BackAlign);
            wheel_move_by_metrics(Right, TURN_SPEED, 90 * FACTOR_SURFACE);
            infiniteTurningAlignment(ALIGN_SPEED, RightAlign);
            moveUntilObs(1, TURN_SPEED, 32);
            wheel_move_by_metrics(Right, TURN_SPEED, 90 * FACTOR_SURFACE);
            
        } else {
            moveUntilObs(1, MAX_SPEED, 25);
            uniturningAlignment(TURN_SPEED, FrontAlign);
            wheel_move_by_metrics(Left, TURN_SPEED, 90 * FACTOR_SURFACE);
            uniturningAlignment(TURN_SPEED, RightAlign);
            moveUntilObs(1, TURN_SPEED, 32);
            wheel_move_by_metrics(Right, TURN_SPEED, 90 * FACTOR_SURFACE);
            
            facingRight = 1;
        }
        
    }
    
    else {
        
        if (facingRight) {
            moveUntilObs(1, MAX_SPEED, 30);
            uniturningAlignment(TURN_SPEED, FrontAlign);
            wheel_move_by_metrics(Right, TURN_SPEED, 90 * FACTOR_SURFACE);
            uniturningAlignment(TURN_SPEED, LeftAlign);
            moveUntilObs(1, HALF_SPEED, 32);
            wheel_move_by_metrics(Right, TURN_SPEED, 90 * FACTOR_SURFACE);
            
            facingRight = 0;
        } else {
            moveUntilObs(0, MAX_SPEED, 30);
            uniturningAlignment(TURN_SPEED, BackAlign);
            wheel_move_by_metrics(Left, TURN_SPEED, 90 * FACTOR_SURFACE);
            uniturningAlignment(TURN_SPEED, LeftAlign);
            moveUntilObs(1, HALF_SPEED, 32);
            wheel_move_by_metrics(Right, TURN_SPEED, 90 * FACTOR_SURFACE);
        }
        
    }
    
}


void state_3_5_v2() {
    
    read_U();
    
    bool notSeen = true;
    bool arenaWallNotTooFar = true;
    bool arenaWallNotTooClose = false;
    
    initializeSharpIR(0);
    
    
    
    if (base_color == YellowBase || base_color == BlueBase) {
        
        double curr_ADC_Level = 0;
        double prev_ADC_Level = 0;
        
        while (notSeen) {
            infiniteTurningAlignment(ALIGN_SPEED, BackAlign);
            arenaWallNotTooFar = true;
            curr_ADC_Level = SharpIR_ReadDistance(1);
            prev_ADC_Level = SharpIR_ReadDistance(1);
            
            wheel_move(Forward, HALF_SPEED);
            
            while (arenaWallNotTooFar) { 
                read_U();
                arenaWallNotTooFar = (FLU <= 55) && (FRU <= 55);
                curr_ADC_Level = SharpIR_ReadDistance(1);
                if (curr_ADC_Level - prev_ADC_Level >= 4.5) {
                    notSeen = false;                  
                } else {
                    prev_ADC_Level = curr_ADC_Level;   
                }
                
                if (!notSeen) {
                    printValue("SEEN\n");
                    break;
                }
            }
            
            wheel_move(StopMotion, HALF_SPEED);
                
            if (!notSeen){
                return;   
            }
            
            CyDelay(300);

            read_U();
            arenaWallNotTooClose = (FLU >= 14) || (FRU >= 14);
            
            curr_ADC_Level = SharpIR_ReadDistance(1);
            prev_ADC_Level = SharpIR_ReadDistance(1);
            
            wheel_move(Backward, HALF_SPEED);
 
            while (arenaWallNotTooClose) {
                read_U();
                arenaWallNotTooClose = (FLU >= 14) || (FRU >= 14);
                
                curr_ADC_Level = SharpIR_ReadDistance(1);
                
                if (curr_ADC_Level - prev_ADC_Level >= 4.5) {
                    notSeen = false;                  
                } else {
                    prev_ADC_Level = curr_ADC_Level;   
                }
                
                if (!notSeen) {
                    printValue("SEEN\n");
                    break;
                }            
            }
            
            wheel_move(StopMotion, HALF_SPEED);
            
        }
        
    }
    
    else {
        
        
    }
    
}

void state_3_5() {
    
    read_U();
    
    bool notSeen = true;
    bool arenaWallNotTooFar = true;
    bool arenaWallNotTooClose = false;
    
    initializeSharpIR(0);
    
    CyDelay(200);
    
    if (base_color == YellowBase || base_color == BlueBase) {
        
        double curr_ADC_Level = 0;
        double prev_ADC_Level = 0;
        
        while (notSeen) {
            infiniteTurningAlignment(ALIGN_SPEED, FrontAlign);
            arenaWallNotTooFar = true;
            curr_ADC_Level = SharpIR_ReadDistance(0);
            prev_ADC_Level = SharpIR_ReadDistance(0);
            
            wheel_move(Backward, HALF_SPEED);
            
            while (arenaWallNotTooFar) { 
                read_U();
                arenaWallNotTooFar = (FLU <= 55) && (FRU <= 55);
                curr_ADC_Level = SharpIR_ReadDistance(0);
                if (curr_ADC_Level - prev_ADC_Level >= 4.5) {
                    notSeen = false;                  
                } else {
                    prev_ADC_Level = curr_ADC_Level;   
                }
                
                if (!notSeen) {
                    printValue("SEEN\n");
                    break;
                }
            }
            
            wheel_move(StopMotion, HALF_SPEED);
                
            if (!notSeen){
                return;   
            }
            
            CyDelay(300);

            read_U();
            arenaWallNotTooClose = (FLU >= 14) || (FRU >= 14);
            
            curr_ADC_Level = SharpIR_ReadDistance(0);
            prev_ADC_Level = SharpIR_ReadDistance(0);
            
            wheel_move(Forward, HALF_SPEED);
 
            while (arenaWallNotTooClose) {
                read_U();
                arenaWallNotTooClose = (FLU >= 14) || (FRU >= 14);
                
                curr_ADC_Level = SharpIR_ReadDistance(0);
                
                if (curr_ADC_Level - prev_ADC_Level >= 4.5) {
                    notSeen = false;                  
                } else {
                    prev_ADC_Level = curr_ADC_Level;   
                }
                
                if (!notSeen) {
                    printValue("SEEN\n");
                    break;
                }            
            }
            
            wheel_move(StopMotion, HALF_SPEED);
        }
    }
    else {
        
        double curr_ADC_Level = 0;
        double prev_ADC_Level = 0;
        
        while (notSeen) {
            infiniteTurningAlignment(ALIGN_SPEED, BackAlign);;
            arenaWallNotTooFar = true;
            curr_ADC_Level = SharpIR_ReadDistance(0);
            prev_ADC_Level = SharpIR_ReadDistance(0);
            
            wheel_move(Backward, HALF_SPEED);
            
            while (arenaWallNotTooFar) { 
                read_U();
                arenaWallNotTooFar = (FLU <= 50) && (FRU <= 50);
                curr_ADC_Level = SharpIR_ReadDistance(0);
                if (curr_ADC_Level - prev_ADC_Level >= 4.5) {
                    notSeen = false;                  
                } else {
                    prev_ADC_Level = curr_ADC_Level;   
                }
                
                if (!notSeen) {
                    printValue("SEEN\n");
                    break;
                }
            }
            
            wheel_move(StopMotion, HALF_SPEED);
                
            if (!notSeen){
                return;   
            }
            
            CyDelay(300);

            read_U();
            arenaWallNotTooClose = (FLU >= 14) || (FRU >= 14);
            
            curr_ADC_Level = SharpIR_ReadDistance(0);
            prev_ADC_Level = SharpIR_ReadDistance(0);
            
            wheel_move(Forward, HALF_SPEED);
 
            while (arenaWallNotTooClose) {
                read_U();
                arenaWallNotTooClose = (FLU >= 14) || (FRU >= 14);
                
                curr_ADC_Level = SharpIR_ReadDistance(0);
                
                if (curr_ADC_Level - prev_ADC_Level >= 4.5) {
                    notSeen = false;                  
                } else {
                    prev_ADC_Level = curr_ADC_Level;   
                }
                
                if (!notSeen) {
                    printValue("SEEN\n");
                    break;
                }            
            }
            
            wheel_move(StopMotion, HALF_SPEED);
        }
    }
    
    stopSharpIR();
    
}

void state_3_6_v2() {
    if (base_color == YellowBase || base_color == BlueBase) {
        wheel_move_by_metrics(Forward, HALF_SPEED, 0.025);
        wheel_move_by_metrics(Right, TURN_SPEED, 90 * FACTOR_SURFACE);
        infiniteTurningAlignment(ALIGN_SPEED, RightAlign);
        
        CyDelay(320);
        
        read_U();
        
        double dist = FLU > FRU ? FLU : FRU;
        
        dist = dist - 30;
        
        if (dist > 0) {
            wheel_move_by_metrics(Forward, HALF_SPEED, dist/100);
        } else {
            wheel_move_by_metrics(Backward, HALF_SPEED, -1.0*dist/100);
        }
        
        placePuckAtDeck();
        shoot();
    }
    
}

void state_3_6() {
    if (base_color == YellowBase || base_color == BlueBase) {
        wheel_move_by_metrics(Forward, HALF_SPEED, 0.025);
        infiniteTurningAlignment(ALIGN_SPEED, RightAlign);
        wheel_move_by_metrics(Left, TURN_SPEED, 90 * FACTOR_SURFACE);
        infiniteTurningAlignment(ALIGN_SPEED, RightAlign);
//        biturningAlignment();
        CyDelay(320);
        
        read_U();
        
        double dist = FLU > FRU ? FLU : FRU;
        
        dist = dist - 30;
        
        if (dist > 0) {
            wheel_move_by_metrics(Forward, HALF_SPEED, dist/100);
        } else {
            wheel_move_by_metrics(Backward, HALF_SPEED, -1.0*dist/100);
        }
        
        placePuckAtDeck();
        shoot();
        
    } else {
        wheel_move_by_metrics(Forward, HALF_SPEED, 0.035);
        wheel_move_by_metrics(Left, TURN_SPEED, 90 * FACTOR_SURFACE);
        infiniteTurningAlignment(ALIGN_SPEED, RightAlign);
        //biturningAlignment();
        CyDelay(320);
        
        read_U();
        
        double dist = FLU > FRU ? FLU : FRU;
        
        dist = dist - 30;
        
        if (dist > 0) {
            wheel_move_by_metrics(Forward, HALF_SPEED, dist/100);
        } else {
            wheel_move_by_metrics(Backward, HALF_SPEED, -1.0*dist/100);
        }
        
        placePuckAtDeck();
        shoot();
        
    }
}

void state_3_7() {
    if (base_color == YellowBase || base_color == BlueBase) {
        
        wheel_move_by_metrics(Left, TURN_SPEED, 90 * FACTOR_SURFACE);
        
        infiniteTurningAlignment(TURN_SPEED, RightAlign);
        
        read_U();
        
        double dist = 0.8 - (BLU + BRU)/200;
        
        wheel_move_by_metrics(Forward, MAX_SPEED, dist);
        
        wheel_move_by_metrics(Left, TURN_SPEED, 90 * FACTOR_SURFACE);
        
        infiniteTurningAlignment(ALIGN_SPEED, BackAlign);
        
        read_U();
        
        dist = (BLU + BRU)/200;
        
        wheel_move_by_metrics(Backward, MAX_SPEED, dist - 0.025);
        
    }
}
/* [] END OF FILE */
