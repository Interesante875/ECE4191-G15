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
#include <math.h>
#include "bluetooth.h"
#include "levels.h"
#include "LevelOne.h"
#include "analog.h"
#include "trialRun.h"

int main(void)
{
    CyGlobalIntEnable; /* Enable global interrupts. */
//    initializeRobotBase();
//    run_L1();

    initializeRobot();
    test_run();

    
    
    for(;;)
    {

    }
}

/* [] END OF FILE */
