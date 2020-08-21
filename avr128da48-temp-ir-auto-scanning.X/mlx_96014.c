/*
 * File:   mlx_90614.c
 * Author: M50683
 *
 * Created on June 11, 2020, 1:11 PM
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include "i2c.h"
#include "mlx_96014.h"

#define MLX_SLAVE_ADDRESS 0x5A

/* accept 5 bit address field, command types are specified in the header file */
/* returns true if operation is succeeded or false otherwise */
/* data buffer must have at least 3 bytes */
bool MLX_Read(uint8_t address, uint8_t *pData, MLX_CMD_t command_type)
{
    uint8_t ret, command;
    bool  exit_val;
    
    if(pData == NULL)
        return false;
    
    switch(command_type)
    {
        case MLX_CMD_RAM:    command = 0x1F & address; break;
        case MLX_CMD_EEPROM: command = (0x1F & address) | 0x20; break;
        case MLX_CMD_FLAGS:  command = 0xF0; break;
        default:             return false;
    }

    ret = I2C_0_SendData((MLX_SLAVE_ADDRESS<<1), &command, 1);
    if(ret == 0xFF) // did not get ACK
    {
        I2C_0_EndSession();
        exit_val = false;
    }
    else  // received ACK
    {
        ret = I2C_0_GetData((MLX_SLAVE_ADDRESS<<1), pData, 3);
        I2C_0_EndSession();
        if(ret == 0xFF)  // did not get ACK
        {
            exit_val = false;
        }
        else    // received ACK
        {
            exit_val = true;
        }
    }
    return exit_val;
}