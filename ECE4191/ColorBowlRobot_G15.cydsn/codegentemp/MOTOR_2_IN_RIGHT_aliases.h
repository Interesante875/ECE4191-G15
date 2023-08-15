/*******************************************************************************
* File Name: MOTOR_2_IN_RIGHT.h  
* Version 2.20
*
* Description:
*  This file contains the Alias definitions for Per-Pin APIs in cypins.h. 
*  Information on using these APIs can be found in the System Reference Guide.
*
* Note:
*
********************************************************************************
* Copyright 2008-2015, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/

#if !defined(CY_PINS_MOTOR_2_IN_RIGHT_ALIASES_H) /* Pins MOTOR_2_IN_RIGHT_ALIASES_H */
#define CY_PINS_MOTOR_2_IN_RIGHT_ALIASES_H

#include "cytypes.h"
#include "cyfitter.h"


/***************************************
*              Constants        
***************************************/
#define MOTOR_2_IN_RIGHT_0			(MOTOR_2_IN_RIGHT__0__PC)
#define MOTOR_2_IN_RIGHT_0_INTR	((uint16)((uint16)0x0001u << MOTOR_2_IN_RIGHT__0__SHIFT))

#define MOTOR_2_IN_RIGHT_INTR_ALL	 ((uint16)(MOTOR_2_IN_RIGHT_0_INTR))

#endif /* End Pins MOTOR_2_IN_RIGHT_ALIASES_H */


/* [] END OF FILE */
