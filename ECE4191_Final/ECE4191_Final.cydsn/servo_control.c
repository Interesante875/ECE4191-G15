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
#include "bluetooth.h"
#include "servo.h"
#include <math.h>

// Function pointer type to set servo compare value
typedef void (*ServoControlFunction)(int);

static int GripperPWM_Status = 0;
static int FlickerRecoilPWM_Status = 0;
static int FlickerLockPWM_Status = 0;

void setFlag_GripperPWM(int OnOffFlag) {
    GripperPWM_Status = OnOffFlag; 
}
void setFlag_FlickerRecoilPWM(int OnOffFlag) {
    FlickerRecoilPWM_Status = OnOffFlag;
}
void setFlag_FlickerLockPWM(int OnOffFlag) {
    FlickerLockPWM_Status = OnOffFlag;
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

void shutdown_Gripper() {
    if (GripperPWM_Status) {
        turnOff_GripperHand(); 
        turnOff_GripperArm(); 
        setFlag_GripperPWM(!GripperPWM_Status);
    }
}

void GripperHand_Open() {
    if (!GripperPWM_Status) {
        turnOn_GripperHand(); 
        setFlag_GripperPWM(!GripperPWM_Status);
    }
    ServoControlFunction gripperHand = GripperHand_SetCompare;
    smoothControl(870, 980, 5, gripperHand);

}

void GripperHand_GripPuck() {
    if (!GripperPWM_Status) {
        turnOn_GripperHand();   
        setFlag_GripperPWM(!GripperPWM_Status);
    }   
    ServoControlFunction gripperHand = GripperHand_SetCompare;
    smoothControl(980, 960, 2, gripperHand);

}

void GripperHand_Close() {
    if (!GripperPWM_Status) {
        turnOn_GripperHand(); 
        setFlag_GripperPWM(!GripperPWM_Status);
    }   
    ServoControlFunction gripperHand = GripperHand_SetCompare;
    smoothControl(980, 950, 5, gripperHand);
}

void GripperArm_Extend() {
    if (!GripperPWM_Status) {
        turnOn_GripperArm(); 
        setFlag_GripperPWM(!GripperPWM_Status);
    }   
    ServoControlFunction gripperArm = GripperArm_SetCompare;
    smoothControl(980, 865, 1, gripperArm);

}

void GripperArm_SmallExtend() {
    if (!GripperPWM_Status) {
        turnOn_GripperArm(); 
        setFlag_GripperPWM(!GripperPWM_Status);
    }   
    ServoControlFunction gripperArm = GripperArm_SetCompare;
    smoothControl(900, 870, 1, gripperArm);
}

void GripperArm_Retract() {
    if (!GripperPWM_Status) {
        turnOn_GripperArm();   
        setFlag_GripperPWM(!GripperPWM_Status);
    }   
    ServoControlFunction gripperArm = GripperArm_SetCompare;
    smoothControl(850, 950, 1, gripperArm);
    // smoothControl(940, 950, 1, gripperArm);
}

void GripperArm_Hurl() {
    if (!GripperPWM_Status) {
        turnOn_GripperArm();   
        setFlag_GripperPWM(!GripperPWM_Status);
    }  
    ServoControlFunction gripperArm = GripperArm_SetCompare;
    smoothControl(980, 870, 10, gripperArm);
}

void boot_FlickerRecoil() {
    if (!FlickerRecoilPWM_Status) {
        turnOn_FlickerRecoil();   
        setFlag_FlickerRecoilPWM(!FlickerRecoilPWM_Status);
    }  
}

void shutdown_FlickerRecoil() {
    if (FlickerRecoilPWM_Status) {
        turnOff_FlickerRecoil();
        setFlag_FlickerRecoilPWM(!FlickerRecoilPWM_Status);
    }  
}

void FlickerRecoil_Load() {
    boot_FlickerRecoil();
    ServoControlFunction flickerRecoil = FlickerRecoil_SetCompare;
    smoothControl(900, 800, 1, flickerRecoil);
    shutdown_FlickerRecoil();
}

void FlickerRecoil_Unload() {
    boot_FlickerRecoil();
    ServoControlFunction flickerRecoil = FlickerRecoil_SetCompare;
    smoothControl(910, 999, 1, flickerRecoil);
    shutdown_FlickerRecoil();
}

void boot_FlickerLock() {
    if (!FlickerLockPWM_Status) {
        turnOn_FlickerLock();   
        setFlag_FlickerLockPWM(!FlickerLockPWM_Status);
    } 
}

void shutdown_FlickerLock() {
    if (FlickerLockPWM_Status) {
        turnOff_FlickerLock();
        setFlag_FlickerLockPWM(!FlickerLockPWM_Status);
    }  
}

void FlickerLock_Lock() {
    boot_FlickerLock();   
    ServoControlFunction flickerLock = FlickerLock_SetCompare;
    smoothControl(875, 930, 1, flickerLock);
    shutdown_FlickerLock();
}

void FlickerLock_Unlock() {
    boot_FlickerLock();   
    ServoControlFunction flickerLock = FlickerLock_SetCompare;
    smoothControl(930, 870, 1, flickerLock);
    shutdown_FlickerLock();
}

/* [] END OF FILE */
