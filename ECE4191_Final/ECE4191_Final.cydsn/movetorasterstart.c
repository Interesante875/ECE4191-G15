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
#include "movetorasterstart.h"
#include "behavior_tree.h"
#include "task.h"

#include <math.h>

#define MAX_SPEED 230
#define TURN_SPEED 220
#define FACTOR_SURFACE 1.1

int _end_move_to_raster_start = 0;
int _cond_move_to_raster_start = 1;
int _status_mtrs = 0;

int _cond_is_near_to_wall = 0;
int _cond_far_from_wall = 0;
int _cond_no_obs_front = 0;
int _cond_mtrs_is_moving = 0;
int _cond_mtrs_displace = 0;

void root_move_to_raster_start() {
}


void end_move_to_raster_start() {
    
    if (_end_move_to_raster_start) {
        
        printValue("DONE MOVE TO RASTER START\n");
        wheel_move(StopMotion, MAX_SPEED);

    }
}

void cond_check_if_near_wall() {
    
    if (_status_mtrs == 0) {
        read_U();
        
        if (FLU >= 25 || FRU >= 25) {  
            _cond_is_near_to_wall = 0;
        } else {
            _cond_is_near_to_wall = 1;
            
        }
        
    }
}


void act_stop_at_near_wall() {
    
    if (_status_mtrs == 0 && _cond_is_near_to_wall) {
        
        printValue("DONE MOVE TO NEAREST WALL\n");
        
        wheel_move(StopMotion, MAX_SPEED);  
        if (base_color == YellowBase || base_color == BlueBase) {
            wheel_move_by_metrics(Right, TURN_SPEED, 90);
            multiTurningAlignment(TURN_SPEED, LeftAlign, 3);
        }
        else {
            wheel_move_by_metrics(Left, TURN_SPEED, 90);
            multiTurningAlignment(TURN_SPEED, RightAlign, 3);
        }
        
        _status_mtrs = 1;
    }
}

void act_move_toward_wall() {
    if (_status_mtrs == 0 && !_cond_is_near_to_wall) {
        wheel_move(Forward, MAX_SPEED);   
    }
}


void cond_check_if_far_from_back_wall() {
    
    if (_status_mtrs == 1) {
        read_U();
        
        if (BLU <= 40 && BRU <= 40) {
            
            _cond_far_from_wall = 0;
        } else {
            _cond_far_from_wall = 1;
            
        }
        
    }
    
}

void cond_check_if_obs_front() {
    if (_status_mtrs == 1 && !_cond_far_from_wall) {
        read_U();
        if (FLU > 20 || FRU > 20) {
            
            _cond_no_obs_front = 1;
        }
        else {
            _cond_no_obs_front = 0;
        }
        
    }
    
}

void act_stop_because_obs() {
    if (_status_mtrs == 1 && !_cond_far_from_wall && !_cond_no_obs_front && _cond_mtrs_is_moving) {
        printValue("ENCOUNTER OBS\n");   
        wheel_move(StopMotion, MAX_SPEED);
        _cond_mtrs_is_moving = 0;
    } 
}

void act_move_forward() {
    if (_status_mtrs == 1 && !_cond_far_from_wall && _cond_no_obs_front && !_cond_mtrs_is_moving) {
        _cond_mtrs_is_moving = 1;
        wheel_move(Forward, MAX_SPEED);
    }
    
}

void act_raster_start_pos() {
    if (_status_mtrs == 1 && _cond_far_from_wall) {
        
        printValue("ARRIVE TO START POS\n");
        if (base_color == YellowBase || base_color == BlueBase) {
            CyDelay(1000);
            wheel_move_by_metrics(Right, TURN_SPEED, 90 * FACTOR_SURFACE);
            wheel_move_by_metrics(Backward, MAX_SPEED, 0.1);
            multiTurningAlignment(TURN_SPEED, BackAlign, 4);
        }
        else {
            CyDelay(1000);
            wheel_move_by_metrics(Left, TURN_SPEED, 90 * FACTOR_SURFACE);
            wheel_move_by_metrics(Backward, MAX_SPEED, 0.1);
            multiTurningAlignment(TURN_SPEED, BackAlign, 4);
        }
        
        _status_mtrs = 2;
        
    }
    
}

void cond_mtrs_check_displacement_to_wall() {
    if (_status_mtrs == 2) {
        read_U();
        
        if (fabs(BLU - BRU) > 2) _cond_mtrs_displace = 1;
        else {
            _cond_mtrs_displace = 0;
            _end_move_to_raster_start = 1;
        }
    }
    
}

void act_mtrs_align_backwall() {
    if (_status_mtrs == 2 && _cond_mtrs_displace) {
        printValue("ALIGNING\n");
        multiTurningAlignment(TURN_SPEED, BackAlign, 3);
        _end_move_to_raster_start = 1;
    }
}

void bt_move_to_raster_start() {
    if (_cond_move_to_raster_start) {
        struct Graph* graph = createGraph(12, 
            &root_move_to_raster_start, //0
            &end_move_to_raster_start, //1
            &cond_check_if_near_wall, //2
            &act_stop_at_near_wall, //3
            &act_move_toward_wall, //4
            &cond_check_if_far_from_back_wall, //5
            &cond_check_if_obs_front, //6
            &act_stop_because_obs, //7
            &act_move_forward, //8
            &act_raster_start_pos, //9
            &cond_mtrs_check_displacement_to_wall, //10
            &act_mtrs_align_backwall //11
            
        );
        
        addEdge(graph, 0, 1);
        addEdge(graph, 0, 2);
        addEdge(graph, 2, 3);
        addEdge(graph, 2, 4);
        addEdge(graph, 0, 5);
        addEdge(graph, 5, 6);
        addEdge(graph, 6, 7);
        addEdge(graph, 6, 8);
        addEdge(graph, 6, 9);
        addEdge(graph, 0, 10);
        addEdge(graph, 10, 11);
        
        for (;;) {
            if (_end_move_to_raster_start) break;
            
            for (int j = 0; j < graph->numVertices; j++) {
                graph->visited[j] = 0;
            }
            DFS(graph, 0);   
        }
    }
    
}
/* [] END OF FILE */
