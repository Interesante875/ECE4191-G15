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
#ifndef SERVO_CONTROL_H
    #define SERVO_CONTROL_H
    
    void setFlag_GripperPWM(int OnOffFlag);
    
    void setFlag_FlickerPWM(int OnOffFlag);
    
    void turnOff_Gripper();
    void turnOff_Flicker();
    
    void GripperHand_Open();
    void GripperHand_GripPuck();
    void GripperHand_Close();
    
    void GripperArm_Extend();
    void GripperArm_Retract();
    
    void FlickerRecoil_Load();
    void FlickerRecoil_Unload();
    
    void FlickerLock_Lock();
    void FlickerLock_Unlock();
    
#endif
/* [] END OF FILE */
