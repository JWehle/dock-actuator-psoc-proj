/*******************************************************************************
* File Name: Encoder_3_PM.c  
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

#include "Encoder_3.h"

static Encoder_3_backupStruct Encoder_3_backup;


/*******************************************************************************
* Function Name: Encoder_3_SaveConfig
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
*  Encoder_3_backup:  Variables of this global structure are modified to 
*  store the values of non retention configuration registers when Sleep() API is 
*  called.
*
*******************************************************************************/
void Encoder_3_SaveConfig(void) 
{
    #if (!Encoder_3_UsingFixedFunction)

        Encoder_3_backup.CounterUdb = Encoder_3_ReadCounter();

        #if(!Encoder_3_ControlRegRemoved)
            Encoder_3_backup.CounterControlRegister = Encoder_3_ReadControlRegister();
        #endif /* (!Encoder_3_ControlRegRemoved) */

    #endif /* (!Encoder_3_UsingFixedFunction) */
}


/*******************************************************************************
* Function Name: Encoder_3_RestoreConfig
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
*  Encoder_3_backup:  Variables of this global structure are used to 
*  restore the values of non retention registers on wakeup from sleep mode.
*
*******************************************************************************/
void Encoder_3_RestoreConfig(void) 
{      
    #if (!Encoder_3_UsingFixedFunction)

       Encoder_3_WriteCounter(Encoder_3_backup.CounterUdb);

        #if(!Encoder_3_ControlRegRemoved)
            Encoder_3_WriteControlRegister(Encoder_3_backup.CounterControlRegister);
        #endif /* (!Encoder_3_ControlRegRemoved) */

    #endif /* (!Encoder_3_UsingFixedFunction) */
}


/*******************************************************************************
* Function Name: Encoder_3_Sleep
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
*  Encoder_3_backup.enableState:  Is modified depending on the enable 
*  state of the block before entering sleep mode.
*
*******************************************************************************/
void Encoder_3_Sleep(void) 
{
    #if(!Encoder_3_ControlRegRemoved)
        /* Save Counter's enable state */
        if(Encoder_3_CTRL_ENABLE == (Encoder_3_CONTROL & Encoder_3_CTRL_ENABLE))
        {
            /* Counter is enabled */
            Encoder_3_backup.CounterEnableState = 1u;
        }
        else
        {
            /* Counter is disabled */
            Encoder_3_backup.CounterEnableState = 0u;
        }
    #else
        Encoder_3_backup.CounterEnableState = 1u;
        if(Encoder_3_backup.CounterEnableState != 0u)
        {
            Encoder_3_backup.CounterEnableState = 0u;
        }
    #endif /* (!Encoder_3_ControlRegRemoved) */
    
    Encoder_3_Stop();
    Encoder_3_SaveConfig();
}


/*******************************************************************************
* Function Name: Encoder_3_Wakeup
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
*  Encoder_3_backup.enableState:  Is used to restore the enable state of 
*  block on wakeup from sleep mode.
*
*******************************************************************************/
void Encoder_3_Wakeup(void) 
{
    Encoder_3_RestoreConfig();
    #if(!Encoder_3_ControlRegRemoved)
        if(Encoder_3_backup.CounterEnableState == 1u)
        {
            /* Enable Counter's operation */
            Encoder_3_Enable();
        } /* Do nothing if Counter was disabled before */    
    #endif /* (!Encoder_3_ControlRegRemoved) */
    
}


/* [] END OF FILE */
