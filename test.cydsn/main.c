/* ========================================
 *
 * Copyright YOUR COMPANY, THE YEAR
 * All Rights Reserved
 * UNPUBLISHED, LICENSED SOFTWARE.
 *
 * CONFIDENTIAL AND PROPRIETARY INFORMATION
 * WHICH IS THE PROPERTY OF your company.
 *
 * ========================================
*/
#include "project.h"
#include "math.h"
#define EOL 0xFF
uint16_t relays_state = 0x0000;
uint8_t command = 0x00;
CYBIT newCommand = 0;

uint32_t actuator_1_position = 0x00000000;
uint32_t actuator_2_position = 0x00000000;
uint32_t actuator_3_position = 0x00000000;
uint32_t actuator_4_position = 0x00000000;
uint32_t actuator_5_position = 0x00000000;
uint32_t actuator_6_position = 0x00000000;
uint32_t actuator_7_position = 0x00000000;

uint32_t actuator_1_home_position = 0x00000000;
uint32_t actuator_2_home_position = 0x00000000;
uint32_t actuator_3_home_position = 0x00000000;
uint32_t actuator_4_home_position = 0x00000000;
uint32_t actuator_5_home_position = 0x00000000;
uint32_t actuator_6_home_position = 0x00000000;
uint32_t actuator_7_home_position = 0x00000000;

float encoder_tick = 0.277; // mm/pulse

typedef enum {
    OPEN_A_1,
    OPEN_A_2,
    OPEN_A_3,
    OPEN_A_4,
    OPEN_A_5,
    OPEN_A_6,
    OPEN_A_7,
    CLOSE_A_1,
    CLOSE_A_2,
    CLOSE_A_3,
    CLOSE_A_4,
    CLOSE_A_5,
    CLOSE_A_6,
    CLOSE_A_7,
    IDLE
    
} motorCommand;

void MyInst_Start(void){
    // Start Encoders ** Calls Init and Enable
    Encoder_1_Start();  
    Encoder_2_Start();
    Encoder_3_Start();
    Encoder_4_Start();
    Encoder_5_Start();
    Encoder_6_Start();
    Encoder_7_Start();
    // Start I2C ** Calls Init and Enable
    I2C_1_Start();
    // Start UART ** Calls Init and Enable
    UART_1_Start();
}


void sendI2Cbyte(uint16 rx) {
    uint8_t rx_MSB = (uint8_t)(rx >> 8);
    uint8_t rx_LSB = (uint8_t)rx;
    //I2C_1_MasterWriteBuf(0x25, rxBuff, 2, I2C_1_MODE_COMPLETE_XFER);
    uint8_t rxBuff[2] = {rx_LSB, rx_MSB}; 
    I2C_1_MasterSendStart(0x25, 0x00);

    // Write data bytes
    for (uint8 i = 0; i < 2; i++)
    {
        I2C_1_MasterWriteByte(rxBuff[i]);
    }

    // Send stop condition
    I2C_1_MasterSendStop();
    
}

void setRelayState(uint8_t relay, CYBIT state){
    uint16_t new_state = 0x0000;
    new_state = new_state | state;
    new_state = new_state << (relay-1);
    if(state == 1){
        relays_state = relays_state | new_state;
    }
    else{
        new_state = ~new_state;
        relays_state = relays_state & new_state;
    }
    sendI2Cbyte(~relays_state);
}



/*
void sendI2Cbyte(uint16 rx) {
    uint8_t rx_MSB = (uint8_t)(rx >> 8);
    uint8_t rx_LSB = (uint8_t)rx;
    uint8_t rxBuff[2] = {rx_MSB, rx_LSB}; 
    //I2C_1_MasterWriteBuf(0x25, rxBuff, 2, 0);
    I2C_1_MasterSendStart(0x25, 0x00);
    // Write data bytes
    for (uint8 i = 0; i < 2; i++)
    {
        I2C_1_MasterWriteByte(rxBuff[i]);
    }
    // Send stop condition
    I2C_1_MasterSendStop();
}

void setRelayState(uint8_t relay, CYBIT state) {
    uint16_t new_state = 0x0000;
    new_state = new_state | state;
    new_state = new_state << relay;
    relays_state = relays_state | new_state;
    sendI2Cbyte(~relays_state);
}*/

void enable_actuator(uint8_t actuator){
    switch(actuator){
    case 0x00:
        setRelayState(7, 1);
        break;
    case 0x01:
        setRelayState(8, 1);
        break;
    case 0x02:
        setRelayState(9, 1);
        break;
    case 0x03:
        setRelayState(10, 1);
        break;
    case 0x04:
        setRelayState(11, 1);
        break;
    case 0x05:
        setRelayState(12, 1);
        break;
    case 0x06:
        setRelayState(13, 1);
        break;
    default:
        UART_1_PutChar(0x44); // send Jetson an error character
        break;
    }
}

void disable_actuator(uint8_t actuator){
    switch(actuator){
    case 0x00:
        setRelayState(7, 0);
        break;
    case 0x01:
        setRelayState(8, 0);
        break;
    case 0x02:
        setRelayState(9, 0);
        break;
    case 0x03:
        setRelayState(10, 0);
        break;
    case 0x04:
        setRelayState(11, 0);
        break;
    case 0x05:
        setRelayState(12, 0);
        break;
    case 0x06:
        setRelayState(13, 0);
        break;
    default:
        UART_1_PutChar(0x55); // send Jetson an error character
        break;
    }
}

uint32_t get_actuator_position(uint8_t actuator){
    switch(actuator){
    case 0x00:
        return actuator_1_position;
        break;
    case 0x01:
        return actuator_2_position;
        break;
    case 0x02:
        return actuator_3_position;
        break;
    case 0x03:
        return actuator_4_position;
        break;
    case 0x04:
        return actuator_5_position;
        break;
    case 0x05:
        return actuator_6_position;
        break;
    case 0x06:
        return actuator_7_position;
        break;
    default:
        UART_1_PutChar(0xAA); // send Jetson an error character
        return 0xFFFFFFFF;
        break;
    }
}    

float getPosition(uint32_t actuator_position_raw){
    return (actuator_position_raw * encoder_tick);
}

void update_actuator_position(){
    uint32_t actuator_1_position_raw = 0x00000000;
    uint32_t actuator_2_position_raw = 0x00000000;
    uint32_t actuator_3_position_raw = 0x00000000;
    uint32_t actuator_4_position_raw = 0x00000000;
    uint32_t actuator_5_position_raw = 0x00000000;
    uint32_t actuator_6_position_raw = 0x00000000;
    uint32_t actuator_7_position_raw = 0x00000000;
    // Get raw encoder count
    actuator_1_position_raw = Encoder_1_ReadCounter();
    actuator_2_position_raw = Encoder_2_ReadCounter();
    actuator_3_position_raw = Encoder_3_ReadCounter();
    actuator_4_position_raw = Encoder_4_ReadCounter();
    actuator_5_position_raw = Encoder_5_ReadCounter();
    actuator_6_position_raw = Encoder_6_ReadCounter();
     actuator_7_position_raw = Encoder_7_ReadCounter();
    // Compute actual position in mm
    actuator_1_position = getPosition(actuator_1_position_raw);
    actuator_2_position = getPosition(actuator_2_position_raw);
    actuator_3_position = getPosition(actuator_3_position_raw);
    actuator_4_position = getPosition(actuator_4_position_raw);
    actuator_5_position = getPosition(actuator_5_position_raw);
    actuator_6_position = getPosition(actuator_6_position_raw);
    actuator_7_position = getPosition(actuator_7_position_raw);
}

void process_command(){
    if(UART_1_GetRxBufferSize() > 0)
    {
        // Read received byte
        uint8 rxData = UART_1_GetChar();
        if((rxData != EOL) && (newCommand == 0)){
            command = rxData;
            newCommand = 1;
        }
        else if((newCommand == 1) && (rxData == 0xFF)){
            switch(command){
                case 0x00:
                    setRelayState(0, 1);
                    break;
                case 0x01:
                    setRelayState(1, 1);
                    break;
                case 0x02:
                    setRelayState(2, 1);
                    break;
                case 0x03:
                    setRelayState(3, 1);
                    break;
                case 0x04:
                    setRelayState(4, 1);
                    break;
                case 0x05:
                    setRelayState(5, 1);
                    break;
                case 0x06:
                    setRelayState(6, 1);
                    break;
                case 0x07:
                    setRelayState(0, 0);
                    break;
                case 0x08:
                    setRelayState(1, 0);
                    break;
                case 0x09:
                    setRelayState(2, 0);
                    break;
                case 0x0A:
                    setRelayState(3, 0);
                    break;
                case 0x0B:
                    setRelayState(4, 0);
                    break;
                case 0x0C:
                    setRelayState(5, 0);
                    break;
                case 0x0D:
                    setRelayState(6, 0);
                    break;
                case 0x0E:
                    enable_actuator(0x00);
                    break;
                case 0x0F:
                    enable_actuator(0x01);
                    break;
                case 0x10:
                    enable_actuator(0x02);
                    break;
                case 0x11:
                    enable_actuator(0x03);
                    break;
                case 0x12:
                    enable_actuator(0x04);
                    break;
                case 0x13:
                    enable_actuator(0x05);
                    break;
                case 0x14:
                    enable_actuator(0x06);
                    break;
                case 0x15:
                    disable_actuator(0x00);
                    break;
                case 0x16:
                    disable_actuator(0x01);
                    break;
                case 0x17:
                    disable_actuator(0x02);
                    break;
                case 0x18:
                    disable_actuator(0x03);
                    break;
                case 0x19:
                    disable_actuator(0x04);
                    break;
                case 0x1A:
                    disable_actuator(0x05);
                    break;
                case 0x1B:
                    disable_actuator(0x06);
                    break;
                case 0x1C:
                    get_actuator_position(0x00);
                    break;
                case 0x1D:
                    get_actuator_position(0x01);
                    break;
                case 0x1E:
                    get_actuator_position(0x02);
                    break;
                case 0x1F:
                    get_actuator_position(0x03);
                    break;
                case 0x20:
                    get_actuator_position(0x04);
                    break;
                case 0x21:
                    get_actuator_position(0x05);
                    break;
                case 0x22:
                    get_actuator_position(0x06);
                    break;
                default:
                    UART_1_PutChar(0xDD); // send Jetson an error character
                    break;
            }
            newCommand = 0; 
        }
        else{
            newCommand = 0; 
            UART_1_PutChar(0xEE); // send Jetson an error character
        }
    }
}

void go_to_home(void){
    // TODO: Set relays to retract all actuators for X amount of time OR read position and stop when position no longer changes.
    update_actuator_position();
    float actuator_1_last_position = get_actuator_position(0x00);
    float actuator_2_last_position = get_actuator_position(0x01);
    float actuator_3_last_position = get_actuator_position(0x02);
    float actuator_4_last_position = get_actuator_position(0x03);
    float actuator_5_last_position = get_actuator_position(0x04);
    float actuator_6_last_position = get_actuator_position(0x05);
    float actuator_7_last_position = get_actuator_position(0x06);
    
    CYBIT actuator_1_state = 0;
    CYBIT actuator_2_state = 0;
    CYBIT actuator_3_state = 0;
    CYBIT actuator_4_state = 0;
    CYBIT actuator_5_state = 0;
    CYBIT actuator_6_state = 0;
    CYBIT actuator_7_state = 0;
    // Set the relays to retract
    
    // Enable relays
    enable_actuator(0);
    enable_actuator(1);
    enable_actuator(2);
    enable_actuator(3);
    enable_actuator(4);
    enable_actuator(5);
    enable_actuator(6);
    
    
    // Wait till all retract
    while(1){ // TODO: add timeout instead of 1
        update_actuator_position();
        
        if(fabs(actuator_1_last_position - get_actuator_position(0x00)) < (2 * encoder_tick)){
            // Stop actuator 1
            disable_actuator(0);
            actuator_1_state = 1;
        }
        
         if(fabs(actuator_2_last_position - get_actuator_position(0x01)) < (2 * encoder_tick)){
            // Stop actuator 2
            disable_actuator(1);
            actuator_2_state = 1;
        }
        
         if(fabs(actuator_3_last_position - get_actuator_position(0x02)) < (2 * encoder_tick)){
            // Stop actuator 3
            disable_actuator(2);
            actuator_3_state = 1;
        }
        
         if(fabs(actuator_4_last_position - get_actuator_position(0x03)) < (2 * encoder_tick)){
            // Stop actuator 4
            disable_actuator(3);
            actuator_4_state = 1;
        }
        
         if(fabs(actuator_5_last_position - get_actuator_position(0x04)) < (2 * encoder_tick)){
            // Stop actuator 5
            disable_actuator(4);
            actuator_5_state = 1;
        }
        
         if(fabs(actuator_6_last_position - get_actuator_position(0x05)) < (2 * encoder_tick)){
            // Stop actuator 6
            disable_actuator(5);
            actuator_6_state = 1;
        }
        
         if(fabs(actuator_7_last_position - get_actuator_position(0x06)) < (2 * encoder_tick)){
            // Stop actuator 7
            disable_actuator(6);
            actuator_7_state = 1;
        }
        if(actuator_1_state && actuator_2_state && actuator_3_state && actuator_4_state && actuator_5_state && actuator_6_state && actuator_7_state){
            break;
        }
        
        actuator_2_last_position = get_actuator_position(0x01);
        actuator_3_last_position = get_actuator_position(0x02);
        actuator_4_last_position = get_actuator_position(0x03);
        actuator_5_last_position = get_actuator_position(0x04);
        actuator_6_last_position = get_actuator_position(0x05);
        actuator_7_last_position = get_actuator_position(0x06);
        
        actuator_1_last_position = get_actuator_position(0x00);
        actuator_2_last_position = get_actuator_position(0x01);
        actuator_3_last_position = get_actuator_position(0x02);
        actuator_4_last_position = get_actuator_position(0x03);
        actuator_5_last_position = get_actuator_position(0x04);
        actuator_6_last_position = get_actuator_position(0x05);
        actuator_7_last_position = get_actuator_position(0x06);
    }
    
    // Set you home value
    Encoder_1_WriteCounter(0x00000000);
    Encoder_2_WriteCounter(0x00000000);
    Encoder_3_WriteCounter(0x00000000);
    Encoder_4_WriteCounter(0x00000000);
    Encoder_5_WriteCounter(0x00000000);
    Encoder_6_WriteCounter(0x00000000);
    Encoder_7_WriteCounter(0x00000000);
}


int main(void)
{
    CyGlobalIntEnable; /* Enable global interrupts. */
    MyInst_Start();
    
    sendI2Cbyte(0xFFFF);
    CyDelay(1000);
    setRelayState(1, 1);
    CyDelay(1000);
    
    
    for(;;){
        sendI2Cbyte(0xFFFF);
        CyDelay(1000);
        for(int i = 1; i <= 16; i++){
            setRelayState(i, 1);
            CyDelay(1000);
        }
        for(int i = 1; i <= 16; i++){
            setRelayState(i, 0);
            CyDelay(1000);
        }
    
    }
    
    
    
    
    
    go_to_home();
    /* Place your initialization/startup code here (e.g. MyInst_Start()) */
    
    for(;;)
    {
        process_command();
        update_actuator_position();
        /* Place your application code here. */
    }
}

/* [] END OF FILE */
