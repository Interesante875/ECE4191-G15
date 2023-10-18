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
#include "rasterscan.h"
#include "behavior_tree.h"
#include "math.h"

#define MAX_SPEED 250
#define TURN_SPEED 230
#define HALF_SPEED 210
#define FACTOR_SURFACE 1.0

int _end_rs = 0;
int _cond_rs = 1; // higher level stuff

int _cond_rs_obs_at_front = 0;
int _cond_rs_wall_at_front = 0;

int _cond_facing_right;

void root_raster_scan();

void end_raster_scan() {
    if (_end_rs) {
        printValue("DONE RASTER SCAN\n");
        wheel_move(StopMotion, MAX_SPEED);
    }
}

void cond_front_obs() {
    
    read_U();
    
    if (FLU <= 20 || FRU <= 20) {
        _cond_rs_obs_at_front = 1;      
    } 
    else {
        _cond_rs_obs_at_front = 0;
    }
    
}

void cond_front_is_wall() {
    
    if (_cond_rs_obs_at_front) {
        read_U();
        
        if (FLU <= 20 && FRU <= 20) {
            _cond_rs_wall_at_front = 1;
        }
        else {
            _cond_rs_wall_at_front = 0;   
        }
    }  
}

void act_move_in_line() {
    
    
}

void bt_raster_scan() {
    
    _cond_facing_right = (base_color == YellowBase || base_color == BlueBase);
    
}

/* [] END OF FILE */
