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
#include "moveoutfrombase.h"
#include "behavior_tree.h"
#include "task.h"

#include <math.h>

#define MAX_SPEED 250
#define TURN_SPEED 230
#define FACTOR_SURFACE 1.0

int _end = 0;
int _cond_moving_out_from_base = 1; // higher level stuff

int _cond_far_enough = 0;
int _cond_is_moving = 0;
int _cond_obs_front = 0;
int _cond_displace = 0;

void _root_moving_out_of_base() {
}


void _end_moving_out_of_base() {
    
    if (_end) {
        
        printValue("DONE MOVING OUT OF BASE\n");
        _cond_is_moving = 0;
        wheel_move(StopMotion, MAX_SPEED);
        
    }
}

void act_moving_straight_out() {
    
    if (!_cond_is_moving && !_cond_far_enough) {
        printValue("MOVING OUT\n");
        _cond_is_moving = 1;
        wheel_move(Forward, MAX_SPEED);
    }
    
}

void act_avoid_headon_collision() {
    
    if (_cond_obs_front && _cond_is_moving) {
        printValue("AVOID\n");
        wheel_move(StopMotion, MAX_SPEED); 
        _cond_is_moving = 0;
    }
}

void act_turn_to_nearest_wall() {
    
    if (_cond_far_enough) {
        
        printValue("TURN\n");
        
        _cond_is_moving = 0;
        wheel_move(StopMotion, MAX_SPEED);
        _cond_is_moving = 1;
        if (base_color == YellowBase || base_color == BlueBase) {
            wheel_move_by_metrics(Left, TURN_SPEED, 90 * FACTOR_SURFACE);
            multiTurningAlignment(TURN_SPEED, LeftAlign, 2);
        }
        else {
            wheel_move_by_metrics(Right, TURN_SPEED, 90 * FACTOR_SURFACE);
            multiTurningAlignment(TURN_SPEED, RightAlign, 2);
        }
        
        _end = 1;
    }
    
}

void act_turn_to_wall_to_align() {
    
    if (_cond_displace && !_cond_far_enough) {
        printValue("ALIGNING\n");
        uniturningAlignment(TURN_SPEED, BackAlign);
    }
}

void cond_check_displacement_to_wall_from_behind() {
    
    read_U();
    
    if (fabs(BLU - BRU) > 5) _cond_displace = 1;
    else _cond_displace = 0;
    
}

void cond_check_distance_to_wall_from_behind() {
    
    read_U();
    
    if ((BLU + BRU)/2 >= 30) _cond_far_enough = 1;
    else _cond_far_enough = 0;
}

void cond_check_obs_front() {
    
    read_U();
    
    if ((FLU < 25) && (FRU < 25)) _cond_obs_front = 1;
    else _cond_obs_front = 0;
}


void bt_moving_out_of_base() {
    
    if (_cond_moving_out_from_base) {
        struct Graph* graph = createGraph(9, 
            &_root_moving_out_of_base, //0
            &_end_moving_out_of_base, // 1
            &cond_check_obs_front, // 2
            &act_avoid_headon_collision, // 3
            &cond_check_displacement_to_wall_from_behind, //4
            &act_turn_to_wall_to_align, // 5
            &cond_check_distance_to_wall_from_behind, // 6
            &act_turn_to_nearest_wall, // 7
            &act_moving_straight_out // 8
        );
        
        addEdge(graph, 0, 1);
        addEdge(graph, 0, 2);
        addEdge(graph, 2, 3);
        addEdge(graph, 0, 4);
        addEdge(graph, 4, 5);
        addEdge(graph, 0, 6);
        addEdge(graph, 6, 7);
        addEdge(graph, 6, 8);
        
        CyDelay(1000);
        
        for (;;) {
            if (_end) break;
            
            for (int j = 0; j < graph->numVertices; j++) {
                graph->visited[j] = 0;
            }
            DFS(graph, 0);   
        }
    }
}

/* [] END OF FILE */
