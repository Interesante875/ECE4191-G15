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
#include "project.h"
#include "bluetooth.h"
#include "ultrasonic.h"
#include "locomotion.h"
#include "colorsensor.h"
#include "servo_control.h"
#include "irsensor.h"
#include "gyroscope.h"
#include "navigation.h"

int main(void)
{
    CyGlobalIntEnable; /* Enable global interrupts. */
    
    StartingBase base_color = RedBase;
    /* Place your initialization/startup code here (e.g. MyInst_Start()) */
    initializeBluetooth();
    startGyroscope();
    
//    FlickerRecoil_Load();
//    CyDelay(1000);
//    FlickerRecoil_Unload();
//    CyDelay(1000);
//    FlickerLock_Unlock();
//    CyDelay(1000);
//    FlickerLock_Lock();
    
//    FlickerRecoil_Load();
//    CyDelay(200);
//    GripperHand_Open();
//    GripperArm_Extend();
//    GripperHand_GripPuck();
//    GripperArm_Retract();
//    shutdown_Gripper();
//    testingValue();
//    initializePosition(base_color);
//    InitalizeUltrasonicSensor();
//    UltrasonicSensor_ChangeState(UdsDetectFront);
//    
//    wheel_move_by_metrics(Forward, 220, 0.25);
//    
//    printToBluetooth();
    
    
//    CyDelay(1000);
//    GripperHand_Open();
//    GripperHand_GripPuck();
////    
//    GripperArm_Retract();
//    
//    turnOff_Gripper();
    
//    initializeBluetooth();
    // testingValue();
//    InitalizeUltrasonicSensor();
//    UltrasonicSensor_ChangeState(UdsDetectFront);
//    
//    CyDelay(1000);
//    double FLU, FRU;
//    
//    FLU = UltrasonicSensor_ReadDistanceData(0);
//    FRU = UltrasonicSensor_ReadDistanceData(1);
//    
//    printValue("FLU: %d FRU:%d\n", (int) FLU, (int) FRU);
//    
//    
//    bool obstacle_not_met = (FLU >= 25) && (FRU >= 25);
//    
//    wheel_move(Forward, 245);
//    
//    // printValue("FLU: %lf FRU:%lf\n", FLU, FRU);
//    while (obstacle_not_met) {
//        FLU = UltrasonicSensor_ReadDistanceData(0);
//        FRU = UltrasonicSensor_ReadDistanceData(1);
//        obstacle_not_met = (FLU >= 25) && (FRU >= 25);
//        
//    }
//    
//    
//    wheel_move(StopMotion, 200);
//    
//    UltrasonicSensor_ChangeState(UdsDetectFLR);
//    
//    double RU, LU;
//    
//    RU = UltrasonicSensor_ReadDistanceData(6);
//    LU = UltrasonicSensor_ReadDistanceData(4);
//    
//    if (LU > RU) {
//        wheel_move_by_metrics (Right, 245, 90);
//    }
//    else {
//        wheel_move_by_metrics (Left, 245, 90);
//    }

    for(;;)
    {

    }
}

/* [] END OF FILE */
