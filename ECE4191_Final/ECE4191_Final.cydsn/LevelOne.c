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
#include "returntobase.h"
#include "moveoutfrombase.h"
#include "math.h"


#define MAX_SPEED 230
#define HALF_SPEED 185
#define TURN_SPEED 200
#define ALIGN_SPEED 190
#define FACTOR_SURFACE 1.0

int facingRight;


void run_L1() {

    state_1_0();
    state_1_1();
    state_1_2();
    state_1_3();
    state_1_4();
    state_1_5();  
    
}

// Function to turn right and align with the left/right wall depending on the base color.
// This function takes into account the robot's base color and performs the necessary actions.
// It also updates the 'facingRight' variable.
void state_1_0() {

    wheel_move_by_metrics(Forward, MAX_SPEED, 0.3);
   
    if (base_color == YellowBase || base_color == BlueBase) {
        // If the base color is either Yellow or Blue, turn left by 90 degrees at a speed of TURN_SPEED.
        wheel_move_by_metrics(Left, TURN_SPEED, 90 * FACTOR_SURFACE);
        
        // Perform an alignment with the left wall using ALIGN_SPEED and the LeftAlign strategy.
        //infiniteTurningAlignment(ALIGN_SPEED, LeftAlign);
        
        moveUntilObs(1, MAX_SPEED, 27);
        //infiniteTurningAlignment(ALIGN_SPEED, FrontAlign);  
        wheel_move_by_metrics(Right, TURN_SPEED, 90 * FACTOR_SURFACE);
        //infiniteTurningAlignment(ALIGN_SPEED, LeftAlign);  
        wheel_move_by_metrics(Forward, MAX_SPEED, 0.65);
        //infiniteTurningAlignment(ALIGN_SPEED, LeftAlign);    
        wheel_move_by_metrics(Right, TURN_SPEED, 90 * FACTOR_SURFACE);
        wheel_move_by_metrics(Backward, MAX_SPEED, 0.09);
        infiniteTurningAlignment(TURN_SPEED, BackAlign);
        
        // Set 'facingRight' to 0 to indicate that the robot is not facing right.
        facingRight = 1;
    } else {
        
        wheel_move_by_metrics(Right, TURN_SPEED, 90 * FACTOR_SURFACE);
        
        // Perform an alignment with the left wall using ALIGN_SPEED and the LeftAlign strategy.
        //infiniteTurningAlignment(ALIGN_SPEED, RightAlign);
        
        moveUntilObs(1, MAX_SPEED, 27);
        //infiniteTurningAlignment(ALIGN_SPEED, FrontAlign);  
        wheel_move_by_metrics(Left, TURN_SPEED, 90 * FACTOR_SURFACE);
        //infiniteTurningAlignment(ALIGN_SPEED, RightAlign);  
        wheel_move_by_metrics(Forward, MAX_SPEED, 0.65);
        //infiniteTurningAlignment(ALIGN_SPEED, RightAlign);    
        wheel_move_by_metrics(Left, TURN_SPEED, 90 * FACTOR_SURFACE);
        wheel_move_by_metrics(Backward, MAX_SPEED, 0.09);
        infiniteTurningAlignment(TURN_SPEED, BackAlign);
        
        // Set 'facingRight' to 1 to indicate that the robot is facing right.
        facingRight = 0;
    }
}


void state_1_1() {
    
    bool obstacle_not_met = true;   
    int line_not_finish = 1;
    int raster_finish = 0;
    double prev_FLU_FRU = 0.0;
    read_U();

    
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
            prev_FLU_FRU = fmin(FLU, FRU);

            wheel_move(StopMotion, MAX_SPEED);
            
            if (infraredDetectionStatus == Presence) {
                stopIR();
                wheel_move_by_metrics(Backward, ALIGN_SPEED, 0.015);
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

            int wallDetected = false;

            printValue("IN TURNING\n");
            stopIR();
            retractGripper();
            
            if (facingRight){
                wheel_move_by_metrics(Left, TURN_SPEED, 90 * FACTOR_SURFACE);
                CyDelay(160);
                wheel_move_by_metrics(Forward, HALF_SPEED, 0.25);
                read_U();
                CyDelay(160);

                
                wallDetected = (fabs(prev_FLU_FRU - RFU) <= 12);

            }
            else if (!facingRight){
                wheel_move_by_metrics(Right, TURN_SPEED, 90* FACTOR_SURFACE);

                wheel_move_by_metrics(Forward, HALF_SPEED, 0.25);
                read_U();
                CyDelay(160);
                wallDetected = (fabs(prev_FLU_FRU - LFU) <= 12);

            }

            printValue("Wall Detected: %d\n", wallDetected);
        
            if (facingRight) {
                if (wallDetected){
                    printValue("Next row of raster scan\n");
                    // infiniteTurningAlignment(ALIGN_SPEED, RightAlign);
                    wheel_move_by_metrics(Left, TURN_SPEED, 90 * FACTOR_SURFACE); 
                    //infiniteTurningAlignment(ALIGN_SPEED, BackAlign); 
                    wheel_move_by_metrics(Backward, MAX_SPEED, 0.05);
                    infiniteTurningAlignment(ALIGN_SPEED, BackAlign);
                    
                }
                else if (!wallDetected){  
                    printValue("Same line of raster scan");
                    wheel_move_by_metrics(Right, TURN_SPEED, 90 * FACTOR_SURFACE);
                }        
     
            }   
            else if (!facingRight){
                if (wallDetected){
                    
                    printValue("Next row of raster scan\n");
                    //infiniteTurningAlignment(ALIGN_SPEED, LeftAlign);
                    wheel_move_by_metrics(Right, TURN_SPEED, 90 * FACTOR_SURFACE);  
                    //infiniteTurningAlignment(ALIGN_SPEED, BackAlign);
                    wheel_move_by_metrics(Backward, MAX_SPEED, 0.05);
                    infiniteTurningAlignment(TURN_SPEED, BackAlign);
                }
                else if (!wallDetected){

                    wheel_move_by_metrics(Left, TURN_SPEED, 90 * FACTOR_SURFACE);

                } 
                
            }
        
            if (wallDetected){
                facingRight = !facingRight;
            }
            line_not_finish = 1;
        }
    }
}


void state_1_2 () {
    
    printValue("RETURNING TO BASE: %d\n", facingRight);
    double prev_BLU_BRU = 0.0;
    double prev_FLU_FRU = 0.0;
    if (base_color == YellowBase || base_color == BlueBase) {
        if (facingRight) {
            int reachWall = false;
            
            while (!reachWall) {
                wheel_move(Backward, MAX_SPEED);
                int obstacleDetected = false;
                int wallDetected = false;
                while (!obstacleDetected) {
                    read_U();
                    obstacleDetected = (BLU <= 27) || (BRU <= 27); 
                        
                }
                
                wheel_move(StopMotion, MAX_SPEED);
                
                prev_BLU_BRU = fmin(BLU, BRU);
                wheel_move_by_metrics(Right, TURN_SPEED, 90 * FACTOR_SURFACE);
                CyDelay(160);
                wheel_move_by_metrics(Forward, HALF_SPEED, 0.25);
                
                CyDelay(160);
                read_U();
                    
                wallDetected = (fabs(prev_BLU_BRU - RFU) <= 5);
                
                if (wallDetected){
                    moveUntilObs(1, TURN_SPEED, 32);
                    wheel_move_by_metrics(Right, TURN_SPEED, 90 * FACTOR_SURFACE);
                    infiniteTurningAlignment(ALIGN_SPEED, LeftAlign);
                    reachWall = true;
                    facingRight = 1;
                }
                else if (!wallDetected){
                    wheel_move_by_metrics(Left, TURN_SPEED, 90 * FACTOR_SURFACE);
                }
            }
            
        } else {
            
            int reachWall = false;
            
            while (!reachWall) {
                wheel_move(Forward, MAX_SPEED);
                int obstacleDetected = false;
                int wallDetected = false;
                while (!obstacleDetected) {
                    read_U();
                    obstacleDetected = (FLU <= 27) || (FRU <= 27); 
                        
                }
                
                wheel_move(StopMotion, MAX_SPEED);
                
                prev_FLU_FRU = fmin(FLU, FRU);
                wheel_move_by_metrics(Left, TURN_SPEED, 90 * FACTOR_SURFACE);
                CyDelay(160);
                wheel_move_by_metrics(Forward, HALF_SPEED, 0.25);
                read_U();
                CyDelay(160);

                    
                wallDetected = (fabs(prev_FLU_FRU - RFU) <= 12);
                
                if (wallDetected){
                    moveUntilObs(1, TURN_SPEED, 32);
                    wheel_move_by_metrics(Right, TURN_SPEED, 90 * FACTOR_SURFACE);
                    infiniteTurningAlignment(ALIGN_SPEED, LeftAlign);
                    reachWall = true;
                }
                else if (!wallDetected){
                    wheel_move_by_metrics(Right, TURN_SPEED, 90 * FACTOR_SURFACE);
                }
            }
            
            
//            moveUntilObs(1, TURN_SPEED, 34);
//            infiniteTurningAlignment(ALIGN_SPEED, FrontAlign);
//            wheel_move_by_metrics(Left, TURN_SPEED, 90 * FACTOR_SURFACE);
//            //infiniteTurningAlignment(ALIGN_SPEED, RightAlign);
//            moveUntilObs(1, TURN_SPEED, 32);
//            //infiniteTurningAlignment(ALIGN_SPEED, RightAlign);
//            wheel_move_by_metrics(Right, TURN_SPEED, 90 * FACTOR_SURFACE);
//            infiniteTurningAlignment(ALIGN_SPEED, LeftAlign);
//            facingRight = 1;
        }
        
        
        infiniteTurningAlignment(ALIGN_SPEED, FrontAlign);
    }
    
    else {
        
        if (facingRight) {
            int reachWall = false;
            
            while (!reachWall) {
                wheel_move(Forward, MAX_SPEED);
                int obstacleDetected = false;
                int wallDetected = false;
                while (!obstacleDetected) {
                    read_U();
                    obstacleDetected = (FLU <= 23) || (FRU <= 23); 
                        
                }
                
                wheel_move(StopMotion, MAX_SPEED);
                
                prev_FLU_FRU = fmin(FLU, FRU);
                wheel_move_by_metrics(Right, TURN_SPEED, 90 * FACTOR_SURFACE);
                CyDelay(160);
                wheel_move_by_metrics(Forward, HALF_SPEED, 0.25);
                
                CyDelay(160);
                read_U();
                    
                wallDetected = (fabs(prev_FLU_FRU - LFU) <= 12);
                
                if (wallDetected){
                    moveUntilObs(1, TURN_SPEED, 32);
                    wheel_move_by_metrics(Right, TURN_SPEED, 90 * FACTOR_SURFACE);
                    infiniteTurningAlignment(ALIGN_SPEED, LeftAlign);
                    facingRight = 0;
                    reachWall = true;
                }
                else if (!wallDetected){
                    wheel_move_by_metrics(Left, TURN_SPEED, 90 * FACTOR_SURFACE);
                }
            }
        } else {
            int reachWall = false;
            
            while (!reachWall) {
                wheel_move(Backward, MAX_SPEED);
                int obstacleDetected = false;
                int wallDetected = false;
                while (!obstacleDetected) {
                    read_U();
                    obstacleDetected = (BLU <= 25) || (BRU <= 25); 
                        
                }
                
                wheel_move(StopMotion, MAX_SPEED);
                
                prev_BLU_BRU = fmin(BLU, BRU);
                wheel_move_by_metrics(Left, TURN_SPEED, 90 * FACTOR_SURFACE);
                CyDelay(160);
                wheel_move_by_metrics(Forward, HALF_SPEED, 0.25);
                
                CyDelay(160);
                read_U();
                    
                wallDetected = (fabs(prev_BLU_BRU - LFU) <= 5);
                
                if (wallDetected){
                    moveUntilObs(1, TURN_SPEED, 32);
                    wheel_move_by_metrics(Right, TURN_SPEED, 90 * FACTOR_SURFACE);
                    infiniteTurningAlignment(ALIGN_SPEED, BackAlign);
                    facingRight = 1;
                    reachWall = true;
                }
                else if (!wallDetected){
                    wheel_move_by_metrics(Right, TURN_SPEED, 90 * FACTOR_SURFACE);
                }
            }
            
            wheel_move_by_metrics(Backward, TURN_SPEED, 0.1);
            infiniteTurningAlignment(ALIGN_SPEED, BackAlign);
//            moveUntilObs(0, MAX_SPEED, 27);
//            infiniteTurningAlignment(ALIGN_SPEED, BackAlign);
//            wheel_move_by_metrics(Forward, TURN_SPEED, 0.07);
//            wheel_move_by_metrics(Left, TURN_SPEED, 90 * FACTOR_SURFACE);
//            //infiniteTurningAlignment(ALIGN_SPEED, LeftAlign);
//            moveUntilObs(1, TURN_SPEED, 32);
//            //infiniteTurningAlignment(ALIGN_SPEED, LeftAlign);
//            wheel_move_by_metrics(Right, TURN_SPEED, 90 * FACTOR_SURFACE);
//            //infiniteTurningAlignment(ALIGN_SPEED, LeftAlign);
        }
        
        // wheel_move_by_metrics(Backward, TURN_SPEED, 0.05);
        infiniteTurningAlignment(ALIGN_SPEED, BackAlign);

    }
    
}

void state_1_3() {
    
    read_U();
    
    bool notSeen = true;
    bool arenaWallNotTooFar = true;
    bool arenaWallNotTooClose = false;
    
    initializeSharpIR(0);
    
    CyDelay(200);
    
    if (base_color == YellowBase || base_color == BlueBase) {
        
        double curr_ADC_Level = 0;
        double prev_ADC_Level = 0;
        
        infiniteTurningAlignment(ALIGN_SPEED, FrontAlign);
        
        while (notSeen) {
            
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
        
        infiniteTurningAlignment(ALIGN_SPEED, BackAlign);
        
        while (notSeen) {
            // infiniteTurningAlignment(ALIGN_SPEED, BackAlign);
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

void state_1_4() {
    

    if (base_color == YellowBase || base_color == BlueBase) {
        // wheel_move_by_metrics(Forward, HALF_SPEED, 0.025);
        //infiniteTurningAlignment(ALIGN_SPEED, RightAlign);
        
    } else {
        wheel_move_by_metrics(Backward, HALF_SPEED, 0.0125);
        //infiniteTurningAlignment(ALIGN_SPEED, LeftAlign);  
    }
    
    wheel_move_by_metrics(Left, TURN_SPEED - 5, (90)* FACTOR_SURFACE);
    
    CyDelay(320);
    
    read_U();
    
    double dist = FLU > FRU ? FLU : FRU;
    
    dist = dist - 31.5;
    
    if (dist > 0) {
        wheel_move_by_metrics(Forward, HALF_SPEED, dist/100);
    } else {
        wheel_move_by_metrics(Backward, HALF_SPEED, -1.0*dist/100);
    }
    
    placePuckAtDeck();
    shoot();
        
    
}

void state_1_5() {
    
    double dist;
    
    if (base_color == YellowBase || base_color == BlueBase) {
        //infiniteTurningAlignment(ALIGN_SPEED, RightAlign);
        wheel_move_by_metrics(Left, TURN_SPEED, 90 * FACTOR_SURFACE);
        //infiniteTurningAlignment(TURN_SPEED, RightAlign);
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
        //infiniteTurningAlignment(TURN_SPEED, LeftAlign);
        read_U();
        dist = 0.8 - (BLU + BRU)/200;
        wheel_move_by_metrics(Forward, MAX_SPEED, dist);
        wheel_move_by_metrics(Right, TURN_SPEED, 90 * FACTOR_SURFACE);
        //infiniteTurningAlignment(ALIGN_SPEED, BackAlign);
        read_U();
        dist = (BLU + BRU)/200;
        wheel_move_by_metrics(Backward, MAX_SPEED, dist - 0.025);
    }


}


/* [] END OF FILE */
