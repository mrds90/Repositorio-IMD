/*=============================================================================
 * Author: Marcos Dominguez <mrds0690@gmail.com>
 * Date: 2021/10/08
 * Version: 1
 *===========================================================================*/

/*=====[Inclusions of function dependencies]=================================*/

#include "raspi_port.h"
#include <sys/ioctl.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdint.h>
#include <stddef.h>

/*=====[Prototypes (declarations) of private functions]=======================*/

static void PORT_i2cInit(void);

static int PORT_i2cWrite(uint8_t i2cSlaveAddress, uint8_t i2c_register, uint16_t data);

static int PORT_i2cWriteRead(uint8_t i2cSlaveAddress, uint8_t i2c_register, uint16_t *read_data);

static __always_inline int MasterSend(uint8_t slaveAddr, const uint8_t *buff, uint8_t len);

static __always_inline int MasterReceive(uint8_t slaveAddr, uint8_t reg, uint8_t *buff, uint8_t len);

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

static int PORT_i2cWrite(uint8_t i2cSlaveAddress, uint8_t i2c_register, uint16_t data) {
    uint8_t data_to_send[3];
    data_to_send[0] = i2c_register;
    data_to_send[1] = (uint8_t)((data >> 8) & 0xFF);
    data_to_send[2] = (uint8_t)(data & 0xFF);
    return MasterSend(i2cSlaveAddress, data_to_send, 3);
}

static int PORT_i2cWriteRead(uint8_t i2cSlaveAddress, uint8_t i2c_register, uint16_t *read_data) {
    uint8_t dataToRead[2];
    int ret = MasterReceive(i2cSlaveAddress, i2c_register, dataToRead, 2);
    if (ret >= 0) {
        *read_data = (dataToRead[0] << 8) | dataToRead[1];
    }
    return ret;
}

static __always_inline int MasterSend(uint8_t slaveAddr, const uint8_t *buff, uint8_t len) {
    int ads111x_dev = open("/dev/ads111x00", O_WRONLY);
    int ret = 0;

    if (ads111x_dev < 0) {
        perror("Fail to open device file: /dev/ads111x00.");
        ret = -1;
    }
    else {
        ret = write(ads111x_dev, buff, len);
        close(ads111x_dev);
    }

    return ret;
}

static __always_inline int MasterReceive(uint8_t slaveAddr, uint8_t reg, uint8_t *buff, uint8_t len) {
    int ads111x_dev = open("/dev/ads111x00", O_RDWR);
    int ret = 0;

    if (ads111x_dev < 0) {
        perror("Fail to open device file: /dev/ads111x00.");
        ret = -1;
    }
    else {
        buff[0] = reg;
        ret = read(ads111x_dev, buff, len);
        close(ads111x_dev);
    }

    return ret;
}
