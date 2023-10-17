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
#include "returntobase.h"
#include "behavior_tree.h"
#include "math.h"

#define MAX_SPEED 250
#define TURN_SPEED 230
#define HALF_SPEED 210
#define FACTOR_SURFACE 1.0

int _end_rtb = 0;
int _cond_return_to_base = 1; // higher level stuff

int _cond_rtb_far_from_nearest_wall = 0;
int _cond_rtb_is_moving = 0;
int _cond_rtb_obs_front = 0;
int _cond_rtb_displace = 0;
int _cond_rtb_far_from_base = 0;


int _status_rtb = 0;


void root_return_to_base() {
}

void end_return_to_base() {
    
    if (_end_rtb) {
        
        printValue("DONE RETURN TO BASE\n");
        _cond_rtb_is_moving = 0;
        wheel_move(StopMotion, MAX_SPEED);
        
    }
}

void cond_rtb_check_distance_to_wall_from_behind() {
    
    if (_status_rtb == 0) { 
        read_U();
        
        if (BLU > 15 || BRU > 15) {
            _cond_rtb_far_from_nearest_wall = 1;
        }
        else {
            _cond_rtb_far_from_nearest_wall = 0;
        }
    }
} 

void act_stop_reverse_to_nearest_wall() {
    
    if (!_cond_rtb_far_from_nearest_wall && _status_rtb == 0) {
        
        printValue("FOUND NEARESR WALL\n");
        _cond_rtb_is_moving = 0;
        
        wheel_move(StopMotion, MAX_SPEED);
        
        _status_rtb = 1;
    }
    
}

void act_reverse_to_nearest_wall() {
    
    if (_cond_rtb_far_from_nearest_wall && _status_rtb == 0) {
        
        printValue("REVERSE TO NEAREST WALL\n");
        
        _cond_rtb_is_moving = 1;
        
        wheel_move(Backward, MAX_SPEED);
        
    }

}

void cond_rtb_check_displacement_to_wall_from_behind() {
    
    if (_status_rtb == 1) {
        read_U();
        
        if (fabs(BLU - BRU) > 1) _cond_rtb_displace = 1;
        else {
            _cond_rtb_displace = 0;
            _status_rtb = 2;
        }
    }
    
}

void act_align_to_nearest_wall() {
    
    if (_cond_rtb_displace && _status_rtb == 1) {
        printValue("ALIGNING\n");
        multiTurningAlignment(HALF_SPEED, BackAlign, 3);
        _status_rtb = 2;
    }
}



void act_move_to_base_front() {
    
    if (_status_rtb == 2) {
        
        printValue("MOVE TO BASE FRONT\n");
        
        read_U();
        
        double dist = 0.81 - (BLU + BRU)/200;
        wheel_move_by_metrics(Forward, MAX_SPEED, dist);
        
        if (base_color == YellowBase || base_color == BlueBase) {
            
            wheel_move_by_metrics(Left, TURN_SPEED, 90 * FACTOR_SURFACE);
            
        }
        else {
            
            wheel_move_by_metrics(Right, TURN_SPEED, 90 * FACTOR_SURFACE);
        }

        _status_rtb = 3;
    }
    
}

void cond_reversing_check_near_enough() {
    
    if (_status_rtb == 3) {
        
        read_U();
        
        if (BLU >= 33 && BRU >= 33) {
            _cond_rtb_far_from_base = 1;
        } 
        else {
            _cond_rtb_far_from_base = 0;
        }
        
    }
}

void act_reverse_a_bit() {
    
    if (_status_rtb == 3 && _cond_rtb_far_from_base) {
        
        wheel_move_by_metrics(Backward, MAX_SPEED, 0.125);
        
    }
}

void cond_misalign_base() {
    if (_status_rtb == 3 && !_cond_rtb_far_from_base) {

        read_U();
        
        if (fabs(BLU - BRU) > 2) _cond_rtb_displace = 1;
        else {
            _cond_rtb_displace = 0;
        }
        
    }
}

void act_align_base_wall() {
    
    if (_status_rtb == 3 && !_cond_rtb_far_from_base && _cond_rtb_displace) {
        printValue("ALIGNING\n");
        multiTurningAlignment(HALF_SPEED, BackAlign, 3);
    }
}

void act_reverse_back_to_base() {
    
    if (_status_rtb == 3 && !_cond_rtb_far_from_base && !_cond_rtb_displace) {
        printValue("PARKING\n");
        
        CyDelay(320);
        
        read_U();
        
        double backDist = (BLU + BRU)/200;
    
        wheel_move_by_metrics(Backward, MAX_SPEED, backDist - 0.04);
        
        _end_rtb = 1;
    } 
}

void bt_return_to_base() {
    
    if (_cond_return_to_base) {
        struct Graph* graph = createGraph(13, 
            &root_return_to_base, //0
            &end_return_to_base, //1
            &cond_rtb_check_distance_to_wall_from_behind, //2
            &act_stop_reverse_to_nearest_wall, //3
            &act_reverse_to_nearest_wall, //4
            &cond_rtb_check_displacement_to_wall_from_behind, //5
            &act_align_to_nearest_wall, //6
            &act_move_to_base_front, //7
            &cond_reversing_check_near_enough, //8
            &act_reverse_a_bit, //9
            &cond_misalign_base, //10
            &act_align_base_wall, //11
            &act_reverse_back_to_base //12
            
        );
        
        addEdge(graph, 0, 1);
        addEdge(graph, 0, 2);
        addEdge(graph, 2, 3);
        addEdge(graph, 2, 4);
        addEdge(graph, 0, 5);
        addEdge(graph, 5, 6);
        addEdge(graph, 0, 7);
        addEdge(graph, 0, 8);
        addEdge(graph, 8, 9);
        addEdge(graph, 0, 10);
        addEdge(graph, 10, 11);
        addEdge(graph, 0, 12);
        
        for (;;) {
            if (_end_rtb) break;
            
            for (int j = 0; j < graph->numVertices; j++) {
                graph->visited[j] = 0;
            }
            DFS(graph, 0);   
        }
        
        
    }
}
/* [] END OF FILE */
