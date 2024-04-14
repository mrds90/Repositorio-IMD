/*=============================================================================
 * Author: Marcos Dominguez <mrds0690@gmail.com>
 * Date: 2021/10/08
 * Version: 1
 *===========================================================================*/

/*=====[Inclusions of function dependencies]=================================*/

#include <sys/ioctl.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdint.h>
#include "i2c_custom.h"


/*=====[Definition macros of private constants]==============================*/

/*=====[Definitions of private functions]====================================*/

/*=====[Definitions of extern global variables]==============================*/

/*=====[Definitions of public global variables]==============================*/

/*=====[Definitions of private global variables]=============================*/

/*=====[Implementation of public functions]==================================*/

int i2c_MasterSend(uint8_t slaveAddr, const uint8_t *buff, uint8_t len) {
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

int i2c_MasterReceive(uint8_t slaveAddr, uint8_t reg, uint8_t *buff, uint8_t len) {
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

/*=====[Implementations of private functions]================================*/

/*=====[Implementations of interrupt functions]==============================*/
