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
#include "LevelOne.h"
#include "math.h"


#define MAX_SPEED 245
#define HALF_SPEED 210
#define TURN_SPEED 225
#define FACTOR_SURFACE 1.05


int state = 0;
int facingRight = 0;


void run_L1() {

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

void state_1_0() {

    wheel_move_by_metrics(Forward, MAX_SPEED, 0.3);
    
    if (base_color == YellowBase || base_color == BlueBase) {
        wheel_move_by_metrics(Left, TURN_SPEED, 90 * FACTOR_SURFACE);
        uniturningAlignment(TURN_SPEED, LeftAlign); 
        moveUntilObs(1, MAX_SPEED, 25);
        uniturningAlignment(TURN_SPEED, FrontAlign);       
        wheel_move_by_metrics(Right, TURN_SPEED, 90 * FACTOR_SURFACE);
        uniturningAlignment(TURN_SPEED, LeftAlign);
        wheel_move_by_metrics(Forward, MAX_SPEED, 0.6);
        wheel_move_by_metrics(Right, TURN_SPEED, 90 * FACTOR_SURFACE);
        wheel_move_by_metrics(Backward, MAX_SPEED, 0.15);
        uniturningAlignment(TURN_SPEED, BackAlign); 
        facingRight = 1;
    } else {
        wheel_move_by_metrics(Left, TURN_SPEED, 90 * FACTOR_SURFACE);
        uniturningAlignment(TURN_SPEED, LeftAlign);
        moveUntilObs(0, MAX_SPEED, 22);
        uniturningAlignment(TURN_SPEED, BackAlign);
        wheel_move_by_metrics(Right, TURN_SPEED, 90 * FACTOR_SURFACE);
        uniturningAlignment(TURN_SPEED, RightAlign); 
        wheel_move_by_metrics(Forward, MAX_SPEED, 0.6);
        wheel_move_by_metrics(Left, TURN_SPEED, 90 * FACTOR_SURFACE);
        wheel_move_by_metrics(Backward, MAX_SPEED, 0.15);
        uniturningAlignment(TURN_SPEED, BackAlign);
        facingRight = 0;
    }

}


void state_1_1() {

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
                line_not_finish = (FLU >= 34) || (FRU >= 34); 
                
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
            
            else {
//              
                
                int period = 0;
                bool isWall = false;
                int obstacleCount = 0;  // Track the number of consecutive obstacles
                obstacle_not_met = (FLU >= 50) && (FRU >= 50);
                while (period++<=10) {
                    read_U();
                    obstacle_not_met = (FLU >= 50) || (FRU >= 50);
                    if (obstacle_not_met) {
                        line_not_finish = 1;
                        obstacleCount = 0;  // Reset obstacle count if no obstacle
                    } else {
                        isWall = true;  
                        obstacleCount++;  // Increment obstacle count
                        printValue("STILL DECIDING? %d\n", obstacleCount);
                    }
                    if (!isWall) {
                        CyDelay(100);
                    } else if (obstacleCount > 5) {
                        printValue("IT IS A WALL\n");
                        line_not_finish = 0;
                        break;
                    }    
                }
//                
            }
             
        }
        
        if (!raster_finish) {
            printValue("IN TURNING\n");
            stopIR();
            retractGripper();
            
            wheel_move_by_metrics(Forward, TURN_SPEED, 0.1);
            if (facingRight) {
                uniturningAlignment(TURN_SPEED, FrontAlign);
                wheel_move_by_metrics(Left, TURN_SPEED, 90 * FACTOR_SURFACE);  
                uniturningAlignment(TURN_SPEED, RightAlign);
                wheel_move_by_metrics(Forward, MAX_SPEED, 0.15);
                uniturningAlignment(TURN_SPEED, RightAlign);
                wheel_move_by_metrics(Left, TURN_SPEED, 90 * FACTOR_SURFACE);  
                wheel_move_by_metrics(Backward, MAX_SPEED, 0.15);
                uniturningAlignment(TURN_SPEED, BackAlign);
            }
            else {
                uniturningAlignment(TURN_SPEED, FrontAlign);
                wheel_move_by_metrics(Right, TURN_SPEED, 90 * FACTOR_SURFACE);
                uniturningAlignment(TURN_SPEED, LeftAlign);
                wheel_move_by_metrics(Forward, MAX_SPEED, 0.15);
                uniturningAlignment(TURN_SPEED, LeftAlign);
                wheel_move_by_metrics(Right, TURN_SPEED, 90 * FACTOR_SURFACE);  
                wheel_move_by_metrics(Backward, MAX_SPEED, 0.15);
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

void state_1_3() {
    
    read_U();
    
    bool notSeen = true;
    bool arenaWallNotTooFar = true;
    bool arenaWallNotTooClose = false;
    
    initializeSharpIR(0);
    
    if (base_color == YellowBase || base_color == BlueBase) {
        
        double curr_ADC_Level = 0;
        double prev_ADC_Level = 0;
        
        while (notSeen) {
            uniturningAlignment(TURN_SPEED, FrontAlign);
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
            uniturningAlignment(TURN_SPEED, BackAlign);
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

void state_1_4() {
    
    if (base_color == YellowBase || base_color == BlueBase) {
        wheel_move_by_metrics(Forward, HALF_SPEED, 0.01);
        wheel_move_by_metrics(Left, HALF_SPEED, 90 * FACTOR_SURFACE);
//        uniturningAlignment(TURN_SPEED, RightAlign);
//        biturningAlignment();
        CyDelay(320);
        
        read_U();
        
        double dist = FLU > FRU ? FLU : FRU;
        
        dist = dist - 35.5;
        
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
        uniturningAlignment(TURN_SPEED, RightAlign);
        //biturningAlignment();
        CyDelay(320);
        
        read_U();
        
        double dist = FLU > FRU ? FLU : FRU;
        
        dist = dist - 35;;
        
        if (dist > 0) {
            wheel_move_by_metrics(Forward, HALF_SPEED, dist/100);
        } else {
            wheel_move_by_metrics(Backward, HALF_SPEED, -1.0*dist/100);
        }
        
        placePuckAtDeck();
        shoot();
        
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
    else {
        wheel_move_by_metrics(Right, TURN_SPEED, 90 * FACTOR_SURFACE);
        uniturningAlignment(TURN_SPEED, LeftAlign);
        
        read_U();
        
        double dist = 0.8 - (BLU + BRU)/200;
        
        wheel_move_by_metrics(Forward, MAX_SPEED, dist);
        
        wheel_move_by_metrics(Right, TURN_SPEED, 90 * FACTOR_SURFACE);
        
        uniturningAlignment(TURN_SPEED, BackAlign);
        
    }
    
    
    read_U();
    
    double backDist = (BLU + BRU)/200;
    
    wheel_move_by_metrics(Backward, MAX_SPEED, backDist);
    
}


/* [] END OF FILE */
