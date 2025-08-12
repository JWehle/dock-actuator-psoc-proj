/*******************************************************************************
* File Name: Encoder_5_PM.c  
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

#include "Encoder_5.h"

static Encoder_5_backupStruct Encoder_5_backup;


/*******************************************************************************
* Function Name: Encoder_5_SaveConfig
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
*  Encoder_5_backup:  Variables of this global structure are modified to 
*  store the values of non retention configuration registers when Sleep() API is 
*  called.
*
*******************************************************************************/
void Encoder_5_SaveConfig(void) 
{
    #if (!Encoder_5_UsingFixedFunction)

        Encoder_5_backup.CounterUdb = Encoder_5_ReadCounter();

        #if(!Encoder_5_ControlRegRemoved)
            Encoder_5_backup.CounterControlRegister = Encoder_5_ReadControlRegister();
        #endif /* (!Encoder_5_ControlRegRemoved) */

    #endif /* (!Encoder_5_UsingFixedFunction) */
}


/*******************************************************************************
* Function Name: Encoder_5_RestoreConfig
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
*  Encoder_5_backup:  Variables of this global structure are used to 
*  restore the values of non retention registers on wakeup from sleep mode.
*
*******************************************************************************/
void Encoder_5_RestoreConfig(void) 
{      
    #if (!Encoder_5_UsingFixedFunction)

       Encoder_5_WriteCounter(Encoder_5_backup.CounterUdb);

        #if(!Encoder_5_ControlRegRemoved)
            Encoder_5_WriteControlRegister(Encoder_5_backup.CounterControlRegister);
        #endif /* (!Encoder_5_ControlRegRemoved) */

    #endif /* (!Encoder_5_UsingFixedFunction) */
}


/*******************************************************************************
* Function Name: Encoder_5_Sleep
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
*  Encoder_5_backup.enableState:  Is modified depending on the enable 
*  state of the block before entering sleep mode.
*
*******************************************************************************/
void Encoder_5_Sleep(void) 
{
    #if(!Encoder_5_ControlRegRemoved)
        /* Save Counter's enable state */
        if(Encoder_5_CTRL_ENABLE == (Encoder_5_CONTROL & Encoder_5_CTRL_ENABLE))
        {
            /* Counter is enabled */
            Encoder_5_backup.CounterEnableState = 1u;
        }
        else
        {
            /* Counter is disabled */
            Encoder_5_backup.CounterEnableState = 0u;
        }
    #else
        Encoder_5_backup.CounterEnableState = 1u;
        if(Encoder_5_backup.CounterEnableState != 0u)
        {
            Encoder_5_backup.CounterEnableState = 0u;
        }
    #endif /* (!Encoder_5_ControlRegRemoved) */
    
    Encoder_5_Stop();
    Encoder_5_SaveConfig();
}


/*******************************************************************************
* Function Name: Encoder_5_Wakeup
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
*  Encoder_5_backup.enableState:  Is used to restore the enable state of 
*  block on wakeup from sleep mode.
*
*******************************************************************************/
void Encoder_5_Wakeup(void) 
{
    Encoder_5_RestoreConfig();
    #if(!Encoder_5_ControlRegRemoved)
        if(Encoder_5_backup.CounterEnableState == 1u)
        {
            /* Enable Counter's operation */
            Encoder_5_Enable();
        } /* Do nothing if Counter was disabled before */    
    #endif /* (!Encoder_5_ControlRegRemoved) */
    
}


/* [] END OF FILE */
