/*******************************************************************************
* File Name: Wheel_Timer.h
* Version 2.80
*
*  Description:
*     Contains the function prototypes and constants available to the timer
*     user module.
*
*   Note:
*     None
*
********************************************************************************
* Copyright 2008-2017, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
********************************************************************************/

#if !defined(CY_TIMER_Wheel_Timer_H)
#define CY_TIMER_Wheel_Timer_H

#include "cytypes.h"
#include "cyfitter.h"
#include "CyLib.h" /* For CyEnterCriticalSection() and CyExitCriticalSection() functions */

extern uint8 Wheel_Timer_initVar;

/* Check to see if required defines such as CY_PSOC5LP are available */
/* They are defined starting with cy_boot v3.0 */
#if !defined (CY_PSOC5LP)
    #error Component Timer_v2_80 requires cy_boot v3.0 or later
#endif /* (CY_ PSOC5LP) */


/**************************************
*           Parameter Defaults
**************************************/

#define Wheel_Timer_Resolution                 32u
#define Wheel_Timer_UsingFixedFunction         0u
#define Wheel_Timer_UsingHWCaptureCounter      0u
#define Wheel_Timer_SoftwareCaptureMode        0u
#define Wheel_Timer_SoftwareTriggerMode        0u
#define Wheel_Timer_UsingHWEnable              0u
#define Wheel_Timer_EnableTriggerMode          0u
#define Wheel_Timer_InterruptOnCaptureCount    0u
#define Wheel_Timer_RunModeUsed                1u
#define Wheel_Timer_ControlRegRemoved          0u

#if defined(Wheel_Timer_TimerUDB_sCTRLReg_SyncCtl_ctrlreg__CONTROL_REG)
    #define Wheel_Timer_UDB_CONTROL_REG_REMOVED            (0u)
#elif  (Wheel_Timer_UsingFixedFunction)
    #define Wheel_Timer_UDB_CONTROL_REG_REMOVED            (0u)
#else 
    #define Wheel_Timer_UDB_CONTROL_REG_REMOVED            (1u)
#endif /* End Wheel_Timer_TimerUDB_sCTRLReg_SyncCtl_ctrlreg__CONTROL_REG */


/***************************************
*       Type defines
***************************************/


/**************************************************************************
 * Sleep Wakeup Backup structure for Timer Component
 *************************************************************************/
typedef struct
{
    uint8 TimerEnableState;
    #if(!Wheel_Timer_UsingFixedFunction)

        uint32 TimerUdb;
        uint8 InterruptMaskValue;
        #if (Wheel_Timer_UsingHWCaptureCounter)
            uint8 TimerCaptureCounter;
        #endif /* variable declarations for backing up non retention registers in CY_UDB_V1 */

        #if (!Wheel_Timer_UDB_CONTROL_REG_REMOVED)
            uint8 TimerControlRegister;
        #endif /* variable declaration for backing up enable state of the Timer */
    #endif /* define backup variables only for UDB implementation. Fixed function registers are all retention */

}Wheel_Timer_backupStruct;


/***************************************
*       Function Prototypes
***************************************/

void    Wheel_Timer_Start(void) ;
void    Wheel_Timer_Stop(void) ;

void    Wheel_Timer_SetInterruptMode(uint8 interruptMode) ;
uint8   Wheel_Timer_ReadStatusRegister(void) ;
/* Deprecated function. Do not use this in future. Retained for backward compatibility */
#define Wheel_Timer_GetInterruptSource() Wheel_Timer_ReadStatusRegister()

#if(!Wheel_Timer_UDB_CONTROL_REG_REMOVED)
    uint8   Wheel_Timer_ReadControlRegister(void) ;
    void    Wheel_Timer_WriteControlRegister(uint8 control) ;
#endif /* (!Wheel_Timer_UDB_CONTROL_REG_REMOVED) */

uint32  Wheel_Timer_ReadPeriod(void) ;
void    Wheel_Timer_WritePeriod(uint32 period) ;
uint32  Wheel_Timer_ReadCounter(void) ;
void    Wheel_Timer_WriteCounter(uint32 counter) ;
uint32  Wheel_Timer_ReadCapture(void) ;
void    Wheel_Timer_SoftwareCapture(void) ;

#if(!Wheel_Timer_UsingFixedFunction) /* UDB Prototypes */
    #if (Wheel_Timer_SoftwareCaptureMode)
        void    Wheel_Timer_SetCaptureMode(uint8 captureMode) ;
    #endif /* (!Wheel_Timer_UsingFixedFunction) */

    #if (Wheel_Timer_SoftwareTriggerMode)
        void    Wheel_Timer_SetTriggerMode(uint8 triggerMode) ;
    #endif /* (Wheel_Timer_SoftwareTriggerMode) */

    #if (Wheel_Timer_EnableTriggerMode)
        void    Wheel_Timer_EnableTrigger(void) ;
        void    Wheel_Timer_DisableTrigger(void) ;
    #endif /* (Wheel_Timer_EnableTriggerMode) */


    #if(Wheel_Timer_InterruptOnCaptureCount)
        void    Wheel_Timer_SetInterruptCount(uint8 interruptCount) ;
    #endif /* (Wheel_Timer_InterruptOnCaptureCount) */

    #if (Wheel_Timer_UsingHWCaptureCounter)
        void    Wheel_Timer_SetCaptureCount(uint8 captureCount) ;
        uint8   Wheel_Timer_ReadCaptureCount(void) ;
    #endif /* (Wheel_Timer_UsingHWCaptureCounter) */

    void Wheel_Timer_ClearFIFO(void) ;
#endif /* UDB Prototypes */

/* Sleep Retention APIs */
void Wheel_Timer_Init(void)          ;
void Wheel_Timer_Enable(void)        ;
void Wheel_Timer_SaveConfig(void)    ;
void Wheel_Timer_RestoreConfig(void) ;
void Wheel_Timer_Sleep(void)         ;
void Wheel_Timer_Wakeup(void)        ;


/***************************************
*   Enumerated Types and Parameters
***************************************/

/* Enumerated Type B_Timer__CaptureModes, Used in Capture Mode */
#define Wheel_Timer__B_TIMER__CM_NONE 0
#define Wheel_Timer__B_TIMER__CM_RISINGEDGE 1
#define Wheel_Timer__B_TIMER__CM_FALLINGEDGE 2
#define Wheel_Timer__B_TIMER__CM_EITHEREDGE 3
#define Wheel_Timer__B_TIMER__CM_SOFTWARE 4



/* Enumerated Type B_Timer__TriggerModes, Used in Trigger Mode */
#define Wheel_Timer__B_TIMER__TM_NONE 0x00u
#define Wheel_Timer__B_TIMER__TM_RISINGEDGE 0x04u
#define Wheel_Timer__B_TIMER__TM_FALLINGEDGE 0x08u
#define Wheel_Timer__B_TIMER__TM_EITHEREDGE 0x0Cu
#define Wheel_Timer__B_TIMER__TM_SOFTWARE 0x10u


/***************************************
*    Initialial Parameter Constants
***************************************/

#define Wheel_Timer_INIT_PERIOD             9999999u
#define Wheel_Timer_INIT_CAPTURE_MODE       ((uint8)((uint8)0u << Wheel_Timer_CTRL_CAP_MODE_SHIFT))
#define Wheel_Timer_INIT_TRIGGER_MODE       ((uint8)((uint8)0u << Wheel_Timer_CTRL_TRIG_MODE_SHIFT))
#if (Wheel_Timer_UsingFixedFunction)
    #define Wheel_Timer_INIT_INTERRUPT_MODE (((uint8)((uint8)0u << Wheel_Timer_STATUS_TC_INT_MASK_SHIFT)) | \
                                                  ((uint8)((uint8)0 << Wheel_Timer_STATUS_CAPTURE_INT_MASK_SHIFT)))
#else
    #define Wheel_Timer_INIT_INTERRUPT_MODE (((uint8)((uint8)0u << Wheel_Timer_STATUS_TC_INT_MASK_SHIFT)) | \
                                                 ((uint8)((uint8)0 << Wheel_Timer_STATUS_CAPTURE_INT_MASK_SHIFT)) | \
                                                 ((uint8)((uint8)0 << Wheel_Timer_STATUS_FIFOFULL_INT_MASK_SHIFT)))
#endif /* (Wheel_Timer_UsingFixedFunction) */
#define Wheel_Timer_INIT_CAPTURE_COUNT      (2u)
#define Wheel_Timer_INIT_INT_CAPTURE_COUNT  ((uint8)((uint8)(1u - 1u) << Wheel_Timer_CTRL_INTCNT_SHIFT))


/***************************************
*           Registers
***************************************/

#if (Wheel_Timer_UsingFixedFunction) /* Implementation Specific Registers and Register Constants */


    /***************************************
    *    Fixed Function Registers
    ***************************************/

    #define Wheel_Timer_STATUS         (*(reg8 *) Wheel_Timer_TimerHW__SR0 )
    /* In Fixed Function Block Status and Mask are the same register */
    #define Wheel_Timer_STATUS_MASK    (*(reg8 *) Wheel_Timer_TimerHW__SR0 )
    #define Wheel_Timer_CONTROL        (*(reg8 *) Wheel_Timer_TimerHW__CFG0)
    #define Wheel_Timer_CONTROL2       (*(reg8 *) Wheel_Timer_TimerHW__CFG1)
    #define Wheel_Timer_CONTROL2_PTR   ( (reg8 *) Wheel_Timer_TimerHW__CFG1)
    #define Wheel_Timer_RT1            (*(reg8 *) Wheel_Timer_TimerHW__RT1)
    #define Wheel_Timer_RT1_PTR        ( (reg8 *) Wheel_Timer_TimerHW__RT1)

    #if (CY_PSOC3 || CY_PSOC5LP)
        #define Wheel_Timer_CONTROL3       (*(reg8 *) Wheel_Timer_TimerHW__CFG2)
        #define Wheel_Timer_CONTROL3_PTR   ( (reg8 *) Wheel_Timer_TimerHW__CFG2)
    #endif /* (CY_PSOC3 || CY_PSOC5LP) */
    #define Wheel_Timer_GLOBAL_ENABLE  (*(reg8 *) Wheel_Timer_TimerHW__PM_ACT_CFG)
    #define Wheel_Timer_GLOBAL_STBY_ENABLE  (*(reg8 *) Wheel_Timer_TimerHW__PM_STBY_CFG)

    #define Wheel_Timer_CAPTURE_LSB         (* (reg16 *) Wheel_Timer_TimerHW__CAP0 )
    #define Wheel_Timer_CAPTURE_LSB_PTR       ((reg16 *) Wheel_Timer_TimerHW__CAP0 )
    #define Wheel_Timer_PERIOD_LSB          (* (reg16 *) Wheel_Timer_TimerHW__PER0 )
    #define Wheel_Timer_PERIOD_LSB_PTR        ((reg16 *) Wheel_Timer_TimerHW__PER0 )
    #define Wheel_Timer_COUNTER_LSB         (* (reg16 *) Wheel_Timer_TimerHW__CNT_CMP0 )
    #define Wheel_Timer_COUNTER_LSB_PTR       ((reg16 *) Wheel_Timer_TimerHW__CNT_CMP0 )


    /***************************************
    *    Register Constants
    ***************************************/

    /* Fixed Function Block Chosen */
    #define Wheel_Timer_BLOCK_EN_MASK                     Wheel_Timer_TimerHW__PM_ACT_MSK
    #define Wheel_Timer_BLOCK_STBY_EN_MASK                Wheel_Timer_TimerHW__PM_STBY_MSK

    /* Control Register Bit Locations */
    /* Interrupt Count - Not valid for Fixed Function Block */
    #define Wheel_Timer_CTRL_INTCNT_SHIFT                  0x00u
    /* Trigger Polarity - Not valid for Fixed Function Block */
    #define Wheel_Timer_CTRL_TRIG_MODE_SHIFT               0x00u
    /* Trigger Enable - Not valid for Fixed Function Block */
    #define Wheel_Timer_CTRL_TRIG_EN_SHIFT                 0x00u
    /* Capture Polarity - Not valid for Fixed Function Block */
    #define Wheel_Timer_CTRL_CAP_MODE_SHIFT                0x00u
    /* Timer Enable - As defined in Register Map, part of TMRX_CFG0 register */
    #define Wheel_Timer_CTRL_ENABLE_SHIFT                  0x00u

    /* Control Register Bit Masks */
    #define Wheel_Timer_CTRL_ENABLE                        ((uint8)((uint8)0x01u << Wheel_Timer_CTRL_ENABLE_SHIFT))

    /* Control2 Register Bit Masks */
    /* As defined in Register Map, Part of the TMRX_CFG1 register */
    #define Wheel_Timer_CTRL2_IRQ_SEL_SHIFT                 0x00u
    #define Wheel_Timer_CTRL2_IRQ_SEL                      ((uint8)((uint8)0x01u << Wheel_Timer_CTRL2_IRQ_SEL_SHIFT))

    #if (CY_PSOC5A)
        /* Use CFG1 Mode bits to set run mode */
        /* As defined by Verilog Implementation */
        #define Wheel_Timer_CTRL_MODE_SHIFT                 0x01u
        #define Wheel_Timer_CTRL_MODE_MASK                 ((uint8)((uint8)0x07u << Wheel_Timer_CTRL_MODE_SHIFT))
    #endif /* (CY_PSOC5A) */
    #if (CY_PSOC3 || CY_PSOC5LP)
        /* Control3 Register Bit Locations */
        #define Wheel_Timer_CTRL_RCOD_SHIFT        0x02u
        #define Wheel_Timer_CTRL_ENBL_SHIFT        0x00u
        #define Wheel_Timer_CTRL_MODE_SHIFT        0x00u

        /* Control3 Register Bit Masks */
        #define Wheel_Timer_CTRL_RCOD_MASK  ((uint8)((uint8)0x03u << Wheel_Timer_CTRL_RCOD_SHIFT)) /* ROD and COD bit masks */
        #define Wheel_Timer_CTRL_ENBL_MASK  ((uint8)((uint8)0x80u << Wheel_Timer_CTRL_ENBL_SHIFT)) /* HW_EN bit mask */
        #define Wheel_Timer_CTRL_MODE_MASK  ((uint8)((uint8)0x03u << Wheel_Timer_CTRL_MODE_SHIFT)) /* Run mode bit mask */

        #define Wheel_Timer_CTRL_RCOD       ((uint8)((uint8)0x03u << Wheel_Timer_CTRL_RCOD_SHIFT))
        #define Wheel_Timer_CTRL_ENBL       ((uint8)((uint8)0x80u << Wheel_Timer_CTRL_ENBL_SHIFT))
    #endif /* (CY_PSOC3 || CY_PSOC5LP) */

    /*RT1 Synch Constants: Applicable for PSoC3 and PSoC5LP */
    #define Wheel_Timer_RT1_SHIFT                       0x04u
    /* Sync TC and CMP bit masks */
    #define Wheel_Timer_RT1_MASK                        ((uint8)((uint8)0x03u << Wheel_Timer_RT1_SHIFT))
    #define Wheel_Timer_SYNC                            ((uint8)((uint8)0x03u << Wheel_Timer_RT1_SHIFT))
    #define Wheel_Timer_SYNCDSI_SHIFT                   0x00u
    /* Sync all DSI inputs with Mask  */
    #define Wheel_Timer_SYNCDSI_MASK                    ((uint8)((uint8)0x0Fu << Wheel_Timer_SYNCDSI_SHIFT))
    /* Sync all DSI inputs */
    #define Wheel_Timer_SYNCDSI_EN                      ((uint8)((uint8)0x0Fu << Wheel_Timer_SYNCDSI_SHIFT))

    #define Wheel_Timer_CTRL_MODE_PULSEWIDTH            ((uint8)((uint8)0x01u << Wheel_Timer_CTRL_MODE_SHIFT))
    #define Wheel_Timer_CTRL_MODE_PERIOD                ((uint8)((uint8)0x02u << Wheel_Timer_CTRL_MODE_SHIFT))
    #define Wheel_Timer_CTRL_MODE_CONTINUOUS            ((uint8)((uint8)0x00u << Wheel_Timer_CTRL_MODE_SHIFT))

    /* Status Register Bit Locations */
    /* As defined in Register Map, part of TMRX_SR0 register */
    #define Wheel_Timer_STATUS_TC_SHIFT                 0x07u
    /* As defined in Register Map, part of TMRX_SR0 register, Shared with Compare Status */
    #define Wheel_Timer_STATUS_CAPTURE_SHIFT            0x06u
    /* As defined in Register Map, part of TMRX_SR0 register */
    #define Wheel_Timer_STATUS_TC_INT_MASK_SHIFT        (Wheel_Timer_STATUS_TC_SHIFT - 0x04u)
    /* As defined in Register Map, part of TMRX_SR0 register, Shared with Compare Status */
    #define Wheel_Timer_STATUS_CAPTURE_INT_MASK_SHIFT   (Wheel_Timer_STATUS_CAPTURE_SHIFT - 0x04u)

    /* Status Register Bit Masks */
    #define Wheel_Timer_STATUS_TC                       ((uint8)((uint8)0x01u << Wheel_Timer_STATUS_TC_SHIFT))
    #define Wheel_Timer_STATUS_CAPTURE                  ((uint8)((uint8)0x01u << Wheel_Timer_STATUS_CAPTURE_SHIFT))
    /* Interrupt Enable Bit-Mask for interrupt on TC */
    #define Wheel_Timer_STATUS_TC_INT_MASK              ((uint8)((uint8)0x01u << Wheel_Timer_STATUS_TC_INT_MASK_SHIFT))
    /* Interrupt Enable Bit-Mask for interrupt on Capture */
    #define Wheel_Timer_STATUS_CAPTURE_INT_MASK         ((uint8)((uint8)0x01u << Wheel_Timer_STATUS_CAPTURE_INT_MASK_SHIFT))

#else   /* UDB Registers and Register Constants */


    /***************************************
    *           UDB Registers
    ***************************************/

    #define Wheel_Timer_STATUS              (* (reg8 *) Wheel_Timer_TimerUDB_rstSts_stsreg__STATUS_REG )
    #define Wheel_Timer_STATUS_MASK         (* (reg8 *) Wheel_Timer_TimerUDB_rstSts_stsreg__MASK_REG)
    #define Wheel_Timer_STATUS_AUX_CTRL     (* (reg8 *) Wheel_Timer_TimerUDB_rstSts_stsreg__STATUS_AUX_CTL_REG)
    #define Wheel_Timer_CONTROL             (* (reg8 *) Wheel_Timer_TimerUDB_sCTRLReg_SyncCtl_ctrlreg__CONTROL_REG )
    
    #if(Wheel_Timer_Resolution <= 8u) /* 8-bit Timer */
        #define Wheel_Timer_CAPTURE_LSB         (* (reg8 *) Wheel_Timer_TimerUDB_sT32_timerdp_u0__F0_REG )
        #define Wheel_Timer_CAPTURE_LSB_PTR       ((reg8 *) Wheel_Timer_TimerUDB_sT32_timerdp_u0__F0_REG )
        #define Wheel_Timer_PERIOD_LSB          (* (reg8 *) Wheel_Timer_TimerUDB_sT32_timerdp_u0__D0_REG )
        #define Wheel_Timer_PERIOD_LSB_PTR        ((reg8 *) Wheel_Timer_TimerUDB_sT32_timerdp_u0__D0_REG )
        #define Wheel_Timer_COUNTER_LSB         (* (reg8 *) Wheel_Timer_TimerUDB_sT32_timerdp_u0__A0_REG )
        #define Wheel_Timer_COUNTER_LSB_PTR       ((reg8 *) Wheel_Timer_TimerUDB_sT32_timerdp_u0__A0_REG )
    #elif(Wheel_Timer_Resolution <= 16u) /* 8-bit Timer */
        #if(CY_PSOC3) /* 8-bit addres space */
            #define Wheel_Timer_CAPTURE_LSB         (* (reg16 *) Wheel_Timer_TimerUDB_sT32_timerdp_u0__F0_REG )
            #define Wheel_Timer_CAPTURE_LSB_PTR       ((reg16 *) Wheel_Timer_TimerUDB_sT32_timerdp_u0__F0_REG )
            #define Wheel_Timer_PERIOD_LSB          (* (reg16 *) Wheel_Timer_TimerUDB_sT32_timerdp_u0__D0_REG )
            #define Wheel_Timer_PERIOD_LSB_PTR        ((reg16 *) Wheel_Timer_TimerUDB_sT32_timerdp_u0__D0_REG )
            #define Wheel_Timer_COUNTER_LSB         (* (reg16 *) Wheel_Timer_TimerUDB_sT32_timerdp_u0__A0_REG )
            #define Wheel_Timer_COUNTER_LSB_PTR       ((reg16 *) Wheel_Timer_TimerUDB_sT32_timerdp_u0__A0_REG )
        #else /* 16-bit address space */
            #define Wheel_Timer_CAPTURE_LSB         (* (reg16 *) Wheel_Timer_TimerUDB_sT32_timerdp_u0__16BIT_F0_REG )
            #define Wheel_Timer_CAPTURE_LSB_PTR       ((reg16 *) Wheel_Timer_TimerUDB_sT32_timerdp_u0__16BIT_F0_REG )
            #define Wheel_Timer_PERIOD_LSB          (* (reg16 *) Wheel_Timer_TimerUDB_sT32_timerdp_u0__16BIT_D0_REG )
            #define Wheel_Timer_PERIOD_LSB_PTR        ((reg16 *) Wheel_Timer_TimerUDB_sT32_timerdp_u0__16BIT_D0_REG )
            #define Wheel_Timer_COUNTER_LSB         (* (reg16 *) Wheel_Timer_TimerUDB_sT32_timerdp_u0__16BIT_A0_REG )
            #define Wheel_Timer_COUNTER_LSB_PTR       ((reg16 *) Wheel_Timer_TimerUDB_sT32_timerdp_u0__16BIT_A0_REG )
        #endif /* CY_PSOC3 */
    #elif(Wheel_Timer_Resolution <= 24u)/* 24-bit Timer */
        #define Wheel_Timer_CAPTURE_LSB         (* (reg32 *) Wheel_Timer_TimerUDB_sT32_timerdp_u0__F0_REG )
        #define Wheel_Timer_CAPTURE_LSB_PTR       ((reg32 *) Wheel_Timer_TimerUDB_sT32_timerdp_u0__F0_REG )
        #define Wheel_Timer_PERIOD_LSB          (* (reg32 *) Wheel_Timer_TimerUDB_sT32_timerdp_u0__D0_REG )
        #define Wheel_Timer_PERIOD_LSB_PTR        ((reg32 *) Wheel_Timer_TimerUDB_sT32_timerdp_u0__D0_REG )
        #define Wheel_Timer_COUNTER_LSB         (* (reg32 *) Wheel_Timer_TimerUDB_sT32_timerdp_u0__A0_REG )
        #define Wheel_Timer_COUNTER_LSB_PTR       ((reg32 *) Wheel_Timer_TimerUDB_sT32_timerdp_u0__A0_REG )
    #else /* 32-bit Timer */
        #if(CY_PSOC3 || CY_PSOC5) /* 8-bit address space */
            #define Wheel_Timer_CAPTURE_LSB         (* (reg32 *) Wheel_Timer_TimerUDB_sT32_timerdp_u0__F0_REG )
            #define Wheel_Timer_CAPTURE_LSB_PTR       ((reg32 *) Wheel_Timer_TimerUDB_sT32_timerdp_u0__F0_REG )
            #define Wheel_Timer_PERIOD_LSB          (* (reg32 *) Wheel_Timer_TimerUDB_sT32_timerdp_u0__D0_REG )
            #define Wheel_Timer_PERIOD_LSB_PTR        ((reg32 *) Wheel_Timer_TimerUDB_sT32_timerdp_u0__D0_REG )
            #define Wheel_Timer_COUNTER_LSB         (* (reg32 *) Wheel_Timer_TimerUDB_sT32_timerdp_u0__A0_REG )
            #define Wheel_Timer_COUNTER_LSB_PTR       ((reg32 *) Wheel_Timer_TimerUDB_sT32_timerdp_u0__A0_REG )
        #else /* 32-bit address space */
            #define Wheel_Timer_CAPTURE_LSB         (* (reg32 *) Wheel_Timer_TimerUDB_sT32_timerdp_u0__32BIT_F0_REG )
            #define Wheel_Timer_CAPTURE_LSB_PTR       ((reg32 *) Wheel_Timer_TimerUDB_sT32_timerdp_u0__32BIT_F0_REG )
            #define Wheel_Timer_PERIOD_LSB          (* (reg32 *) Wheel_Timer_TimerUDB_sT32_timerdp_u0__32BIT_D0_REG )
            #define Wheel_Timer_PERIOD_LSB_PTR        ((reg32 *) Wheel_Timer_TimerUDB_sT32_timerdp_u0__32BIT_D0_REG )
            #define Wheel_Timer_COUNTER_LSB         (* (reg32 *) Wheel_Timer_TimerUDB_sT32_timerdp_u0__32BIT_A0_REG )
            #define Wheel_Timer_COUNTER_LSB_PTR       ((reg32 *) Wheel_Timer_TimerUDB_sT32_timerdp_u0__32BIT_A0_REG )
        #endif /* CY_PSOC3 || CY_PSOC5 */ 
    #endif

    #define Wheel_Timer_COUNTER_LSB_PTR_8BIT       ((reg8 *) Wheel_Timer_TimerUDB_sT32_timerdp_u0__A0_REG )
    
    #if (Wheel_Timer_UsingHWCaptureCounter)
        #define Wheel_Timer_CAP_COUNT              (*(reg8 *) Wheel_Timer_TimerUDB_sCapCount_counter__PERIOD_REG )
        #define Wheel_Timer_CAP_COUNT_PTR          ( (reg8 *) Wheel_Timer_TimerUDB_sCapCount_counter__PERIOD_REG )
        #define Wheel_Timer_CAPTURE_COUNT_CTRL     (*(reg8 *) Wheel_Timer_TimerUDB_sCapCount_counter__CONTROL_AUX_CTL_REG )
        #define Wheel_Timer_CAPTURE_COUNT_CTRL_PTR ( (reg8 *) Wheel_Timer_TimerUDB_sCapCount_counter__CONTROL_AUX_CTL_REG )
    #endif /* (Wheel_Timer_UsingHWCaptureCounter) */


    /***************************************
    *       Register Constants
    ***************************************/

    /* Control Register Bit Locations */
    #define Wheel_Timer_CTRL_INTCNT_SHIFT              0x00u       /* As defined by Verilog Implementation */
    #define Wheel_Timer_CTRL_TRIG_MODE_SHIFT           0x02u       /* As defined by Verilog Implementation */
    #define Wheel_Timer_CTRL_TRIG_EN_SHIFT             0x04u       /* As defined by Verilog Implementation */
    #define Wheel_Timer_CTRL_CAP_MODE_SHIFT            0x05u       /* As defined by Verilog Implementation */
    #define Wheel_Timer_CTRL_ENABLE_SHIFT              0x07u       /* As defined by Verilog Implementation */

    /* Control Register Bit Masks */
    #define Wheel_Timer_CTRL_INTCNT_MASK               ((uint8)((uint8)0x03u << Wheel_Timer_CTRL_INTCNT_SHIFT))
    #define Wheel_Timer_CTRL_TRIG_MODE_MASK            ((uint8)((uint8)0x03u << Wheel_Timer_CTRL_TRIG_MODE_SHIFT))
    #define Wheel_Timer_CTRL_TRIG_EN                   ((uint8)((uint8)0x01u << Wheel_Timer_CTRL_TRIG_EN_SHIFT))
    #define Wheel_Timer_CTRL_CAP_MODE_MASK             ((uint8)((uint8)0x03u << Wheel_Timer_CTRL_CAP_MODE_SHIFT))
    #define Wheel_Timer_CTRL_ENABLE                    ((uint8)((uint8)0x01u << Wheel_Timer_CTRL_ENABLE_SHIFT))

    /* Bit Counter (7-bit) Control Register Bit Definitions */
    /* As defined by the Register map for the AUX Control Register */
    #define Wheel_Timer_CNTR_ENABLE                    0x20u

    /* Status Register Bit Locations */
    #define Wheel_Timer_STATUS_TC_SHIFT                0x00u  /* As defined by Verilog Implementation */
    #define Wheel_Timer_STATUS_CAPTURE_SHIFT           0x01u  /* As defined by Verilog Implementation */
    #define Wheel_Timer_STATUS_TC_INT_MASK_SHIFT       Wheel_Timer_STATUS_TC_SHIFT
    #define Wheel_Timer_STATUS_CAPTURE_INT_MASK_SHIFT  Wheel_Timer_STATUS_CAPTURE_SHIFT
    #define Wheel_Timer_STATUS_FIFOFULL_SHIFT          0x02u  /* As defined by Verilog Implementation */
    #define Wheel_Timer_STATUS_FIFONEMP_SHIFT          0x03u  /* As defined by Verilog Implementation */
    #define Wheel_Timer_STATUS_FIFOFULL_INT_MASK_SHIFT Wheel_Timer_STATUS_FIFOFULL_SHIFT

    /* Status Register Bit Masks */
    /* Sticky TC Event Bit-Mask */
    #define Wheel_Timer_STATUS_TC                      ((uint8)((uint8)0x01u << Wheel_Timer_STATUS_TC_SHIFT))
    /* Sticky Capture Event Bit-Mask */
    #define Wheel_Timer_STATUS_CAPTURE                 ((uint8)((uint8)0x01u << Wheel_Timer_STATUS_CAPTURE_SHIFT))
    /* Interrupt Enable Bit-Mask */
    #define Wheel_Timer_STATUS_TC_INT_MASK             ((uint8)((uint8)0x01u << Wheel_Timer_STATUS_TC_SHIFT))
    /* Interrupt Enable Bit-Mask */
    #define Wheel_Timer_STATUS_CAPTURE_INT_MASK        ((uint8)((uint8)0x01u << Wheel_Timer_STATUS_CAPTURE_SHIFT))
    /* NOT-Sticky FIFO Full Bit-Mask */
    #define Wheel_Timer_STATUS_FIFOFULL                ((uint8)((uint8)0x01u << Wheel_Timer_STATUS_FIFOFULL_SHIFT))
    /* NOT-Sticky FIFO Not Empty Bit-Mask */
    #define Wheel_Timer_STATUS_FIFONEMP                ((uint8)((uint8)0x01u << Wheel_Timer_STATUS_FIFONEMP_SHIFT))
    /* Interrupt Enable Bit-Mask */
    #define Wheel_Timer_STATUS_FIFOFULL_INT_MASK       ((uint8)((uint8)0x01u << Wheel_Timer_STATUS_FIFOFULL_SHIFT))

    #define Wheel_Timer_STATUS_ACTL_INT_EN             0x10u   /* As defined for the ACTL Register */

    /* Datapath Auxillary Control Register definitions */
    #define Wheel_Timer_AUX_CTRL_FIFO0_CLR             0x01u   /* As defined by Register map */
    #define Wheel_Timer_AUX_CTRL_FIFO1_CLR             0x02u   /* As defined by Register map */
    #define Wheel_Timer_AUX_CTRL_FIFO0_LVL             0x04u   /* As defined by Register map */
    #define Wheel_Timer_AUX_CTRL_FIFO1_LVL             0x08u   /* As defined by Register map */
    #define Wheel_Timer_STATUS_ACTL_INT_EN_MASK        0x10u   /* As defined for the ACTL Register */

#endif /* Implementation Specific Registers and Register Constants */

#endif  /* CY_TIMER_Wheel_Timer_H */


/* [] END OF FILE */
