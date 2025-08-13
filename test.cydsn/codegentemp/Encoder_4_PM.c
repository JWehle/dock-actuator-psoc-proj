/*******************************************************************************
* File Name: Encoder_4_PM.c  
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

#include "Encoder_4.h"

static Encoder_4_backupStruct Encoder_4_backup;


/*******************************************************************************
* Function Name: Encoder_4_SaveConfig
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
*  Encoder_4_backup:  Variables of this global structure are modified to 
*  store the values of non retention configuration registers when Sleep() API is 
*  called.
*
*******************************************************************************/
void Encoder_4_SaveConfig(void) 
{
    #if (!Encoder_4_UsingFixedFunction)

        Encoder_4_backup.CounterUdb = Encoder_4_ReadCounter();

        #if(!Encoder_4_ControlRegRemoved)
            Encoder_4_backup.CounterControlRegister = Encoder_4_ReadControlRegister();
        #endif /* (!Encoder_4_ControlRegRemoved) */

    #endif /* (!Encoder_4_UsingFixedFunction) */
}


/*******************************************************************************
* Function Name: Encoder_4_RestoreConfig
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
*  Encoder_4_backup:  Variables of this global structure are used to 
*  restore the values of non retention registers on wakeup from sleep mode.
*
*******************************************************************************/
void Encoder_4_RestoreConfig(void) 
{      
    #if (!Encoder_4_UsingFixedFunction)

       Encoder_4_WriteCounter(Encoder_4_backup.CounterUdb);

        #if(!Encoder_4_ControlRegRemoved)
            Encoder_4_WriteControlRegister(Encoder_4_backup.CounterControlRegister);
        #endif /* (!Encoder_4_ControlRegRemoved) */

    #endif /* (!Encoder_4_UsingFixedFunction) */
}


/*******************************************************************************
* Function Name: Encoder_4_Sleep
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
*  Encoder_4_backup.enableState:  Is modified depending on the enable 
*  state of the block before entering sleep mode.
*
*******************************************************************************/
void Encoder_4_Sleep(void) 
{
    #if(!Encoder_4_ControlRegRemoved)
        /* Save Counter's enable state */
        if(Encoder_4_CTRL_ENABLE == (Encoder_4_CONTROL & Encoder_4_CTRL_ENABLE))
        {
            /* Counter is enabled */
            Encoder_4_backup.CounterEnableState = 1u;
        }
        else
        {
            /* Counter is disabled */
            Encoder_4_backup.CounterEnableState = 0u;
        }
    #else
        Encoder_4_backup.CounterEnableState = 1u;
        if(Encoder_4_backup.CounterEnableState != 0u)
        {
            Encoder_4_backup.CounterEnableState = 0u;
        }
    #endif /* (!Encoder_4_ControlRegRemoved) */
    
    Encoder_4_Stop();
    Encoder_4_SaveConfig();
}


/*******************************************************************************
* Function Name: Encoder_4_Wakeup
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
*  Encoder_4_backup.enableState:  Is used to restore the enable state of 
*  block on wakeup from sleep mode.
*
*******************************************************************************/
void Encoder_4_Wakeup(void) 
{
    Encoder_4_RestoreConfig();
    #if(!Encoder_4_ControlRegRemoved)
        if(Encoder_4_backup.CounterEnableState == 1u)
        {
            /* Enable Counter's operation */
            Encoder_4_Enable();
        } /* Do nothing if Counter was disabled before */    
    #endif /* (!Encoder_4_ControlRegRemoved) */
    
}


/* [] END OF FILE */
