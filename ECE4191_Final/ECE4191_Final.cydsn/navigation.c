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

#include "navigation.h"
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include "bluetooth.h"
#include "project.h"

#define MAX_X 2.37
#define MAX_Y 2.37
#define MIN_X 0
#define MIN_Y 0

#define TICKS_PER_REVOLUTION 4300
#define WHEEL_RADIUS 0.02695
#define WHEEL_DISTANCE 0.1475
#define HALF_WIDTH WHEEL_DISTANCE/2

double pos_x;
double pos_y;
double heading_angle;
StartingBase start_base_color;

double self_pos_x_base;
double self_pos_y_base;
double other_pos_x_base;
double other_pos_y_base;

void initializePosition(StartingBase color) {
    
    start_base_color = color;
    
    switch (color) {
        case RedBase:
            pos_y = 0.85;
            pos_x = 0.05;
            self_pos_y_base = 0.85;
            self_pos_x_base = 0.05;
            heading_angle = 0;
            printValue("RED\n");
        break;
        case YellowBase:
            pos_y = 1.52;
            pos_x = 0.05;
            self_pos_y_base = 1.52;
            self_pos_x_base = 0.05;
            heading_angle = 0;
            printValue("YELLOW\n");
        break;
        case GreenBase:
            pos_y = 1.52;
            pos_x = 2.32;
            self_pos_y_base = 1.52;
            self_pos_x_base = 2.32;
            heading_angle = CY_M_PI;
            printValue("GREEN\n");
        break;
        case BlueBase:
            pos_y = 0.85;
            pos_x = 2.32;
            self_pos_y_base = 0.85;
            self_pos_x_base = 2.32;
            heading_angle = CY_M_PI;
            printValue("BLUE\n");
        break;

    }
}

void computePosition(int left_ticks, int right_ticks) {
    
    double distance_left = (double) ((left_ticks / TICKS_PER_REVOLUTION) * (2 * CY_M_PI * WHEEL_RADIUS)); 
    double distance_right = (double) ((right_ticks / TICKS_PER_REVOLUTION) * (2 * CY_M_PI * WHEEL_RADIUS)); 
    double radial_angle = (distance_left - distance_right)/(2*HALF_WIDTH);
    
    double R = distance_left * (2 * HALF_WIDTH/(distance_left - distance_right)) + HALF_WIDTH;
    
    double distance_travelled = (distance_left + distance_right)/2;
    
    double alpha = radial_angle/2;
    
    double beta = radial_angle;
    
    pos_x += distance_travelled * cos(heading_angle + (radial_angle/2));
    
    if (pos_x > MAX_X) pos_x = MAX_X;
    if (pos_x < MIN_X) pos_x = MIN_X;

    pos_y += distance_travelled * sin(heading_angle + (radial_angle/2));
    
    if (pos_y > MAX_Y) pos_x = MAX_Y;
    if (pos_y < MIN_Y) pos_x = MIN_Y;
    
    heading_angle += radial_angle;
    
    if (heading_angle > CY_M_PI) heading_angle -= CY_M_PI;
    if (heading_angle <= -CY_M_PI) heading_angle += CY_M_PI;
    
}

void printToBluetooth() {
    int pos_x_prime = (int) (pos_x * 100 + 0.5);
    int pos_y_prime = (int) (pos_y * 100 + 0.5);
    int heading_angle_prime = (int) (heading_angle * 180 / CY_M_PI + 0.5);
    
    printValue("x: %d y: %d, t: %d\n", pos_x_prime, pos_y_prime, heading_angle_prime);
}
/* [] END OF FILE */
