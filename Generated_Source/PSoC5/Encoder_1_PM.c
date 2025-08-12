/*******************************************************************************
* File Name: Encoder_1_PM.c  
* Version 3.0
*
*  Description:
*    This file provides the power management source code to API for the
*    Counter.  
*
*   Note:
*     None
*
********************************************************************************
* Copyright 2008-2012, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/

#include "Encoder_1.h"

static Encoder_1_backupStruct Encoder_1_backup;


/*******************************************************************************
* Function Name: Encoder_1_SaveConfig
********************************************************************************
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
*  Encoder_1_backup:  Variables of this global structure are modified to 
*  store the values of non retention configuration registers when Sleep() API is 
*  called.
*
*******************************************************************************/
void Encoder_1_SaveConfig(void) 
{
    #if (!Encoder_1_UsingFixedFunction)

        Encoder_1_backup.CounterUdb = Encoder_1_ReadCounter();

        #if(!Encoder_1_ControlRegRemoved)
            Encoder_1_backup.CounterControlRegister = Encoder_1_ReadControlRegister();
        #endif /* (!Encoder_1_ControlRegRemoved) */

    #endif /* (!Encoder_1_UsingFixedFunction) */
}


/*******************************************************************************
* Function Name: Encoder_1_RestoreConfig
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
*  Encoder_1_backup:  Variables of this global structure are used to 
*  restore the values of non retention registers on wakeup from sleep mode.
*
*******************************************************************************/
void Encoder_1_RestoreConfig(void) 
{      
    #if (!Encoder_1_UsingFixedFunction)

       Encoder_1_WriteCounter(Encoder_1_backup.CounterUdb);

        #if(!Encoder_1_ControlRegRemoved)
            Encoder_1_WriteControlRegister(Encoder_1_backup.CounterControlRegister);
        #endif /* (!Encoder_1_ControlRegRemoved) */

    #endif /* (!Encoder_1_UsingFixedFunction) */
}


/*******************************************************************************
* Function Name: Encoder_1_Sleep
********************************************************************************
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
*  Encoder_1_backup.enableState:  Is modified depending on the enable 
*  state of the block before entering sleep mode.
*
*******************************************************************************/
void Encoder_1_Sleep(void) 
{
    #if(!Encoder_1_ControlRegRemoved)
        /* Save Counter's enable state */
        if(Encoder_1_CTRL_ENABLE == (Encoder_1_CONTROL & Encoder_1_CTRL_ENABLE))
        {
            /* Counter is enabled */
            Encoder_1_backup.CounterEnableState = 1u;
        }
        else
        {
            /* Counter is disabled */
            Encoder_1_backup.CounterEnableState = 0u;
        }
    #else
        Encoder_1_backup.CounterEnableState = 1u;
        if(Encoder_1_backup.CounterEnableState != 0u)
        {
            Encoder_1_backup.CounterEnableState = 0u;
        }
    #endif /* (!Encoder_1_ControlRegRemoved) */
    
    Encoder_1_Stop();
    Encoder_1_SaveConfig();
}


/*******************************************************************************
* Function Name: Encoder_1_Wakeup
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
*  Encoder_1_backup.enableState:  Is used to restore the enable state of 
*  block on wakeup from sleep mode.
*
*******************************************************************************/
void Encoder_1_Wakeup(void) 
{
    Encoder_1_RestoreConfig();
    #if(!Encoder_1_ControlRegRemoved)
        if(Encoder_1_backup.CounterEnableState == 1u)
        {
            /* Enable Counter's operation */
            Encoder_1_Enable();
        } /* Do nothing if Counter was disabled before */    
    #endif /* (!Encoder_1_ControlRegRemoved) */
    
}


/* [] END OF FILE */
