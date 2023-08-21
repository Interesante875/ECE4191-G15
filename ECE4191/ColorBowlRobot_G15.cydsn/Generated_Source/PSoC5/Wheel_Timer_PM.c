/*******************************************************************************
* File Name: Wheel_Timer_PM.c
* Version 2.80
*
*  Description:
*     This file provides the power management source code to API for the
*     Timer.
*
*   Note:
*     None
*
*******************************************************************************
* Copyright 2008-2017, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
********************************************************************************/

#include "Wheel_Timer.h"

static Wheel_Timer_backupStruct Wheel_Timer_backup;


/*******************************************************************************
* Function Name: Wheel_Timer_SaveConfig
********************************************************************************
*
* Summary:
*     Save the current user configuration
*
* Parameters:
*  void
*
* Return:
*  void
*
* Global variables:
*  Wheel_Timer_backup:  Variables of this global structure are modified to
*  store the values of non retention configuration registers when Sleep() API is
*  called.
*
*******************************************************************************/
void Wheel_Timer_SaveConfig(void) 
{
    #if (!Wheel_Timer_UsingFixedFunction)
        Wheel_Timer_backup.TimerUdb = Wheel_Timer_ReadCounter();
        Wheel_Timer_backup.InterruptMaskValue = Wheel_Timer_STATUS_MASK;
        #if (Wheel_Timer_UsingHWCaptureCounter)
            Wheel_Timer_backup.TimerCaptureCounter = Wheel_Timer_ReadCaptureCount();
        #endif /* Back Up capture counter register  */

        #if(!Wheel_Timer_UDB_CONTROL_REG_REMOVED)
            Wheel_Timer_backup.TimerControlRegister = Wheel_Timer_ReadControlRegister();
        #endif /* Backup the enable state of the Timer component */
    #endif /* Backup non retention registers in UDB implementation. All fixed function registers are retention */
}


/*******************************************************************************
* Function Name: Wheel_Timer_RestoreConfig
********************************************************************************
*
* Summary:
*  Restores the current user configuration.
*
* Parameters:
*  void
*
* Return:
*  void
*
* Global variables:
*  Wheel_Timer_backup:  Variables of this global structure are used to
*  restore the values of non retention registers on wakeup from sleep mode.
*
*******************************************************************************/
void Wheel_Timer_RestoreConfig(void) 
{   
    #if (!Wheel_Timer_UsingFixedFunction)

        Wheel_Timer_WriteCounter(Wheel_Timer_backup.TimerUdb);
        Wheel_Timer_STATUS_MASK =Wheel_Timer_backup.InterruptMaskValue;
        #if (Wheel_Timer_UsingHWCaptureCounter)
            Wheel_Timer_SetCaptureCount(Wheel_Timer_backup.TimerCaptureCounter);
        #endif /* Restore Capture counter register*/

        #if(!Wheel_Timer_UDB_CONTROL_REG_REMOVED)
            Wheel_Timer_WriteControlRegister(Wheel_Timer_backup.TimerControlRegister);
        #endif /* Restore the enable state of the Timer component */
    #endif /* Restore non retention registers in the UDB implementation only */
}


/*******************************************************************************
* Function Name: Wheel_Timer_Sleep
********************************************************************************
*
* Summary:
*     Stop and Save the user configuration
*
* Parameters:
*  void
*
* Return:
*  void
*
* Global variables:
*  Wheel_Timer_backup.TimerEnableState:  Is modified depending on the
*  enable state of the block before entering sleep mode.
*
*******************************************************************************/
void Wheel_Timer_Sleep(void) 
{
    #if(!Wheel_Timer_UDB_CONTROL_REG_REMOVED)
        /* Save Counter's enable state */
        if(Wheel_Timer_CTRL_ENABLE == (Wheel_Timer_CONTROL & Wheel_Timer_CTRL_ENABLE))
        {
            /* Timer is enabled */
            Wheel_Timer_backup.TimerEnableState = 1u;
        }
        else
        {
            /* Timer is disabled */
            Wheel_Timer_backup.TimerEnableState = 0u;
        }
    #endif /* Back up enable state from the Timer control register */
    Wheel_Timer_Stop();
    Wheel_Timer_SaveConfig();
}


/*******************************************************************************
* Function Name: Wheel_Timer_Wakeup
********************************************************************************
*
* Summary:
*  Restores and enables the user configuration
*
* Parameters:
*  void
*
* Return:
*  void
*
* Global variables:
*  Wheel_Timer_backup.enableState:  Is used to restore the enable state of
*  block on wakeup from sleep mode.
*
*******************************************************************************/
void Wheel_Timer_Wakeup(void) 
{
    Wheel_Timer_RestoreConfig();
    #if(!Wheel_Timer_UDB_CONTROL_REG_REMOVED)
        if(Wheel_Timer_backup.TimerEnableState == 1u)
        {     /* Enable Timer's operation */
                Wheel_Timer_Enable();
        } /* Do nothing if Timer was disabled before */
    #endif /* Remove this code section if Control register is removed */
}


/* [] END OF FILE */
