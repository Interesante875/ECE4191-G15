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
#include "math.h"


#define MAX_SPEED 250
#define HALF_SPEED 200
#define TURN_SPEED 250
#define FACTOR_SURFACE 1.0


int facingRight;

void run_L3() {

    state_3_0();
    state_3_1();
    state_3_2();
    state_3_3();
    state_3_4();
    state_3_5();  
    
}

void state_3_0() {
    
    STATE_CURR_LEVEL = 3;

    wheel_move_by_metrics(Forward, MAX_SPEED, 0.3);
    
    if (base_color == YellowBase || base_color == BlueBase) {
        wheel_move_by_metrics(Left, TURN_SPEED, 90 * FACTOR_SURFACE);
        uniturningAlignment(TURN_SPEED, LeftAlign); 
        facingRight = 1;
    } else {
        wheel_move_by_metrics(Left, TURN_SPEED, 90 * FACTOR_SURFACE);
        uniturningAlignment(TURN_SPEED, LeftAlign);
        facingRight = 0;
    }

}

void state_3_1() {
    
    read_U();
    
    bool notSeen = true;
    bool arenaWallNotTooFar = true;
    bool arenaWallNotTooClose = false;
    
    // YELLOW AND BLUE BASE
    if (base_color == YellowBase || base_color == BlueBase) {
        
        initializeSharpIR(0);
        
        double curr_ADC_Level = 0;
        double prev_ADC_Level = 0;
        
        while (notSeen) {
            
            read_U();
            arenaWallNotTooClose = (FLU >= 14) || (FRU >= 14);
            
            curr_ADC_Level = SharpIR_ReadDistance();
            prev_ADC_Level = SharpIR_ReadDistance();
            
            wheel_move(Forward, HALF_SPEED);
 
            while (arenaWallNotTooClose) {
                read_U();
                arenaWallNotTooClose = (FLU >= 14) || (FRU >= 14);
                
                curr_ADC_Level = SharpIR_ReadDistance();
                
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
                // COMPUTE ZONE
                
                CyDelay(200);
                
                double distanceToWall = (FLU + FRU) / 2;
                int deck = computeZoneNum(distanceToWall, 1);
                
                if (deck == 0) notSeen = true;
                else {
                    switch (deck) {
                        case 1:
                            levelThreeZoneColor = PinZoneColorBlue;
                            requiredColor_L3 = BlueColor;
                        case 2:
                            levelThreeZoneColor = PinZoneColorGreen;
                            requiredColor_L3 = GreenColor;
                        case 3:
                            levelThreeZoneColor = PinZoneColorRed;
                            requiredColor_L3 = RedColor;
                        case 4:
                            levelThreeZoneColor = PinZoneColorBlue;
                            requiredColor_L3 = BlueColor;
                        case 5:
                            levelThreeZoneColor = PinZoneColorGreen;
                            requiredColor_L3 = GreenColor;
                        case 6:
                            levelThreeZoneColor = PinZoneColorRed;
                            requiredColor_L3 = RedColor;
                        default:
                            levelThreeZoneColor = PinZoneColorInvalid;
                    }
                    
                    return;
                }
            }
            
            CyDelay(200);
            
            uniturningAlignment(TURN_SPEED, FrontAlign);
            
            
            arenaWallNotTooFar = true;
            curr_ADC_Level = SharpIR_ReadDistance();
            prev_ADC_Level = SharpIR_ReadDistance();
            
            wheel_move(Backward, HALF_SPEED);
            
            while (arenaWallNotTooFar) { 
                read_U();
                arenaWallNotTooFar = (FLU <= 49) && (FRU <= 49);
                curr_ADC_Level = SharpIR_ReadDistance();
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
                CyDelay(200);
                
                double distanceToWall = (FLU + FRU) / 2;
                int deck = computeZoneNum(distanceToWall, 1);
                
                if (deck == 0) notSeen = true;
                else {
                    switch (deck) {
                        case 1:
                            levelThreeZoneColor = PinZoneColorBlue;
                            requiredColor_L3 = BlueColor;
                        case 2:
                            levelThreeZoneColor = PinZoneColorGreen;
                            requiredColor_L3 = GreenColor;
                        case 3:
                            levelThreeZoneColor = PinZoneColorRed;
                            requiredColor_L3 = RedColor;
                        case 4:
                            levelThreeZoneColor = PinZoneColorBlue;
                            requiredColor_L3 = BlueColor;
                        case 5:
                            levelThreeZoneColor = PinZoneColorGreen;
                            requiredColor_L3 = GreenColor;
                        case 6:
                            levelThreeZoneColor = PinZoneColorRed;
                            requiredColor_L3 = RedColor;
                        default:
                            levelThreeZoneColor = PinZoneColorInvalid;
                    }
                    
                    return;
                }  
            }  
        }
    }
    
    // RED AND GREEN BASE
    else {
        initializeSharpIR(0);
        
        double curr_ADC_Level = 0;
        double prev_ADC_Level = 0;
        
        while (notSeen) {
            
            read_U();
            arenaWallNotTooClose = (FLU >= 14) || (FRU >= 14);
            
            curr_ADC_Level = SharpIR_ReadDistance();
            prev_ADC_Level = SharpIR_ReadDistance();
            
            wheel_move(Forward, HALF_SPEED);
 
            while (arenaWallNotTooClose) {
                read_U();
                arenaWallNotTooClose = (FLU >= 14) || (FRU >= 14);
                
                curr_ADC_Level = SharpIR_ReadDistance();
                
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
                // COMPUTE ZONE
                
                CyDelay(200);
                
                double distanceToWall = (FLU + FRU) / 2;
                int deck = computeZoneNum(distanceToWall, 1);
                
                if (deck == 0) notSeen = true;
                else {
                    switch (deck) {
                        case 1:
                            levelThreeZoneColor = PinZoneColorBlue;
                            requiredColor_L3 = BlueColor;
                        case 2:
                            levelThreeZoneColor = PinZoneColorGreen;
                            requiredColor_L3 = GreenColor;
                        case 3:
                            levelThreeZoneColor = PinZoneColorRed;
                            requiredColor_L3 = RedColor;
                        case 4:
                            levelThreeZoneColor = PinZoneColorBlue;
                            requiredColor_L3 = BlueColor;
                        case 5:
                            levelThreeZoneColor = PinZoneColorGreen;
                            requiredColor_L3 = GreenColor;
                        case 6:
                            levelThreeZoneColor = PinZoneColorRed;
                            requiredColor_L3 = RedColor;
                        default:
                            levelThreeZoneColor = PinZoneColorInvalid;
                    }
                    
                    return;
                }
            }
            
            CyDelay(200);
            
            uniturningAlignment(TURN_SPEED, FrontAlign);
            
            
            arenaWallNotTooFar = true;
            curr_ADC_Level = SharpIR_ReadDistance();
            prev_ADC_Level = SharpIR_ReadDistance();
            
            wheel_move(Backward, HALF_SPEED);
            
            while (arenaWallNotTooFar) { 
                read_U();
                arenaWallNotTooFar = (FLU <= 49) && (FRU <= 49);
                curr_ADC_Level = SharpIR_ReadDistance();
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
                CyDelay(200);
                
                double distanceToWall = (FLU + FRU) / 2;
                int deck = computeZoneNum(distanceToWall, 1);
                
                if (deck == 0) notSeen = true;
                else {
                    switch (deck) {
                        case 1:
                            levelThreeZoneColor = PinZoneColorBlue;
                            requiredColor_L3 = BlueColor;
                        case 2:
                            levelThreeZoneColor = PinZoneColorGreen;
                            requiredColor_L3 = GreenColor;
                        case 3:
                            levelThreeZoneColor = PinZoneColorRed;
                            requiredColor_L3 = RedColor;
                        case 4:
                            levelThreeZoneColor = PinZoneColorBlue;
                            requiredColor_L3 = BlueColor;
                        case 5:
                            levelThreeZoneColor = PinZoneColorGreen;
                            requiredColor_L3 = GreenColor;
                        case 6:
                            levelThreeZoneColor = PinZoneColorRed;
                            requiredColor_L3 = RedColor;
                        default:
                            levelThreeZoneColor = PinZoneColorInvalid;
                    }
                    
                    return;
                }  
            }  
        }
    }
}


void state_3_2() {
    if (base_color == YellowBase || base_color == BlueBase) {
         
        moveUntilObs(1, MAX_SPEED, 25);
        uniturningAlignment(TURN_SPEED, FrontAlign);       
        wheel_move_by_metrics(Right, TURN_SPEED, 90 * FACTOR_SURFACE);
        uniturningAlignment(TURN_SPEED, LeftAlign);
        wheel_move_by_metrics(Forward, MAX_SPEED, 0.6);
        wheel_move_by_metrics(Right, TURN_SPEED, 90 * FACTOR_SURFACE);
        uniturningAlignment(TURN_SPEED, BackAlign); 
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

    
    read_U();
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
                line_not_finish = (FLU >= 30) || (FRU >= 30); 
                
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
            
//            else {
//                
//                read_U();
//                obstacle_not_met = (BLU >= 50) || (BRU >= 50);
//                
//                if (!obstacle_not_met) {
//                    line_not_finish = 1;
//                    continue;   
//                }
//                
//            }
             
        }
        
        if (!raster_finish) {
            printValue("IN TURNING\n");
            stopIR();
            retractGripper();
            if (facingRight) {
                uniturningAlignment(TURN_SPEED, FrontAlign);
                wheel_move_by_metrics(Left, MAX_SPEED, 90 * FACTOR_SURFACE);  
                uniturningAlignment(TURN_SPEED, RightAlign);
                wheel_move_by_metrics(Forward, MAX_SPEED, 0.15);
                wheel_move_by_metrics(Left, MAX_SPEED, 90 * FACTOR_SURFACE);  
                uniturningAlignment(TURN_SPEED, BackAlign);
            }
            else {
                uniturningAlignment(TURN_SPEED, FrontAlign);
                wheel_move_by_metrics(Right, MAX_SPEED, 90 * FACTOR_SURFACE);
                uniturningAlignment(TURN_SPEED, LeftAlign);
                wheel_move_by_metrics(Forward, MAX_SPEED, 0.15);
                wheel_move_by_metrics(Right, MAX_SPEED, 90 * FACTOR_SURFACE);  
                uniturningAlignment(TURN_SPEED, BackAlign);
            }
        
            facingRight = !facingRight;
            line_not_finish = 1;
        }
    
    }
}

void state_3_4 () {
    
    if (base_color == YellowBase || base_color == BlueBase) {
        if (facingRight) {
            moveUntilObs(0, MAX_SPEED, 30);
            uniturningAlignment(TURN_SPEED, BackAlign);
            wheel_move_by_metrics(Right, TURN_SPEED, 90 * FACTOR_SURFACE);
            uniturningAlignment(TURN_SPEED, RightAlign);
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

/* [] END OF FILE */
