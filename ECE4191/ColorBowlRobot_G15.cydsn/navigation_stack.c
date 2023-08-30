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
#include "navigation_stack.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <cytypes.h>

NavigationStack* createNavigationStack(double initial_x, double initial_y) {
    NavigationStack* stack = (NavigationStack*)malloc(sizeof(NavigationStack));
    if (stack == NULL) {
        return NULL;
    }
    stack->head = NULL;
    stack->tail = NULL;
    stack->count_nodes = 0;
    stack->current_x = initial_x;
    stack->current_y = initial_y;
    stack->current_heading = 0; // Initial heading

    return stack;
}

void destroyNavigationStack(NavigationStack* stack) {
    Node* current = stack->head;
    while (current != NULL) {
        Node* temp = current;
        current = current->next;
        free(temp);
    }
    free(stack);
}

// Function to compute the angle in degrees
double computeAngle(double x_distance, double y_distance) {
    double angle = atan2(x_distance, y_distance) * 180.0 / CY_M_PI;
    
    // Convert angle to range -180 to 180
    if (angle > 180.0) {
        angle -= 360.0;
    } else if (angle < -180.0) {
        angle += 360.0;
    }
    
    return angle;
}

void moveRobot(NavigationStack* stack, double new_x, double new_y) {
    
    double distance = 0;
    double azimuth = 0; // angle from the north, -ve means left, +ve is right
    
    double y_distance = new_y - stack->current_y;
    double x_distance = new_x - stack->current_x;
    
    distance = sqrt(pow(y_distance, 2) + pow(x_distance, 2));
    azimuth = computeAngle(x_distance, y_distance);

    // Reassign pointers, update current_x, current_y, and count_nodes
    stack->current_x = new_x;
    stack->current_y = new_y;
    
    Node* new_node = (Node*)malloc(sizeof(Node));
    new_node->x = new_x;
    new_node->y = new_y;
    new_node->prev = stack->tail;
    new_node->next = NULL;
    
    if (stack->head == NULL) {
        stack->head = new_node;
    } else {
        stack->tail->next = new_node;
    }

    stack->tail = new_node;
    stack->count_nodes++;
}

/* [] END OF FILE */
