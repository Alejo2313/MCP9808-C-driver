/**                             _____________
 *              /\      /\     /             \
 *             //\\____//\\   |   MAUUUU!!    |
 *            /     '      \   \  ___________/
 *           /   /\ '  /\    \ /_/                  / /  ___    / __\ |__   __ _| |_
 *          |    == o ==      |       /|         / /  / _ \  / /  | '_ \ / _` | __|
 *           \      '        /       | |        / /__|  __/ / /___| | | | (_| | |_
 *             \           /         \ \        \____/\___| \____/|_| |_|\__,_|\__|
 *             /----<o>---- \         / /
 *             |            ' \       \ \
 *             |    |    | '   '\      \ \
 *  _________  | ´´ |  ' |     '  \    / /
 *  |  MAYA  | |  ' |    | '       |__/ /
 *   \______/   \__/ \__/ \_______/____/
 * 
 * @file MCP9808.h
 * @author Alejandro Gomez Molina (@Alejo2312)
 * @brief 
 * @version 0.1
 * @date Mar 22, 2022
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#ifndef DRIVERS_INC_MCP9808_H_
#define DRIVERS_INC_MCP9808_H_


/************************************************************************
    INCLUDES
************************************************************************/
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

/************************************************************************
    DEFINES AND TYPES
************************************************************************/

#define MCP9808_ERROR           -1
#define MCP9808_OK               0

#define MCP9808_REG_SIZE        2U
#define MCP9808_SIGN_MASK       0x10U
#define MCP9808_MSB             0U
#define MCP9808_LSB             1U


#define IS_MCP9808_ERROR( error ) ((error) < MCP9808_ERROR)


typedef int MCP9808_Error_t;

typedef enum
{
    /* 0b0000 RESERVED */
    MCP9808_REG_CONFIG          = 0b0001, /**< Configuration register (CONFIG) */
    MCP9808_REG_UPPER_TEMP      = 0b0010, /**< Alert Temperature Upper Boundary Trip register (T UPPER*/
    MCP9808_REG_LOWER_TEMP      = 0b0011, /**< Alert Temperature Lower Boundary Trip register (T LOWER )*/
    MCP9808_REG_CRITICAL_TEMP   = 0b0100, /**< Critical Temperature Trip register (TCRIT)*/
    MCP9808_REG_TEMPERATURE     = 0b0101, /**< Temperature register (TA) */
    MCP9808_REG_ID_1            = 0b0110, /**< Manufacturer ID register */
    MCP9808_REG_ID_2            = 0b0111, /**< Device ID/Revision register*/
    MCP9808_REG_RESOLUTION      = 0b1000, /**< Resolution register */
    /* 0b1001 to 0b1111 RESERVED */
}MCP9808_Register_t;

/** TUPPER and TLOWER Limit Hysteresis bits */
typedef enum
{
    MCP9808_HYST_0C5 = (0b00 << 1),    /**< +0.0C */
    MCP9808_HYST_1C5 = (0b01 << 1),    /**< +1.5C */
    MCP9808_HYST_3C0 = (0b10 << 1),    /**< +3.0C */
    MCP9808_HYST_6C0 = (0b11 << 1),    /**< +6.C */
    MCP9808_HYST_MSK = (0b11 << 1),    /**< Mask */
}MCP9808_Hysteresis_t;

typedef enum
{
    MCP9808_RESOLUTION_1 = 0b00,    /**< 0.5°C (tCONV = 30 ms typical) */
    MCP9808_RESOLUTION_2 = 0b01,    /**< +0.25°C (tCONV = 65 ms typical) */
    MCP9808_RESOLUTION_3 = 0b10,    /**< +0.125°C (tCONV = 130 ms typical) */
    MCP9808_RESOLUTION_4 = 0b11,    /**< +0.0625°C (power-up default, t CONV = 250 ms typical) */
    MCP9808_RESOLUTION_MSK = 0b11,
}MCP9808_Resolution_t;

typedef enum
{
    MCP9808_CONFIG_CRIT_LOCK        = 0x80, /** Lock Bit (Crit. Lock) */
    MCP9808_CONFIG_WIN_LOCK         = 0x40, /**< T UPPER and T LOWER Window Lock Bit (Win. Lock) */
    MCP9808_CONFIG_CLEAR_IRQ        = 0x20, /**< Interrupt Clear (Int. Clear) */
    MCP9808_CONFIG_ALERT_STATUS     = 0x10, /**< Alert Output Status (Alert Stat.) */
    MCP9808_CONFIG_ALERT_CONTROL    = 0x08, /**< Alert Output Control (Alert Cnt.) */
    MCP9808_CONFIG_ALERT_MODE       = 0x04, /**< Alert Output Select (Alert Sel.) */
    MCP9808_CONFIG_ALERT_POLARITY   = 0x02, /**< Alert Output Polarity (Alert Pol.) */
    MCP9808_CONFIG_ALERT_OUTPUT     = 0x01, /**< Alert Output Mode (Alert Mod.) */
}MCP9808_Config_t;

/** Alert Output Select (Alert Sel.) Bit: */
typedef enum
{
    MCP9808_ALERT_MODE_ALL      = 0x00, /**< Alert output for T UPPER, TLOWER and T CRIT (power-up default) */
    MCP9808_ALERT_MODE_TCRIT    = 0x04, /**< TA > TCRIT only. (TUPPER and T LOWER temperature boundaries are disabled.) */
    MCP9808_ALERT_MODE_MSK      = 0x04
}MCP9808_Alert_Mode_t;

/** Alert Output Polarity (Alert Pol.) Bit: */
typedef enum
{
    MCP9808_ALERT_POL_lOW     = 0x00,   /**< Active low (power-up default. Pull-up resistor required) */
    MCP9808_ALERT_POL_HIGH    = 0x02,   /**< Active-high */
    MCP9808_ALERT_POL_MSK     = 0x02,
}MCP9808_Alert_Polarity_t;

/** Alert Output Mode (Alert Mod.) Bit: */
typedef enum
{
    MCP9808_ALERT_OUTPUT_COMP   = 0x00, /**< Comparator output (power-up default) */
    MCP9808_ALERT_OUTPUT_IRQ    = 0x01, /**< Interrupt output */
    MCP9808_ALERT_OUTPUT_MSK    = 0x01
}MCP9808_Alert_Output_t;

/************************************************************************
    FUNCTIONS
************************************************************************/

/** Driver configuration functions */

/**
  See "MCP98008.c" for details of how to use this function.
 */
MCP9808_Error_t MCP9808_Init( uint8_t devAddress );

/**
  See "MCP98008.c" for details of how to use this function.
 */
MCP9808_Error_t MCP9808_setDevAddress( uint8_t devAddress );

/** Read data functions */

/**
  See "MCP98008.c" for details of how to use this function.
 */
MCP9808_Error_t MCP9808_ReadTemperature( float* temperature );

/**
  See "MCP98008.c" for details of how to use this function.
 */
MCP9808_Error_t MCP9808_GetCriticalTemperature( float* temperature );

/**
  See "MCP98008.c" for details of how to use this function.
 */
MCP9808_Error_t MCP9808_GetWindowTemperature( float* upperTemp, float* lowerTemp );

/**
  See "MCP98008.c" for details of how to use this function.
 */
MCP9808_Error_t MCP9808_GetHysteresis( MCP9808_Hysteresis_t* hysteresis );

/**
  See "MCP98008.c" for details of how to use this function.
 */
MCP9808_Error_t MCP9808_GetResolution( MCP9808_Resolution_t* resolution );

/**
  See "MCP98008.c" for details of how to use this function.
 */
MCP9808_Error_t MCP9808_GetID( uint8_t* id, uint8_t* revision );

/**
  See "MCP98008.c" for details of how to use this function.
 */
MCP9808_Error_t MCP9808_GetManufactureID( uint16_t*id );

/** Device configuration functions */

/**
  See "MCP98008.c" for details of how to use this function.
 */
MCP9808_Error_t MCP9808_SetCriticalTemperature( float temperature );
/**
  See "MCP98008.c" for details of how to use this function.
 */
MCP9808_Error_t MCP9808_SetWindowTemperature( float upperTemp, float lowerTemp );
/**
  See "MCP98008.c" for details of how to use this function.
 */
MCP9808_Error_t MCP9808_SetHysteresis( MCP9808_Hysteresis_t hysteresis );
/**
  See "MCP98008.c" for details of how to use this function.
 */
MCP9808_Error_t MCP9808_SetResolution( MCP9808_Resolution_t resolution );

/** Device alert functions */

/**
  See "MCP98008.c" for details of how to use this function.
 */
bool MCP9808_IsAlertAsserted( void );

/**
  See "MCP98008.c" for details of how to use this function.
 */
MCP9808_Error_t MCP9808_EnableAlert( void );
/**
  See "MCP98008.c" for details of how to use this function.
 */
MCP9808_Error_t MCP9808_DisableAlert( void );
/**
  See "MCP98008.c" for details of how to use this function.
 */
MCP9808_Error_t MCP9808_SetAlertMode( MCP9808_Alert_Mode_t mode );
/**
  See "MCP98008.c" for details of how to use this function.
 */
MCP9808_Error_t MCP9808_SetAlertPolarity( MCP9808_Alert_Polarity_t polarity );
/**
  See "MCP98008.c" for details of how to use this function.
 */
MCP9808_Error_t MCP9808_SetAlertOutput( MCP9808_Alert_Output_t output );

/** Other configuration functions */

/**
  See "MCP98008.c" for details of how to use this function.
 */
MCP9808_Error_t MCP9808_LockWindowTempReg( void );
/**
  See "MCP98008.c" for details of how to use this function.
 */
MCP9808_Error_t MCP9808_UnlockWindowTempReg( void );
/**
  See "MCP98008.c" for details of how to use this function.
 */
MCP9808_Error_t MCP9808_LockCriticalTempReg( void );
/**
  See "MCP98008.c" for details of how to use this function.
 */
MCP9808_Error_t MCP9808_UnlockCriticalTempReg( void );
/**
  See "MCP98008.c" for details of how to use this function.
 */
MCP9808_Error_t MCP9808_ClearInterrupt( void );


#endif /* DRIVERS_INC_MCP9808_H_ */
