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
#include "puck_manipulation.h"
#include "servo_control.h"
#include "colorsensor.h"
#include "irsensor.h"
#include "locomotion.h"

GripperUpOrDown gripperUpOrDown;
GripperOpenOrClose gripperOpenOrClose;
IROpenOrClose irOpenOrClose;
ColorSensorOpenOrClose colorSensorOpenOrClose;
PuckOnOrNotOnRobot puckOnOrNotOnRobot;
ProximityToBase proximityToBase;
static int arm_state = 0;

void intializePuckManipulation() {
    gripperUpOrDown = GripperUp;
    gripperOpenOrClose = GripperClose;
    irOpenOrClose = IRClose;
    colorSensorOpenOrClose = ColorSensorClose;
    proximityToBase = NotReachBase;
    puckOnOrNotOnRobot = PuckNotOnRobot;
    arm_state = 0;
}

int setGripperArmStatus() {
    if (gripperUpOrDown == GripperUp)  {
        gripperUpOrDown = GripperDown;  
        return 1;
    }
    gripperUpOrDown = GripperUp;  
    return 0;
}

int setGripperHandStatus() {
    if (gripperOpenOrClose == GripperClose) 
    {
        gripperOpenOrClose = GripperOpen;
        return 1;
    }
    
    gripperOpenOrClose = GripperClose;
    return 0;
}

int setIRSensorStatus() {
    if (irOpenOrClose == IRClose) {
        irOpenOrClose = IROpen; 
        return 1;
    }
    irOpenOrClose = IRClose;
    return 0;
}

int setColorSensorStatus() {
    if (colorSensorOpenOrClose == ColorSensorClose) {
        colorSensorOpenOrClose = ColorSensorOpen;   
        return 1;
    }
    colorSensorOpenOrClose = ColorSensorClose;
    return 0;
    
}

void setGripperArm() {
    if (setGripperArmStatus()) {
        GripperArm_Extend();
    }
    else {
        GripperArm_Retract();
    }
}

void setGripperHand() {
    if (setGripperHandStatus()) {
        GripperHand_Open();
    } else {
        GripperHand_GripPuck();
    }
}

void setIRSensor() {
    if (setIRSensorStatus()) {
        startIR();   
    } else {
        stopIR();   
    }
}

void setColorSensor() {
    if (setColorSensorStatus()) {
        ColorDetection_Run(1);
        setColorSensorStatus();
    }
}

void intelligentControlGripper() {
    
    if (arm_state == 0) {
        setGripperArm();
        setGripperHand();
        setIRSensorStatus();
        arm_state = 1;
    }
    else if (arm_state == 1) {
        setColorSensor();
    }
    // STATE 0: INTIALIZATION - gripper close, and up
    
    // STATE 1: PUT DOWN GRIPPER TO START COLLECTING PUCKS, TURN ON IR
    
    // STATE 2: IR DETECTED, SO TURN OFF IR, and START COLOR DETECTING
    
    // STATE 4: TAKE UP PUCKS
    
}
/* [] END OF FILE */
