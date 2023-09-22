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
#include "locomotion.h"
#include "motor_control.h"
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <stdbool.h>


volatile int masterPWM = 0;
volatile int slavePWM = 0;
volatile int masterLeftTicks = 0;
volatile int slaveRightTicks = 0;

int lastMasterTicks = 0;
int lastSlaveTicks = 0;

int period = 0;

/* [] END OF FILE */
