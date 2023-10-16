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
#ifndef TASK_H

    #define TASK_H
    
    typedef enum {
        LeftAlign,
        RightAlign,
        FrontAlign,
        BackAlign
        
    } Alignment;

    
    void retractGripper();
    void lowerGripperToGround();
    void grabPuckAndHold();
    void grabPuckAndDiscard();
    void grabPuckAndHurl();
    void placePuckAtDeck();
    void shoot();
    
    void detectPin();
    void detectPin_RLeftUDS();
    void detectPin_RightUDS();
    
    int wallIsSeen();
    void biturningAlignment();
    void uniturningAlignment(int speed, Alignment align);
    int computeZoneNum(double dist2Wall, int front_or_back);
    
    
    void moveUntilObs(int dir, int speed, double dist);
    
#endif
/* [] END OF FILE */
