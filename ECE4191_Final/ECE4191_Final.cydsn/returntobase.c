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

#define MAX_SPEED 250
#define TURN_SPEED 230
#define HALF_SPEED 210
#define FACTOR_SURFACE 1.0

//int _end = 0;
//int _cond_return_to_base = 1; // higher level stuff
//
//int _cond_far_enough = 0;
//int _cond_is_moving = 0;
//int _cond_obs_front = 0;
//
//void _root_return_to_base() {
//}
//
//void _end_return_to_base() {
//    
//    if (_end) {
//        
//        printValue("DONE RETURN TO BASE\n");
//        _cond_is_moving = 0;
//        wheel_move(StopMotion, MAX_SPEED);
//        
//    }
//}
//
//
//void act_avoid_headon_collision() {
//    
//    if (_cond_obs_front && _cond_is_moving) {
//        printValue("AVOID\n");
//        wheel_move(StopMotion, MAX_SPEED); 
//        _cond_is_moving = 0;
//    }
//}
//
//void _cond_check_obs_front() {
//    
//    read_U();
//    
//    if ((FLU < 25) && (FRU < 25)) _cond_obs_front = 1;
//    else _cond_obs_front = 0;
//    
//}
//
//void cond_check_distance_to_wall_from_behind() {
//    
//    read_U();
//    
//    if ((BLU + BRU)/2 >= 30) _cond_far_enough = 1;
//    else _cond_far_enough = 0;
//    
//} 
//
//void bt_return_to_base() {
//    
//    
//}
/* [] END OF FILE */
