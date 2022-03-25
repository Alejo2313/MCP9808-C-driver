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
 * @file example.c
 * @author Alejandro Gomez Molina (@Alejo2312)
 * @brief 
 * @version 0.1
 * @date Mar 25, 2022
 * 
 * @copyright Copyright (c) 2022
 * 
 */

/************************************************************************
	INCLUDES
************************************************************************/
#include <stdlib.h>
#include "MCP9808.h"

/************************************************************************
 	DEFINES AND TYPES
************************************************************************/
#define DEV_ADDRESS     0x18U

#define UPPER_TEMP      30.5
#define LOWER_TEMP      12.5
#define CRIT_TEMP       45.75


/************************************************************************
    FUNCTIONS
************************************************************************/
void main( void )
{
    float temperature = 0.0;
    MCP9808_Error_t result;

    result = MCP9808_Init(DEV_ADDRESS);

    if( result != MCP9808_OK )
    {
        printf("Can't init MCP9808 driver! \n");
        while( 1 )
        {
            /* Do nothing */
        }
    }
   /* Temperature window example (not needed) */
    MCP9808_SetCriticalTemperature(CRIT_TEMP);
    MCP9808_SetWindowTemperature(UPPER_TEMP, LOWER_TEMP);

    while( 1 )
    {
        result = MCP9808_ReadTemperature(&temperature);
        if( result == MCP9808_OK )
        {
            printf("Temperature %.2lf",temperature);
        }
        else
        {
            printf("Error reading temperature! \n");
        }
    }



}


