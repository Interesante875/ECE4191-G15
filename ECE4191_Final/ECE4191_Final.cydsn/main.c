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
#include "LevelThree.h"
#include "analog.h"
#include "trialRun.h"

int main(void)
{
    CyGlobalIntEnable; /* Enable global interrupts. */
    initializeRobotBase();
    run_L1();
//    if (STATE_CURR_LEVEL == 1) {
//        run_L1();
//        CyDelay(5000);
//        run_L2();
//        CyDelay(5000);
//        run_L3();
//        CyDelay(5000);
//        run_L4();
//        CyDelay(5000);
//    } else if (STATE_CURR_LEVEL == 2) {
//        run_L2();
//        CyDelay(5000);
//        run_L3();
//        CyDelay(5000);
//        run_L4();
//        CyDelay(5000);
//    }  else if (STATE_CURR_LEVEL == 3) {
//        run_L3();
//        CyDelay(5000);
//        run_L4();
//        CyDelay(5000);
//    }  else if (STATE_CURR_LEVEL == 4) {
//        run_L4();
//        CyDelay(5000);
//    }
//
//    initializeRobot();
//    test_run();

    
    for(;;)
    {

    }
}

/* [] END OF FILE */
