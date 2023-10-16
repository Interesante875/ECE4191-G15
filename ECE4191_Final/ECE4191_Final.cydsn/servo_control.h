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
    
    void setFlag_GripperHand_PWM(int OnOffFlag);
    void setFlag_GripperArm_PWM(int OnOffFlag);
    void setFlag_FlickerRecoilPWM(int OnOffFlag);
    void setFlag_FlickerLockPWM(int OnOffFlag);
    
    void shutdown_Gripper();
    void shutdown_GripperHand();
    void shutdown_GripperArm();
    void boot_FlickerRecoil();
    void shutdown_FlickerRecoil();
    void boot_FlickerLock();
    void shutdown_FlickerLock();
    
    void GripperHand_Open();
    void GripperHand_GripPuck();
    void GripperHand_Close();
    
    void GripperArm_Extend();
    void GripperArm_Retract();
    void GripperArm_Hurl();
    void GripperArm_SmallExtend();
    
    void FlickerRecoil_Load();
    void FlickerRecoil_Unload();
    
    void FlickerLock_Lock();
    void FlickerLock_Unlock();
    
#endif
/* [] END OF FILE */
