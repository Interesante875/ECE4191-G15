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
#include "LevelTwo.h"
#include "returntobase.h"
#include "moveoutfrombase.h"
#include "math.h"


#define MAX_SPEED 230
#define HALF_SPEED 185
#define TURN_SPEED 200
#define ALIGN_SPEED 190
#define FACTOR_SURFACE 1.05


int facingRight_L2 = 0;


void run_L2() {

    state_2_0();
    state_2_1();
    state_2_2();
    state_2_3();
    state_2_4();
    state_2_5();  
    
}

// Function to turn right and align with the left/right wall depending on the base color.
// This function takes into account the robot's base color and performs the necessary actions.
// It also updates the 'facingRight' variable.
void state_2_0() {

    wheel_move_by_metrics(Forward, MAX_SPEED, 0.3);
   
    if (base_color == YellowBase || base_color == BlueBase) {
        // If the base color is either Yellow or Blue, turn left by 90 degrees at a speed of TURN_SPEED.
        wheel_move_by_metrics(Left, TURN_SPEED, 90 * FACTOR_SURFACE);
        
        // Perform an alignment with the left wall using ALIGN_SPEED and the LeftAlign strategy.
        infiniteTurningAlignment(ALIGN_SPEED, LeftAlign);
        
        moveUntilObs(1, MAX_SPEED, 27);
        infiniteTurningAlignment(ALIGN_SPEED, FrontAlign);  
        wheel_move_by_metrics(Right, TURN_SPEED, 90 * FACTOR_SURFACE);
        infiniteTurningAlignment(ALIGN_SPEED, LeftAlign);  
        wheel_move_by_metrics(Forward, MAX_SPEED, 0.6);
        infiniteTurningAlignment(ALIGN_SPEED, LeftAlign);    
        wheel_move_by_metrics(Right, TURN_SPEED, 90 * FACTOR_SURFACE);
        wheel_move_by_metrics(Backward, MAX_SPEED, 0.075);
        infiniteTurningAlignment(TURN_SPEED, BackAlign);
        
        // Set 'facingRight' to 0 to indicate that the robot is not facing right.
        facingRight_L2 = 1;
    } else {
        
        wheel_move_by_metrics(Right, TURN_SPEED, 90 * FACTOR_SURFACE);
        
        // Perform an alignment with the left wall using ALIGN_SPEED and the LeftAlign strategy.
        infiniteTurningAlignment(ALIGN_SPEED, RightAlign);
        
        moveUntilObs(1, MAX_SPEED, 27);
        infiniteTurningAlignment(ALIGN_SPEED, FrontAlign);  
        wheel_move_by_metrics(Left, TURN_SPEED, 90 * FACTOR_SURFACE);
        infiniteTurningAlignment(ALIGN_SPEED, RightAlign);  
        wheel_move_by_metrics(Forward, MAX_SPEED, 0.6);
        infiniteTurningAlignment(ALIGN_SPEED, RightAlign);    
        wheel_move_by_metrics(Left, TURN_SPEED, 90 * FACTOR_SURFACE);
        wheel_move_by_metrics(Backward, MAX_SPEED, 0.05);
        infiniteTurningAlignment(TURN_SPEED, BackAlign);
        
        // Set 'facingRight' to 1 to indicate that the robot is facing right.
        facingRight_L2 = 0;
    }
}


void state_2_1() {
  
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
                if (detectedColor == requiredColor_L2) {
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
            if (facingRight_L2) {
                
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
        
            facingRight_L2 = !facingRight_L2;
            line_not_finish = 1;
        }
    
    }
}


void state_2_2 () {
    
    printValue("RETURNING TO BASE: %d\n", facingRight_L2);
    
    if (base_color == YellowBase || base_color == BlueBase) {
        if (facingRight_L2) {

            moveUntilObs(0, MAX_SPEED, 27);
            infiniteTurningAlignment(ALIGN_SPEED, BackAlign);
            wheel_move_by_metrics(Forward, TURN_SPEED, 0.07);
            wheel_move_by_metrics(Right, TURN_SPEED, 90 * FACTOR_SURFACE);
            infiniteTurningAlignment(ALIGN_SPEED, RightAlign);
            moveUntilObs(1, TURN_SPEED, 32);
            infiniteTurningAlignment(ALIGN_SPEED, RightAlign);
            wheel_move_by_metrics(Right, TURN_SPEED, 90 * FACTOR_SURFACE);
            infiniteTurningAlignment(ALIGN_SPEED, LeftAlign);
        } else {
            moveUntilObs(1, TURN_SPEED, 34);
            infiniteTurningAlignment(ALIGN_SPEED, FrontAlign);
            wheel_move_by_metrics(Left, TURN_SPEED, 90 * FACTOR_SURFACE);
            infiniteTurningAlignment(ALIGN_SPEED, RightAlign);
            moveUntilObs(1, TURN_SPEED, 32);
            infiniteTurningAlignment(ALIGN_SPEED, RightAlign);
            wheel_move_by_metrics(Right, TURN_SPEED, 90 * FACTOR_SURFACE);
            infiniteTurningAlignment(ALIGN_SPEED, LeftAlign);
            facingRight_L2 = 1;
        }
        
    }
    
    else {
        
        if (facingRight_L2) {
            moveUntilObs(1, MAX_SPEED, 34);
            infiniteTurningAlignment(ALIGN_SPEED, FrontAlign);
            wheel_move_by_metrics(Right, TURN_SPEED, 90 * FACTOR_SURFACE);
            infiniteTurningAlignment(ALIGN_SPEED, LeftAlign);
            moveUntilObs(1, TURN_SPEED, 32);
            infiniteTurningAlignment(ALIGN_SPEED, LeftAlign);
            wheel_move_by_metrics(Right, TURN_SPEED, 90 * FACTOR_SURFACE);
            infiniteTurningAlignment(ALIGN_SPEED, LeftAlign);
            facingRight_L2 = 0;
        } else {
            moveUntilObs(0, MAX_SPEED, 27);
            infiniteTurningAlignment(ALIGN_SPEED, BackAlign);
            wheel_move_by_metrics(Forward, TURN_SPEED, 0.07);
            wheel_move_by_metrics(Left, TURN_SPEED, 90 * FACTOR_SURFACE);
            infiniteTurningAlignment(ALIGN_SPEED, LeftAlign);
            moveUntilObs(1, TURN_SPEED, 32);
            infiniteTurningAlignment(ALIGN_SPEED, LeftAlign);
            wheel_move_by_metrics(Right, TURN_SPEED, 90 * FACTOR_SURFACE);
            infiniteTurningAlignment(ALIGN_SPEED, LeftAlign);
        }
        
    }
    
}

void state_2_3() {
    
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
                arenaWallNotTooFar = (FLU <= 47) && (FRU <= 47);
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
            infiniteTurningAlignment(ALIGN_SPEED, BackAlign);
            arenaWallNotTooClose = true;
            curr_ADC_Level = SharpIR_ReadDistance(0);
            prev_ADC_Level = SharpIR_ReadDistance(0);
            
            wheel_move(Backward, HALF_SPEED);
            
            while (arenaWallNotTooClose) { 
                read_U();
                arenaWallNotTooClose = (BLU >= 15) || (BRU >= 15);
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
            arenaWallNotTooFar = true;
            
            curr_ADC_Level = SharpIR_ReadDistance(0);
            prev_ADC_Level = SharpIR_ReadDistance(0);
            
            wheel_move(Forward, HALF_SPEED);
 
            while (arenaWallNotTooFar) {
                read_U();
                arenaWallNotTooFar = (BLU <= 50) || (BRU <= 50);
                
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

void state_2_4() {
    
    
    wheel_move_by_metrics(Forward, HALF_SPEED, 0.025);
    infiniteTurningAlignment(ALIGN_SPEED, RightAlign);
    wheel_move_by_metrics(Left, TURN_SPEED, 90 * FACTOR_SURFACE);
    infiniteTurningAlignment(ALIGN_SPEED, RightAlign);
    CyDelay(320);
    
    read_U();
    
    double dist = FLU > FRU ? FLU : FRU;
    
    dist = dist - 33.5;
    
    if (dist > 0) {
        wheel_move_by_metrics(Forward, HALF_SPEED, dist/100);
    } else {
        wheel_move_by_metrics(Backward, HALF_SPEED, -1.0*dist/100);
    }
    
    placePuckAtDeck();
    shoot();
        
    
}

void state_2_5() {
    
    double dist;
    
    if (base_color == YellowBase || base_color == BlueBase) {
        infiniteTurningAlignment(ALIGN_SPEED, RightAlign);
        wheel_move_by_metrics(Left, TURN_SPEED, 90 * FACTOR_SURFACE);
        infiniteTurningAlignment(TURN_SPEED, RightAlign);
        read_U();
        dist = 0.8 - (BLU + BRU)/200;
        wheel_move_by_metrics(Forward, MAX_SPEED, dist);
        wheel_move_by_metrics(Left, TURN_SPEED, 90 * FACTOR_SURFACE);
        infiniteTurningAlignment(ALIGN_SPEED, BackAlign);
        read_U();
        dist = (BLU + BRU)/200;
        wheel_move_by_metrics(Backward, MAX_SPEED, dist - 0.025);
    } 
    else {
        wheel_move_by_metrics(Right, TURN_SPEED, 90 * FACTOR_SURFACE);
        infiniteTurningAlignment(TURN_SPEED, LeftAlign);
        read_U();
        dist = 0.8 - (BLU + BRU)/200;
        wheel_move_by_metrics(Forward, MAX_SPEED, dist);
        wheel_move_by_metrics(Right, TURN_SPEED, 90 * FACTOR_SURFACE);
        infiniteTurningAlignment(ALIGN_SPEED, BackAlign);
        read_U();
        dist = (BLU + BRU)/200;
        wheel_move_by_metrics(Backward, MAX_SPEED, dist - 0.025);
    }


}
/* [] END OF FILE */
