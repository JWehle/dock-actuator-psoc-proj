/*******************************************************************************
* File Name: Encoder_7_PM.c  
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

#include "Encoder_7.h"

static Encoder_7_backupStruct Encoder_7_backup;


/*******************************************************************************
* Function Name: Encoder_7_SaveConfig
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
*  Encoder_7_backup:  Variables of this global structure are modified to 
*  store the values of non retention configuration registers when Sleep() API is 
*  called.
*
*******************************************************************************/
void Encoder_7_SaveConfig(void) 
{
    #if (!Encoder_7_UsingFixedFunction)

        Encoder_7_backup.CounterUdb = Encoder_7_ReadCounter();

        #if(!Encoder_7_ControlRegRemoved)
            Encoder_7_backup.CounterControlRegister = Encoder_7_ReadControlRegister();
        #endif /* (!Encoder_7_ControlRegRemoved) */

    #endif /* (!Encoder_7_UsingFixedFunction) */
}


/*******************************************************************************
* Function Name: Encoder_7_RestoreConfig
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
*  Encoder_7_backup:  Variables of this global structure are used to 
*  restore the values of non retention registers on wakeup from sleep mode.
*
*******************************************************************************/
void Encoder_7_RestoreConfig(void) 
{      
    #if (!Encoder_7_UsingFixedFunction)

       Encoder_7_WriteCounter(Encoder_7_backup.CounterUdb);

        #if(!Encoder_7_ControlRegRemoved)
            Encoder_7_WriteControlRegister(Encoder_7_backup.CounterControlRegister);
        #endif /* (!Encoder_7_ControlRegRemoved) */

    #endif /* (!Encoder_7_UsingFixedFunction) */
}


/*******************************************************************************
* Function Name: Encoder_7_Sleep
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
*  Encoder_7_backup.enableState:  Is modified depending on the enable 
*  state of the block before entering sleep mode.
*
*******************************************************************************/
void Encoder_7_Sleep(void) 
{
    #if(!Encoder_7_ControlRegRemoved)
        /* Save Counter's enable state */
        if(Encoder_7_CTRL_ENABLE == (Encoder_7_CONTROL & Encoder_7_CTRL_ENABLE))
        {
            /* Counter is enabled */
            Encoder_7_backup.CounterEnableState = 1u;
        }
        else
        {
            /* Counter is disabled */
            Encoder_7_backup.CounterEnableState = 0u;
        }
    #else
        Encoder_7_backup.CounterEnableState = 1u;
        if(Encoder_7_backup.CounterEnableState != 0u)
        {
            Encoder_7_backup.CounterEnableState = 0u;
        }
    #endif /* (!Encoder_7_ControlRegRemoved) */
    
    Encoder_7_Stop();
    Encoder_7_SaveConfig();
}


/*******************************************************************************
* Function Name: Encoder_7_Wakeup
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
*  Encoder_7_backup.enableState:  Is used to restore the enable state of 
*  block on wakeup from sleep mode.
*
*******************************************************************************/
void Encoder_7_Wakeup(void) 
{
    Encoder_7_RestoreConfig();
    #if(!Encoder_7_ControlRegRemoved)
        if(Encoder_7_backup.CounterEnableState == 1u)
        {
            /* Enable Counter's operation */
            Encoder_7_Enable();
        } /* Do nothing if Counter was disabled before */    
    #endif /* (!Encoder_7_ControlRegRemoved) */
    
}


/* [] END OF FILE */
