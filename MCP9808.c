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
 * @file MCP9808.c
 * @author Alejandro Gomez Molina (@Alejo2312)
 * @brief 
 * @version 0.1
 * @date Mar 22, 2022
 * 
 * @copyright Copyright (c) 2022
 * 
 */

/************************************************************************
    INCLUDES
************************************************************************/
#include "MCP9808.h"
#include "MCP9808_port.h"
/************************************************************************
     DEFINES AND TYPES
************************************************************************/



/************************************************************************
    DECLARATIONS
************************************************************************/
static bool MCP9808_isInitialized = false;    /**< Set to true when system initialized */
static uint8_t MCP9808_Address = 0x00;        /**< Device I2C address */

/************************************************************************
    FUNCTIONS
************************************************************************/
/**
 * @brief Convert register temperature data to float.
 *
 * @param regData Register data pointer (uint8_t[2]).
 * @param temperature Pointer to temperature storage.
 * @return MCP9808_Error_t A number lower than '0' if something was wrong.
 */
static MCP9808_Error_t MCP9808_RegToTemp( uint8_t* regData, float* temperature )
{
    MCP9808_Error_t error = MCP9808_ERROR;

    if( (regData != NULL) && (temperature != NULL) )
    {
        if( regData[MCP9808_MSB]&MCP9808_SIGN_MASK )
        {
            regData[MCP9808_MSB] = regData[MCP9808_MSB]&0xF;
            *temperature = 256.0 - ((regData[MCP9808_MSB]*16.0) + (regData[MCP9808_LSB]/16.0));
        }
        else
        {
            regData[MCP9808_MSB] = regData[MCP9808_MSB]&0xF;
            *temperature = ((regData[MCP9808_MSB]*16.0) + (regData[MCP9808_LSB]/16.0));
        }
    }

    return error;
}

/**
 * @brief Transform float temperature into register format.
 *
 * @param regData Register data pointer (uint8_t[2]).
 * @param temperature Pointer to temperature.
 * @return
 */
static MCP9808_Error_t MCP9808_TempToReg( uint8_t* regData, float* temperature )
{
    MCP9808_Error_t error = MCP9808_ERROR;

    int16_t floatExp = 0;
    uint16_t formatedTemp = 0;
    uint32_t* tempData = (uint32_t*)(temperature);

    if( (regData != NULL) && (temperature != NULL) )
    {    /*                    1    8        23
        /* IEE-754 format  |s| exp |  mantissa | */
        /* Extract float exponent */
        floatExp = (*tempData >> 23)&0xFF;
        floatExp -= 127;
        /*                                                    8        2            */
        /* Shift right to obtain a number with format | integer | decimal |    */

        formatedTemp  = ( (*tempData&0x7FFFFF) | 0x800000) >> ( 23 - (floatExp + 2) );

        /* Shift left to match register format */
        formatedTemp = (formatedTemp << 2)&0xFFC;
        /* MSB*/
        regData[MCP9808_MSB] = (formatedTemp >> 8)&0xFF;
        /* LSB */
        regData[MCP9808_LSB] = (formatedTemp)&0xFF;

        /* Add sign bit */
        if( *temperature < 0 )
        {
            regData[MCP9808_MSB] |= MCP9808_SIGN_MASK;
        }

        error = MCP9808_OK;
    }

    return error;
}

/**
 * @brief Initialize MPC9808 driver.
 *
 * @return MCP9808_Error_t A number lower than '0' if something was wrong.
 */
MCP9808_Error_t MCP9808_Init( uint8_t devAddress )
{
    MCP9808_Error_t error = MCP9808_ERROR;

    if( !MCP9808_isInitialized )
    {
        error = MCP9808_PORT_Init();
        MCP9808_Address = devAddress;

        if( !IS_MCP9808_ERROR(error) )
        {
            MCP9808_isInitialized = true;
        }

    }
    return error;
}

/**
 * @brief Update I2C device address.
 *
 * @param devAddress I2C device address.
 * @return MCP9808_Error_t A number lower than '0' if something was wrong.
 */
MCP9808_Error_t MCP9808_setDevAddress( uint8_t devAddress )
{
    MCP9808_Address = devAddress;
}

/**
 * @brief Read current temperature.
 *
 * @param temperature Pointer to temperature storage.
 * @return MCP9808_Error_t A number lower than '0' if something was wrong.
 */
MCP9808_Error_t MCP9808_ReadTemperature( float* temperature )
{
    MCP9808_Error_t error = MCP9808_ERROR;
    uint8_t regData[MCP9808_REG_SIZE];
    uint16_t rawValue = 0;

    error = MCP9808_PORT_Read(MCP9808_Address,
                            MCP9808_REG_TEMPERATURE,
                            MCP9808_REG_SIZE, regData);
    if( !IS_MCP9808_ERROR(error) )
    {
        error = MCP9808_RegToTemp(regData, temperature);
    }
    return error;
}

/**
 * @brief     Set critical temperature value. This value is used to generate
 *             alert signals.
 *
 * @param temperature Temperature to set.
 * @return MCP9808_Error_t A number lower than '0' if something was wrong.
 */
MCP9808_Error_t MCP9808_SetCriticalTemperature( float temperature )
{
    MCP9808_Error_t error = MCP9808_ERROR;
    uint8_t regData[MCP9808_REG_SIZE];

    error = MCP9808_TempToReg(regData, &temperature);

    if( !IS_MCP9808_ERROR(error) )
    {
        error = MCP9808_PORT_Write(MCP9808_Address,
                                MCP9808_REG_CRITICAL_TEMP,
                                MCP9808_REG_SIZE, regData);
    }

    return error;
}

/**
 * @brief     Set critical temperature value from device.
 *             the alert function must be enabled.
 *
 * @param temperature Pointer to temperature storage.
 * @return MCP9808_Error_t A number lower than '0' if something was wrong.
 */
MCP9808_Error_t MCP9808_GetCriticalTemperature( float* temperature )
{
    MCP9808_Error_t error = MCP9808_ERROR;
    uint8_t regData[MCP9808_REG_SIZE];

    error = MCP9808_PORT_Read(MCP9808_Address,
                            MCP9808_REG_CRITICAL_TEMP,
                            MCP9808_REG_SIZE, regData);
    if( !IS_MCP9808_ERROR(error) )
    {
        error = MCP9808_RegToTemp(regData, temperature);
    }
    return error;
}

/**
 * @brief     Set temperature window. This value are used to generate alert signals.
 *            the alert function must be enabled.
 * @param upperTemp Upper temperature boundary
 * @param lowerTemp Lower temperature boundary.
 * @return MCP9808_Error_t A number lower than '0' if something was wrong.
 */
MCP9808_Error_t MCP9808_SetWindowTemperature( float upperTemp, float lowerTemp )
{
    MCP9808_Error_t error = MCP9808_ERROR;
    uint8_t regData[MCP9808_REG_SIZE];

    error = MCP9808_TempToReg(regData, &upperTemp);

    if( !IS_MCP9808_ERROR(error) )
    {
        error = MCP9808_PORT_Write(MCP9808_Address,
                                MCP9808_REG_UPPER_TEMP,
                                MCP9808_REG_SIZE, regData);

        if( !IS_MCP9808_ERROR(error) )
        {
            error = MCP9808_TempToReg(regData, &upperTemp);
            if( !IS_MCP9808_ERROR(error) )
            {
                error = MCP9808_PORT_Write(MCP9808_Address,
                                        MCP9808_REG_LOWER_TEMP,
                                        MCP9808_REG_SIZE, regData);
            }

        }
    }

    return error;
}

/**
 * @brief Get temperature window.
 *
 * @param upperTemp Upper temperature boundary storage.
 * @param lowerTemp Lower temperature boundary storage.
 * @return MCP9808_Error_t A number lower than '0' if something was wrong.
 */
MCP9808_Error_t MCP9808_GetWindowTemperature( float* upperTemp, float* lowerTemp )
{
    MCP9808_Error_t error = MCP9808_ERROR;
    uint8_t regData[MCP9808_REG_SIZE];

    error = MCP9808_PORT_Read(MCP9808_Address,
                            MCP9808_REG_UPPER_TEMP,
                            MCP9808_REG_SIZE, regData);
    if( !IS_MCP9808_ERROR(error) )
    {
        error = MCP9808_RegToTemp(regData, upperTemp);
        if( !IS_MCP9808_ERROR(error) )
        {
            error = MCP9808_PORT_Read(MCP9808_Address,
                                        MCP9808_REG_LOWER_TEMP,
                                        MCP9808_REG_SIZE, regData);
            if( !IS_MCP9808_ERROR(error) )
            {
                error = MCP9808_RegToTemp(regData, lowerTemp);
            }
        }

    }
    return error;
}

/**
 * @brief Enable window register write protection.
 *
 * @return MCP9808_Error_t A number lower than '0' if something was wrong.
 */
MCP9808_Error_t MCP9808_LockWindowTempReg( void )
{
    MCP9808_Error_t error = MCP9808_ERROR;
    uint8_t regData[MCP9808_REG_SIZE];


    error = MCP9808_PORT_Read(MCP9808_Address,
                                MCP9808_REG_CONFIG,
                                MCP9808_REG_SIZE, regData);
    if ( !IS_MCP9808_ERROR(error) )
    {
        regData[MCP9808_LSB] |= MCP9808_CONFIG_WIN_LOCK;

        error = MCP9808_PORT_Write(MCP9808_Address,
                                MCP9808_REG_CONFIG,
                                MCP9808_REG_SIZE, regData);
    }

    return error;
}

/**
 * @brief Disable window register write protection.
 *
 * @return MCP9808_Error_t A number lower than '0' if something was wrong.
 */
MCP9808_Error_t MCP9808_UnlockWindowTempReg( void )
{
    MCP9808_Error_t error = MCP9808_ERROR;
    uint8_t regData[MCP9808_REG_SIZE];


    error = MCP9808_PORT_Read(MCP9808_Address,
                                MCP9808_REG_CONFIG,
                                MCP9808_REG_SIZE, regData);
    if ( !IS_MCP9808_ERROR(error) )
    {
        regData[MCP9808_LSB] &= ~(MCP9808_CONFIG_WIN_LOCK);

        error = MCP9808_PORT_Write(MCP9808_Address,
                                MCP9808_REG_CONFIG,
                                MCP9808_REG_SIZE, regData);
    }

    return error;
}

/**
 * @brief Enable critical temperature register write protection.
 *
 * @return MCP9808_Error_t A number lower than '0' if something was wrong.
 */
MCP9808_Error_t MCP9808_LockCriticalTempReg( void )
{
    MCP9808_Error_t error = MCP9808_ERROR;
    uint8_t regData[MCP9808_REG_SIZE];


    error = MCP9808_PORT_Read(MCP9808_Address,
                                MCP9808_REG_CONFIG,
                                MCP9808_REG_SIZE, regData);
    if ( !IS_MCP9808_ERROR(error) )
    {
        regData[MCP9808_LSB] |= MCP9808_CONFIG_CRIT_LOCK;

        error = MCP9808_PORT_Write(MCP9808_Address,
                                MCP9808_REG_CONFIG,
                                MCP9808_REG_SIZE, regData);
    }

    return error;
}

/**
 * @brief Disable critical temperature register write protection.
 *
 * @return MCP9808_Error_t A number lower than '0' if something was wrong.
 */
MCP9808_Error_t MCP9808_UnlockCriticalTempReg( void )
{
    MCP9808_Error_t error = MCP9808_ERROR;
    uint8_t regData[MCP9808_REG_SIZE];


    error = MCP9808_PORT_Read(MCP9808_Address,
                                MCP9808_REG_CONFIG,
                                MCP9808_REG_SIZE, regData);

    if ( !IS_MCP9808_ERROR(error) )
    {
        regData[MCP9808_LSB] &= ~(MCP9808_CONFIG_CRIT_LOCK);

        error = MCP9808_PORT_Write(MCP9808_Address,
                                MCP9808_REG_CONFIG,
                                MCP9808_REG_SIZE, regData);
    }

    return error;
}

/**
 * @brief Clear interrupt status.
 *
 * @return MCP9808_Error_t A number lower than '0' if something was wrong.
 */
MCP9808_Error_t MCP9808_ClearInterrupt( void )
{
    MCP9808_Error_t error = MCP9808_ERROR;
    uint8_t regData[MCP9808_REG_SIZE];


    error = MCP9808_PORT_Read(MCP9808_Address,
                                MCP9808_REG_CONFIG,
                                MCP9808_REG_SIZE, regData);
    if ( !IS_MCP9808_ERROR(error) )
    {
        regData[MCP9808_LSB] |= MCP9808_CONFIG_CLEAR_IRQ;

        error = MCP9808_PORT_Write(MCP9808_Address,
                                MCP9808_REG_CONFIG,
                                MCP9808_REG_SIZE, regData);
    }

    return error;
}

/**
 * @brief Check if the alert is asserted.
 *
 * @return MCP9808_Error_t A number lower than '0' if something was wrong.
 */
bool MCP9808_IsAlertAsserted( void )
{
    bool result = false;
    MCP9808_Error_t error = MCP9808_ERROR;
    uint8_t regData[MCP9808_REG_SIZE];


    error = MCP9808_PORT_Read(MCP9808_Address,
                                MCP9808_REG_CONFIG,
                                MCP9808_REG_SIZE, regData);
    if ( !IS_MCP9808_ERROR(error) )
    {
        result = regData[MCP9808_LSB]&MCP9808_CONFIG_ALERT_STATUS;

    }

    return result;
}

/**
 * @brief Enable alert function.
 *
 * @return MCP9808_Error_t A number lower than '0' if something was wrong.
 */
MCP9808_Error_t MCP9808_EnableAlert( void )
{
    MCP9808_Error_t error = MCP9808_ERROR;
    uint8_t regData[MCP9808_REG_SIZE];


    error = MCP9808_PORT_Read(MCP9808_Address,
                                MCP9808_REG_CONFIG,
                                MCP9808_REG_SIZE, regData);
    if ( !IS_MCP9808_ERROR(error) )
    {
        regData[MCP9808_LSB] |= MCP9808_CONFIG_ALERT_CONTROL;

        error = MCP9808_PORT_Write(MCP9808_Address,
                                MCP9808_REG_CONFIG,
                                MCP9808_REG_SIZE, regData);
    }

    return error;
}

/**
 * @brief Disable alert function.
 *
 * @return MCP9808_Error_t A number lower than '0' if something was wrong.
 */
MCP9808_Error_t MCP9808_DisableAlert( void )
{
    MCP9808_Error_t error = MCP9808_ERROR;
    uint8_t regData[MCP9808_REG_SIZE];


    error = MCP9808_PORT_Read(MCP9808_Address,
                                MCP9808_REG_CONFIG,
                                MCP9808_REG_SIZE, regData);
    if ( !IS_MCP9808_ERROR(error) )
    {
        regData[MCP9808_LSB] &= ~(MCP9808_CONFIG_ALERT_CONTROL);

        error = MCP9808_PORT_Write(MCP9808_Address,
                                MCP9808_REG_CONFIG,
                                MCP9808_REG_SIZE, regData);
    }

    return error;
}

/**
 * @brief     Set alert output assert mode. If it is set to "MCP9808_ALERT_MODE_ALL"
 *             the output will be updated when the current temperature crosses any boundary,
 *             otherwise, the output will only be updated when it crosses TCRIT.
 *
 * @param mode Output mode.
 * @return MCP9808_Error_t A number lower than '0' if something was wrong.
 */
MCP9808_Error_t MCP9808_SetAlertMode( MCP9808_Alert_Mode_t mode )
{
    MCP9808_Error_t error = MCP9808_ERROR;
    uint8_t regData[MCP9808_REG_SIZE];


    error = MCP9808_PORT_Read(MCP9808_Address,
                                MCP9808_REG_CONFIG,
                                MCP9808_REG_SIZE, regData);
    if ( !IS_MCP9808_ERROR(error) )
    {
        regData[MCP9808_LSB] &= ~(MCP9808_ALERT_MODE_MSK);
        regData[MCP9808_LSB] |= mode&MCP9808_ALERT_MODE_MSK;

        error = MCP9808_PORT_Write(MCP9808_Address,
                                MCP9808_REG_CONFIG,
                                MCP9808_REG_SIZE, regData);
    }

    return error;
}

/**
 * @brief    Set alert output polarity (HIGH or LOW).
 *
 * @param polarity Polarity to set.
 * @return MCP9808_Error_t A number lower than '0' if something was wrong.
 */
MCP9808_Error_t MCP9808_SetAlertPolarity( MCP9808_Alert_Polarity_t polarity )
{
    MCP9808_Error_t error = MCP9808_ERROR;
    uint8_t regData[MCP9808_REG_SIZE];


    error = MCP9808_PORT_Read(MCP9808_Address,
                                MCP9808_REG_CONFIG,
                                MCP9808_REG_SIZE, regData);
    if ( !IS_MCP9808_ERROR(error) )
    {
        regData[MCP9808_LSB] &= ~(MCP9808_ALERT_POL_MSK);
        regData[MCP9808_LSB] |= polarity&MCP9808_ALERT_POL_MSK;

        error = MCP9808_PORT_Write(MCP9808_Address,
                                MCP9808_REG_CONFIG,
                                MCP9808_REG_SIZE, regData);
    }

    return error;
}

/**
 * @brief Set alert output mode (compare or IRQ).
 *
 * @param output Output mode to set.
 * @return MCP9808_Error_t A number lower than '0' if something was wrong.
 */
MCP9808_Error_t MCP9808_SetAlertOutput( MCP9808_Alert_Output_t output )
{
    MCP9808_Error_t error = MCP9808_ERROR;
    uint8_t regData[MCP9808_REG_SIZE];


    error = MCP9808_PORT_Read(MCP9808_Address,
                                MCP9808_REG_CONFIG,
                                MCP9808_REG_SIZE, regData);
    if ( !IS_MCP9808_ERROR(error) )
    {
        regData[MCP9808_LSB] &= ~(MCP9808_ALERT_OUTPUT_MSK);
        regData[MCP9808_LSB] |= output&MCP9808_ALERT_OUTPUT_MSK;

        error = MCP9808_PORT_Write(MCP9808_Address,
                                MCP9808_REG_CONFIG,
                                MCP9808_REG_SIZE, regData);
    }

    return error;
}

/**
 * @brief Get device ID and revision.
 *
 * @param id Pointer to ID storage.
 * @param revision Pointer to revision storage.
 * @return MCP9808_Error_t A number lower than '0' if something was wrong.
 */
MCP9808_Error_t MCP9808_GetID( uint8_t* id, uint8_t* revision )
{
    MCP9808_Error_t error = MCP9808_ERROR;
    uint8_t regData[MCP9808_REG_SIZE];


    error = MCP9808_PORT_Read(MCP9808_Address,
                                MCP9808_REG_ID_2,
                                MCP9808_REG_SIZE, regData);
    if ( !IS_MCP9808_ERROR(error) )
    {
        *id = regData[MCP9808_MSB];
        *revision = regData[MCP9808_LSB];
    }

    return error;
}

/**
 * @brief Get hysteresis configuration.
 *
 * @param hysteresis Pointer to hysteresis mode storage.
 * @return MCP9808_Error_t A number lower than '0' if something was wrong.
 */
MCP9808_Error_t MCP9808_GetHysteresis( MCP9808_Hysteresis_t* hysteresis )
{
    MCP9808_Error_t error = MCP9808_ERROR;
    uint8_t regData[MCP9808_REG_SIZE];


    error = MCP9808_PORT_Read(MCP9808_Address,
                                MCP9808_REG_CONFIG,
                                MCP9808_REG_SIZE, regData);
    if ( !IS_MCP9808_ERROR(error) )
    {
        *hysteresis = regData[MCP9808_MSB] & MCP9808_HYST_MSK;
    }

    return error;
}

/**
 * @brief Set hysteresis configuration.
 *
 * @param hysteresis Hysteresis mode.
 * @return MCP9808_Error_t A number lower than '0' if something was wrong.
 */
MCP9808_Error_t MCP9808_SetHysteresis( MCP9808_Hysteresis_t hysteresis )
{
    MCP9808_Error_t error = MCP9808_ERROR;
    uint8_t regData[MCP9808_REG_SIZE];


    error = MCP9808_PORT_Read(MCP9808_Address,
                                MCP9808_REG_CONFIG,
                                MCP9808_REG_SIZE, regData);
    if ( !IS_MCP9808_ERROR(error) )
    {
        regData[MCP9808_MSB] &= ~(MCP9808_HYST_MSK);
        regData[MCP9808_MSB] |= hysteresis&MCP9808_HYST_MSK;

        error = MCP9808_PORT_Write(MCP9808_Address,
                                MCP9808_REG_CONFIG,
                                MCP9808_REG_SIZE, regData);
    }

    return error;
}

/**
 * @brief Set temperature resolution configuration.
 *
 * @param resolution Temperature resolution storage.
 * @return MCP9808_Error_t A number lower than '0' if something was wrong.
 */
MCP9808_Error_t MCP9808_SetResolution( MCP9808_Resolution_t resolution )
{
    MCP9808_Error_t error = MCP9808_ERROR;
    uint8_t regData;

    error = MCP9808_PORT_Read(MCP9808_Address, MCP9808_REG_RESOLUTION,1, &regData);
    if ( !IS_MCP9808_ERROR(error) )
    {
        regData &= ~(MCP9808_RESOLUTION_MSK);
        regData |= resolution&MCP9808_RESOLUTION_MSK;

        error = MCP9808_PORT_Write(MCP9808_Address, MCP9808_REG_RESOLUTION, 1, &regData);
    }

    return error;
}

/**
 * @brief Get temperature resolution configuration.
 *
 * @param resolution Temperature resolution.
 * @return MCP9808_Error_t A number lower than '0' if something was wrong.
 */
MCP9808_Error_t MCP9808_GetResolution( MCP9808_Resolution_t* resolution )
{
    MCP9808_Error_t error = MCP9808_ERROR;
    uint8_t regData;

    error = MCP9808_PORT_Read(MCP9808_Address, MCP9808_REG_RESOLUTION,1, &regData);
    if ( !IS_MCP9808_ERROR(error) )
    {
        *resolution = regData &MCP9808_RESOLUTION_MSK;
    }

    return error;
}

/**
 * @brief Get the manufacturer ID (0x0054).
 *
 * @param id Manufacturer ID.
 * @return MCP9808_Error_t A number lower than '0' if something was wrong.
 */
MCP9808_Error_t MCP9808_GetManufactureID( uint16_t*id )
{
    MCP9808_Error_t error = MCP9808_ERROR;
    uint8_t regData[MCP9808_REG_SIZE];


    error = MCP9808_PORT_Read(MCP9808_Address,
                                MCP9808_REG_ID_1,
                                MCP9808_REG_SIZE, regData);
    if ( !IS_MCP9808_ERROR(error) )
    {
        *id = (regData[MCP9808_MSB] << 8) | regData[MCP9808_LSB];
    }

    return error;
}
