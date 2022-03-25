
# MCP9808 C Driver

MCP9808 temperature sensor driver implemetation written in C. This driver can be ported to different platforms implementing the I2C I/O functions defined in `MCP9808_port.h` file. These functions must be defined with the following prototypes:

```
MCP9808_Error_t MCP9808_PORT_Init( void );

MCP9808_Error_t MCP9808_PORT_Read(uint8_t address, uint8_t reg, uint8_t size, uint8_t* data);

MCP9808_Error_t MCP9808_PORT_Write(uint8_t address, uint8_t reg, uint8_t size, uint8_t* data);

```

# How to use it

```
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
```
