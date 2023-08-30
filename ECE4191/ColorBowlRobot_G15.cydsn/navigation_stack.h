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

typedef struct Node {
    double x;
    double y;
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

NavigationStack* createNavigationStack(double initial_x, double initial_y);

void destroyNavigationStack(NavigationStack* stack);

void moveRobot(NavigationStack* stack, double new_x, double new_y);

#endif /* NAVIGATION_STACK_H */



/* [] END OF FILE */
