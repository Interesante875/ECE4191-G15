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
#include "levels.h"

#define ENABLE_BT 1
#define HND_SHAKE 0 

StartingBase base_color;
Color requiredColor_L1;
Color requiredColor_L2;
Color requiredColor_L3;
Color requiredColor_L4;

int STATE_CURR_LEVEL;

void initializeRobotBase() {
    base_color = YellowBase;
    
    initializePosition(base_color);
    
    #if ENABLE_BT
        initializeBluetooth();
    #endif
    
    #if HND_SHAKE
        waitingHandshake();
        requiredColor_L1 = levelOneZoneColor;
        requiredColor_L2 = levelTwoZoneColor;
        STATE_CURR_LEVEL = currentLevel;
    #else
        requiredColor_L1 = GreenColor;
        requiredColor_L2 = RedColor;
        STATE_CURR_LEVEL = 1;
    #endif
    
    printValue("FINDING: %d\n", requiredColor_L1);
    InitalizeUltrasonicSensor();
}
/* [] END OF FILE */
