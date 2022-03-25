/**                             _____________
 *              /\      /\     /             \
 *             //\\____//\\   |   MAUUUU!!    |
 *            /     '      \   \  ___________/
 *           /   /\ '  /\    \ /_/			      / /  ___    / __\ |__   __ _| |_ 
 *          |    == o ==      |       /|	     / /  / _ \  / /  | '_ \ / _` | __|
 *           \      '        /       | |	    / /__|  __/ / /___| | | | (_| | |_ 
 *             \           /         \ \	    \____/\___| \____/|_| |_|\__,_|\__|
 *             /----<o>---- \         / /
 *             |            ' \       \ \
 *             |    |    | '   '\      \ \
 *  _________  | ´´ |  ' |     '  \    / /
 *  |  MAYA  | |  ' |    | '       |__/ /
 *   \______/   \__/ \__/ \_______/____/
 * 
 * @file MCP9808_port.c
 * @author Alejandro Gomez Molina (@Alejo2312)
 * @brief I2C port layer for MPC9808 driver.
 * @version 0.1
 * @date Mar 22, 2022
 * 
 * @copyright Copyright (c) 2022
 * 
 */

/************************************************************************
	INCLUDES
************************************************************************/
#include "MCP9808_port.h"
/************************************************************************
 	DEFINES AND TYPES
************************************************************************/


/************************************************************************
	DECLARATIONS
************************************************************************/


/************************************************************************
	FUNCTIONS
************************************************************************/

/**
 * @brief Initialize I2c driver.
 *
 * @return error_t NO_ERROR if the device has been configured otherwise, SYS_ERROR.
 */
MCP9808_Error_t MCP9808_PORT_Init( void )
{
	/* Implement your function here! */
	return 0;
}

/**
 * @brief Read a register o execute a read command in a I2C register based device.
 * 		| S |  ADDR  | W | A | REG | A | DATA0 | ---- | DATAN | A | P |
 *
 * @param address Slave address
 * @param reg Register/command to be read
 * @param size Register size in byte
 * @param data Register data
 * @return error_t NO_ERROR if the register has been read otherwise, SYS_ERROR
 */
MCP9808_Error_t MCP9808_PORT_Read(uint8_t address, uint8_t reg, uint8_t size, uint8_t* data)
{
	/* Implement your function here! */
	return 0;
}

/**
 * @brief Write a register or execute a write command in a I2C register based device.
 * 		| S |  ADDR  | W | A | REG | A |
 * 		| S |  ADDR  | R | A | DATA0 | ---- | DATAN | A | P |
 *
 * @param address Slave address
 * @param reg Register/command to be written
 * @param size Register size in byte
 * @param data Register data
 * @return error_t NO_ERROR if the register has been written otherwise, SYS_ERROR
 */
MCP9808_Error_t MCP9808_PORT_Write(uint8_t address, uint8_t reg, uint8_t size, uint8_t* data)
{
	/* Implement your function here! */
	return 0;
}
