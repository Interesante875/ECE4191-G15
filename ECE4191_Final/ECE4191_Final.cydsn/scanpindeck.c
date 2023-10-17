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
#include "levels.h"
#include "scanpindeck.h"
#include "behavior_tree.h"
#include "task.h"

#include <math.h>

#define MAX_SPEED 250
#define TURN_SPEED 230
#define HALF_SPEED 210
#define FACTOR_SURFACE 1.0

double curr_ADC_Level = 0;
double prev_ADC_Level = 0;

int _end_scan_pin_deck = 0;
int _cond_scan_pin_deck = 1; // higher level stuff
int _init_deck_num = 0; // higher level stuff
int _scanned_deck_num = 0;

int _status_spd = 0;

int _cond_not_seen_pin = 1;
int _cond_spd_wall_too_far = 0;
int _cond_spd_wall_too_near = 0;
int _cond_spd_is_moving_forward = 0;
int _cond_spd_is_moving_backward = 0;

void root_scan_pin_deck() {
}


void end_scan_pin_deck() {
    
    if (_end_scan_pin_deck) {
        
        printValue("DONE SCANNING PIN DECK\n");
        wheel_move(StopMotion, MAX_SPEED);
    }
}

void cond_spd_check_wall_distance() {
    
    read_U();
    
    if (BLU <= 15 || BRU <= 15) {
        _status_spd = 0;
    }
    else if (BLU > 45 || BRU > 45) {
        _status_spd = 1; 
    }
    
}

void act_spd_move_backward() {
    printValue("MOVE BACKWARD: %d\n", _status_spd);
    if (_status_spd == 1 && !_cond_spd_is_moving_backward) {
        multiTurningAlignment(HALF_SPEED, BackAlign, 3);
        if (base_color == YellowBase || base_color == BlueBase) {  
            curr_ADC_Level = SharpIR_ReadDistance(1);
            prev_ADC_Level = SharpIR_ReadDistance(1);
        } else {    
            curr_ADC_Level = SharpIR_ReadDistance(0);
            prev_ADC_Level = SharpIR_ReadDistance(0);
        }
        wheel_move(Backward, HALF_SPEED);
        _cond_spd_is_moving_backward = 1;
        _cond_spd_is_moving_forward = 0;
    }
}

void act_spd_move_forward() {
    printValue("MOVE FORWARD: %d\n", _status_spd);
    if (_status_spd == 0 && !_cond_spd_is_moving_forward) {
        if (base_color == YellowBase || base_color == BlueBase) {  
            curr_ADC_Level = SharpIR_ReadDistance(1);
            prev_ADC_Level = SharpIR_ReadDistance(1);
        } else {    
            curr_ADC_Level = SharpIR_ReadDistance(0);
            prev_ADC_Level = SharpIR_ReadDistance(0);
        }
        wheel_move(Forward, HALF_SPEED);
        _cond_spd_is_moving_forward = 1;
        _cond_spd_is_moving_backward = 0;
    }
}

void cond_scan() {
    if (base_color == YellowBase || base_color == BlueBase) {
        curr_ADC_Level = SharpIR_ReadDistance(1);
    }
    else {
        curr_ADC_Level = SharpIR_ReadDistance(0);
    }
        
    if (curr_ADC_Level - prev_ADC_Level >= 4.5) {
        _cond_not_seen_pin = 0;
        wheel_move(StopMotion, HALF_SPEED);
        // NOT ENDED HERE
        _end_scan_pin_deck = 1;
    } else {
        prev_ADC_Level = curr_ADC_Level;   
    }

    
}

void bt_scan_pin_deck() {
    

    if (_cond_scan_pin_deck) {
        initializeSharpIR(0);
        struct Graph* graph = createGraph(6, 
            &root_scan_pin_deck, //0
            &end_scan_pin_deck, // 1
            &cond_scan, //2
            &cond_spd_check_wall_distance, //3
            &act_spd_move_backward, // 4
            &act_spd_move_forward //5
        );
        
        addEdge(graph, 0, 1);
        addEdge(graph, 0, 2);
        addEdge(graph, 0, 3);
        addEdge(graph, 3, 4);
        addEdge(graph, 3, 5);
    
        for (;;) {
            if (_end_scan_pin_deck) break;
            
            for (int j = 0; j < graph->numVertices; j++) {
                graph->visited[j] = 0;
            }
            DFS(graph, 0);   
        }
        
        stopSharpIR();
    }
    
    
    
}

/* [] END OF FILE */
