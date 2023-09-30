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
#include "servo_control.h"
#include "servo.h"
#include <math.h>

// Function pointer type to set servo compare value
typedef void (*ServoControlFunction)(int);

static int GripperPWM_Status = 0;
static int FlickerPWM_Status = 0;

void setFlag_GripperPWM(int OnOffFlag) {
    GripperPWM_Status = OnOffFlag; 
}
void setFlag_FlickerPWM(int OnOffFlag) {
    FlickerPWM_Status = OnOffFlag;
}

// Function to control the servo smoothly
void smoothControl(int begin, int end, int step, ServoControlFunction servoFunc) {
    if (begin < end) {
        for (int i = begin; i <= end; i += step) {
            servoFunc(i);
            CyDelay(5);  
        }
    } else {
        for (int i = begin; i >= end; i -= step) {
            servoFunc(i);
            CyDelay(5);  
        }
    }
}

// Function to control the servo smoothly with exponential interpolation
void exponentialControl(int begin, int end, int steps, ServoControlFunction servoFunc) {
    if (begin < end) {
        for (int i = 0; i <= steps; i++) {
            double alpha = (double)i / steps; // Interpolation factor between 0 and 1
            int value = begin + (int)(pow(alpha, 2) * (end - begin)); // Exponential interpolation
            servoFunc(value);
            CyDelay(2);
        }
    } else {
        for (int i = 0; i <= steps; i++) {
            double alpha = (double)i / steps; // Interpolation factor between 0 and 1
            int value = end + (int)(pow(1 - alpha, 2) * (begin - end)); // Exponential interpolation
            servoFunc(value);
            CyDelay(2); 
        }
    }
}

void GripperHand_Open() {
    if (!GripperPWM_Status) {
        turnOn_GripperHand(); 
        setFlag_GripperPWM(!GripperPWM_Status);
    }
    ServoControlFunction gripperHand = GripperHand_SetCompare;
    smoothControl(870, 980, 10, gripperHand);

}

void GripperHand_GripPuck() {
    if (!GripperPWM_Status) {
        turnOn_GripperHand();   
        setFlag_GripperPWM(!GripperPWM_Status);
    }   
    ServoControlFunction gripperHand = GripperHand_SetCompare;
    smoothControl(980, 960, 4, gripperHand);

}

void GripperHand_Close() {
    if (!GripperPWM_Status) {
        turnOn_GripperHand(); 
        setFlag_GripperPWM(!GripperPWM_Status);
    }   
    ServoControlFunction gripperHand = GripperHand_SetCompare;
    smoothControl(980, 950, 10, gripperHand);
}

void GripperArm_Extend() {
    if (!GripperPWM_Status) {
        turnOn_GripperArm(); 
        setFlag_GripperPWM(!GripperPWM_Status);
    }   
    ServoControlFunction gripperArm = GripperArm_SetCompare;
    smoothControl(980, 870, 10, gripperArm);
}

void GripperArm_Retract() {
    if (!GripperPWM_Status) {
        turnOn_GripperArm();   
        setFlag_GripperPWM(!GripperPWM_Status);
    }   
    ServoControlFunction gripperArm = GripperArm_SetCompare;
    smoothControl(870, 945, 2, gripperArm);
}

void FlickerRecoil_Load() {
    if (!FlickerPWM_Status) {
        turnOn_FlickerRecoil();   
        setFlag_FlickerPWM(!FlickerPWM_Status);
    }  
    ServoControlFunction flickerRecoil = FlickerRecoil_SetCompare;
    smoothControl(900, 820, 1, flickerRecoil);
    turnOff_FlickerRecoil(); 
    setFlag_FlickerPWM(!FlickerPWM_Status);
}

void FlickerRecoil_Unload() {
    if (!FlickerPWM_Status) {
        turnOn_FlickerRecoil();   
        setFlag_FlickerPWM(!FlickerPWM_Status);
    }     
    ServoControlFunction flickerRecoil = FlickerRecoil_SetCompare;
    smoothControl(920, 990, 1, flickerRecoil);
//    smoothControl(890, 875, 1,  flickerRecoil);
//    turnOff_FlickerRecoil(); 
//    setFlag_FlickerPWM(!FlickerPWM_Status);
}

void FlickerLock_Lock() {
    if (!FlickerPWM_Status) {
        turnOn_FlickerLock();   
        setFlag_FlickerPWM(!FlickerPWM_Status);
    }     
    ServoControlFunction flickerLock = FlickerLock_SetCompare;
    smoothControl(875, 930, 5, flickerLock);
}

void FlickerLock_Unlock() {
    if (!FlickerPWM_Status) {
        turnOn_FlickerLock();   
        setFlag_FlickerPWM(!FlickerPWM_Status);
    }     
    ServoControlFunction flickerLock = FlickerLock_SetCompare;
    smoothControl(930, 880, 5, flickerLock);
}

/* [] END OF FILE */
