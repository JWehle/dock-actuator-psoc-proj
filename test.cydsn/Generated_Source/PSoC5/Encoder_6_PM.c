/*******************************************************************************
* File Name: Encoder_6_PM.c  
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

#include "Encoder_6.h"

static Encoder_6_backupStruct Encoder_6_backup;


/*******************************************************************************
* Function Name: Encoder_6_SaveConfig
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
*  Encoder_6_backup:  Variables of this global structure are modified to 
*  store the values of non retention configuration registers when Sleep() API is 
*  called.
*
*******************************************************************************/
void Encoder_6_SaveConfig(void) 
{
    #if (!Encoder_6_UsingFixedFunction)

        Encoder_6_backup.CounterUdb = Encoder_6_ReadCounter();

        #if(!Encoder_6_ControlRegRemoved)
            Encoder_6_backup.CounterControlRegister = Encoder_6_ReadControlRegister();
        #endif /* (!Encoder_6_ControlRegRemoved) */

    #endif /* (!Encoder_6_UsingFixedFunction) */
}


/*******************************************************************************
* Function Name: Encoder_6_RestoreConfig
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
*  Encoder_6_backup:  Variables of this global structure are used to 
*  restore the values of non retention registers on wakeup from sleep mode.
*
*******************************************************************************/
void Encoder_6_RestoreConfig(void) 
{      
    #if (!Encoder_6_UsingFixedFunction)

       Encoder_6_WriteCounter(Encoder_6_backup.CounterUdb);

        #if(!Encoder_6_ControlRegRemoved)
            Encoder_6_WriteControlRegister(Encoder_6_backup.CounterControlRegister);
        #endif /* (!Encoder_6_ControlRegRemoved) */

    #endif /* (!Encoder_6_UsingFixedFunction) */
}


/*******************************************************************************
* Function Name: Encoder_6_Sleep
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
*  Encoder_6_backup.enableState:  Is modified depending on the enable 
*  state of the block before entering sleep mode.
*
*******************************************************************************/
void Encoder_6_Sleep(void) 
{
    #if(!Encoder_6_ControlRegRemoved)
        /* Save Counter's enable state */
        if(Encoder_6_CTRL_ENABLE == (Encoder_6_CONTROL & Encoder_6_CTRL_ENABLE))
        {
            /* Counter is enabled */
            Encoder_6_backup.CounterEnableState = 1u;
        }
        else
        {
            /* Counter is disabled */
            Encoder_6_backup.CounterEnableState = 0u;
        }
    #else
        Encoder_6_backup.CounterEnableState = 1u;
        if(Encoder_6_backup.CounterEnableState != 0u)
        {
            Encoder_6_backup.CounterEnableState = 0u;
        }
    #endif /* (!Encoder_6_ControlRegRemoved) */
    
    Encoder_6_Stop();
    Encoder_6_SaveConfig();
}


/*******************************************************************************
* Function Name: Encoder_6_Wakeup
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
*  Encoder_6_backup.enableState:  Is used to restore the enable state of 
*  block on wakeup from sleep mode.
*
*******************************************************************************/
void Encoder_6_Wakeup(void) 
{
    Encoder_6_RestoreConfig();
    #if(!Encoder_6_ControlRegRemoved)
        if(Encoder_6_backup.CounterEnableState == 1u)
        {
            /* Enable Counter's operation */
            Encoder_6_Enable();
        } /* Do nothing if Counter was disabled before */    
    #endif /* (!Encoder_6_ControlRegRemoved) */
    
}


/* [] END OF FILE */
