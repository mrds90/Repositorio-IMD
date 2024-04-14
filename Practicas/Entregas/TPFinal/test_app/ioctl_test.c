#include <stdio.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdint.h>

int main(void)
{
    /* First you need run "mknod /dev/mydev c 202 0" to create /dev/mydev */
    int ads111x_dev = open("/dev/ads111x00", O_RDWR);

    if (ads111x_dev < 0)
    {
        perror("Fail to open device file: /dev/ads111x00.");
    }
    else
    {
        int ret = 0;
        uint8_t ads111x_reg[2] = {0x1, 0};
        ret = read(ads111x_dev, &ads111x_reg, 2);
        if (ret < 0) {
            printf("No se pudo leer el registro satisfactoriamente!\n");
        }
        close(ads111x_dev);
    }

    return 0;
}
