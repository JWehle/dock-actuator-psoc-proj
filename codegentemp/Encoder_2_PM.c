/*******************************************************************************
* File Name: Encoder_2_PM.c  
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

#include "Encoder_2.h"

static Encoder_2_backupStruct Encoder_2_backup;


/*******************************************************************************
* Function Name: Encoder_2_SaveConfig
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
*  Encoder_2_backup:  Variables of this global structure are modified to 
*  store the values of non retention configuration registers when Sleep() API is 
*  called.
*
*******************************************************************************/
void Encoder_2_SaveConfig(void) 
{
    #if (!Encoder_2_UsingFixedFunction)

        Encoder_2_backup.CounterUdb = Encoder_2_ReadCounter();

        #if(!Encoder_2_ControlRegRemoved)
            Encoder_2_backup.CounterControlRegister = Encoder_2_ReadControlRegister();
        #endif /* (!Encoder_2_ControlRegRemoved) */

    #endif /* (!Encoder_2_UsingFixedFunction) */
}


/*******************************************************************************
* Function Name: Encoder_2_RestoreConfig
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
*  Encoder_2_backup:  Variables of this global structure are used to 
*  restore the values of non retention registers on wakeup from sleep mode.
*
*******************************************************************************/
void Encoder_2_RestoreConfig(void) 
{      
    #if (!Encoder_2_UsingFixedFunction)

       Encoder_2_WriteCounter(Encoder_2_backup.CounterUdb);

        #if(!Encoder_2_ControlRegRemoved)
            Encoder_2_WriteControlRegister(Encoder_2_backup.CounterControlRegister);
        #endif /* (!Encoder_2_ControlRegRemoved) */

    #endif /* (!Encoder_2_UsingFixedFunction) */
}


/*******************************************************************************
* Function Name: Encoder_2_Sleep
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
*  Encoder_2_backup.enableState:  Is modified depending on the enable 
*  state of the block before entering sleep mode.
*
*******************************************************************************/
void Encoder_2_Sleep(void) 
{
    #if(!Encoder_2_ControlRegRemoved)
        /* Save Counter's enable state */
        if(Encoder_2_CTRL_ENABLE == (Encoder_2_CONTROL & Encoder_2_CTRL_ENABLE))
        {
            /* Counter is enabled */
            Encoder_2_backup.CounterEnableState = 1u;
        }
        else
        {
            /* Counter is disabled */
            Encoder_2_backup.CounterEnableState = 0u;
        }
    #else
        Encoder_2_backup.CounterEnableState = 1u;
        if(Encoder_2_backup.CounterEnableState != 0u)
        {
            Encoder_2_backup.CounterEnableState = 0u;
        }
    #endif /* (!Encoder_2_ControlRegRemoved) */
    
    Encoder_2_Stop();
    Encoder_2_SaveConfig();
}


/*******************************************************************************
* Function Name: Encoder_2_Wakeup
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
*  Encoder_2_backup.enableState:  Is used to restore the enable state of 
*  block on wakeup from sleep mode.
*
*******************************************************************************/
void Encoder_2_Wakeup(void) 
{
    Encoder_2_RestoreConfig();
    #if(!Encoder_2_ControlRegRemoved)
        if(Encoder_2_backup.CounterEnableState == 1u)
        {
            /* Enable Counter's operation */
            Encoder_2_Enable();
        } /* Do nothing if Counter was disabled before */    
    #endif /* (!Encoder_2_ControlRegRemoved) */
    
}


/* [] END OF FILE */
