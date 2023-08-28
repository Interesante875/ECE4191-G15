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

/* [] END OF FILE */
#include "project.h"
#include <stdio.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "controller.h"

static int error = 0;
static double kp = P_CONST;
static int correction_signal = 0;


void initializePController(double prop_constant) {
    error = 0;
    kp = prop_constant;
    correction_signal = 0;
}

void resetPController() {
    error = 0;
    kp = P_CONST;
    correction_signal = 0;
}

void computePController_error(int master_tick, int slave_tick) {
    if (master_tick > 0 && slave_tick > 0) error = master_tick - slave_tick;
    if (master_tick < 0 && slave_tick < 0) error = -(master_tick - slave_tick);
    if (master_tick > 0 && slave_tick < 0) error = abs(master_tick) - abs(slave_tick);
    if (master_tick < 0 && slave_tick > 0) error = abs(master_tick) - abs(slave_tick);
    
}

void computePController_correctionSignal() {
    correction_signal = (int) (kp * (double) error);
}

uint8 computePController_output(uint8 master_pwm) {
    int slave_pwm = master_pwm + correction_signal;
    return (uint8) slave_pwm>PWM_MAX?PWM_MAX:(slave_pwm<PWM_MIN?PWM_MIN:slave_pwm);
}

uint8 computePController(int master_tick, int slave_tick, uint8 master_pwm) {
    computePController_error(master_tick, slave_tick);
    computePController_correctionSignal();
    return computePController_output(master_pwm);
}
