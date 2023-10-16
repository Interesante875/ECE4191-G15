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
#ifndef NAVIGATION_H
#define NAVIGATION_H
    
    typedef enum {
        BlueBase,
        GreenBase,
        YellowBase,
        RedBase
    } StartingBase;
    
    typedef enum {
        FrontFacing,
        HorizontalFacing,
        BackFacing
    } StartingOrientation;
    
    extern double pos_x;
    extern double pos_y;
    extern double heading_angle;
    extern StartingBase start_base_color;
    
    extern double self_pos_x_base;
    extern double self_pos_y_base;
    extern double other_pos_x_base;
    extern double other_pos_y_base;
    
    void initializePosition(StartingBase color);
    void computePosition(int left_ticks, int right_ticks);
    
    void printPositionToBluetooth();

#endif
/* [] END OF FILE */
