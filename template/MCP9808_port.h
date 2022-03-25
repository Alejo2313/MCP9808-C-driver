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
 * @file MCP9808_port.h
 * @author Alejandro Gomez Molina (@Alejo2312)
 * @brief 
 * @version 0.1
 * @date Mar 22, 2022
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#ifndef DRIVERS_INC_MCP9808_PORT_H_
#define DRIVERS_INC_MCP9808_PORT_H_


/************************************************************************
	INCLUDES
************************************************************************/
#include "MCP9808.h"

/************************************************************************
	DEFINES AND TYPES
************************************************************************/
//#define FILE_NAME_DEF	/**< Detailed description */


/************************************************************************
	FUNCTIONS
************************************************************************/

/**
  See "MCP9808_port.c" for details of how to use this function.
 */
MCP9808_Error_t MCP9808_PORT_Init( void );

/**
  See "MCP9808_port.c" for details of how to use this function.
 */
MCP9808_Error_t MCP9808_PORT_Read(uint8_t address, uint8_t reg, uint8_t size, uint8_t* data);

/**
  See "MCP9808_port.c" for details of how to use this function.
 */
MCP9808_Error_t MCP9808_PORT_Write(uint8_t address, uint8_t reg, uint8_t size, uint8_t* data);


#endif /* DRIVERS_INC_MCP9808_PORT_H_ */
