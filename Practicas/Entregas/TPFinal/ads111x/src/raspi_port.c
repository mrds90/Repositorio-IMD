/*=============================================================================
 * Author: Marcos Dominguez <mrds0690@gmail.com>
 * Date: 2021/10/08
 * Version: 1
 *===========================================================================*/

/*=====[Inclusions of function dependencies]=================================*/

#include "raspi_port.h"
#include "i2c_custom.h"

/*=====[Prototypes (declarations) of private functions]=======================*/
static void PORT_i2cInit(void);

static void PORT_i2cWrite(uint8_t i2cSlaveAddress, uint8_t i2c_register, uint16_t data);

static void PORT_i2cWriteRead(uint8_t i2cSlaveAddress, uint8_t i2c_register, uint16_t *read_data);

/*=====[Definitions of private global variables]=============================*/

/*=====[Implementations of public functions]==================================*/
ads111x_i2c_t PORT_Init(void) {
    ads111x_i2c_t i2c_ads111x;
    i2c_ads111x.Init = PORT_i2cInit;
    i2c_ads111x.Write = PORT_i2cWrite;
    i2c_ads111x.WriteRead = PORT_i2cWriteRead;

    return i2c_ads111x;
}

/*=====[Implementations of private functions]=================================*/

static void PORT_i2cInit(void) {}

static void PORT_i2cWrite(uint8_t i2cSlaveAddress, uint8_t i2c_register, uint16_t data) {
    uint8_t data_to_send[3];
    data_to_send[0] = i2c_register;
    data_to_send[1] = (uint8_t)((data >> 8) & 0xFF);
    data_to_send[2] = (uint8_t)(data & 0xFF);
    i2c_MasterSend(i2cSlaveAddress, data_to_send, 3);
}

static void PORT_i2cWriteRead(uint8_t i2cSlaveAddress, uint8_t i2c_register, uint16_t *read_data) {
    uint8_t dataToRead[2];
    if(i2c_MasterReceive(i2cSlaveAddress, i2c_register, dataToRead, 2) >= 0) {
        *read_data = (dataToRead[0] << 8) | dataToRead[1];
    }
}
