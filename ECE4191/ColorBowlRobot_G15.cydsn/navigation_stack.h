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
#ifndef NAVIGATION_STACK_H
#define NAVIGATION_STACK_H

    #define MAX_X 2.37
    #define MAX_Y 2.37
    #define MIN_X 0
    #define MIN_Y 0
    
    
    #include <math.h>
    
    typedef enum {
        BLUE_BASE,
        GREEN_BASE,
        YELLOW_BASE,
        RED_BASE
    } STARTING_BASE;
    
    
    extern double pos_x;
    extern double pos_y;
    extern double heading_angle;
    extern STARTING_BASE start_base_color;
    
    extern double pos_x_base;
    extern double pos_y_base;
    
typedef struct Node {
    double x;
    double y;
    double heading; // in radians
    struct Node* prev;
    struct Node* next;
} Node;

typedef struct NavigationStack {
    Node* head;
    Node* tail;
    int count_nodes;
    double current_x;
    double current_y;
    double current_heading; 
} NavigationStack;

void initializePosition(STARTING_BASE color);
void computePosition(int left_ticks, int right_ticks);

NavigationStack* createNavigationStack(double initial_x, double initial_y);

void destroyNavigationStack(NavigationStack* stack);

void moveRobot(NavigationStack* stack, double new_x, double new_y);

#endif /* NAVIGATION_STACK_H */



/* [] END OF FILE */
