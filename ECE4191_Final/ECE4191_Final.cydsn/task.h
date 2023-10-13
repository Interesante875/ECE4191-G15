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
    
#endif
/* [] END OF FILE */
