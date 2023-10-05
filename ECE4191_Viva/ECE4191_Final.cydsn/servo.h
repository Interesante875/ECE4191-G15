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
#ifndef SERVO_H
#define SERVO_H
    
    void turnOn_GripperHand();
    void turnOff_GripperHand();
    void GripperHand_SetCompare(int compareValue);
    int GripperHand_ReadCompare();
    
    void turnOn_GripperArm();
    void turnOff_GripperArm();
    void GripperArm_SetCompare(int compareValue);
    int GripperArm_ReadCompare();
    
    void turnOn_FlickerRecoil();
    void turnOff_FlickerRecoil();
    void FlickerRecoil_SetCompare(int compareValue);
    int FlickerRecoil_ReadCompare();
    
    void turnOn_FlickerLock();
    void turnOff_FlickerLock();
    void FlickerLock_SetCompare(int compareValue);
    int FlickerLock_ReadCompare();
    
#endif
/* [] END OF FILE */
