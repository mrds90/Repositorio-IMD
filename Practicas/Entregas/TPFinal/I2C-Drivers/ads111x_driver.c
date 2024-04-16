#include <linux/module.h>
#include <linux/miscdevice.h>
#include <linux/i2c.h>
#include <linux/fs.h>
#include <linux/of.h>
#include <linux/uaccess.h>

/*========= [PRIVATE DEVICE STRUCTURE] =========================================*/

struct ads111x_dev {
    struct i2c_client *client;
    struct miscdevice ads111x_misc_device;
    char name[sizeof("ads111xXX")]; /* ads111xXX */
};

/*========= [DEVICE TREE IDENTIFIERS] ==========================================*/

static const struct of_device_id ads111x_dt_ids[] = {
    { .compatible = "mse,ads111x", },
    { /* sentinel */ }
};
MODULE_DEVICE_TABLE(of, ads111x_dt_ids);

/*========= [I2C HELPER FUNCTIONS] =============================================*/

static int i2c_read_reg(struct i2c_client *client, uint8_t reg, uint8_t *data, size_t len) {
    struct i2c_msg msgs[2];

    uint8_t buf[1] = {reg};
    
    msgs[0].addr = client->addr;
    msgs[0].flags = 0;
    msgs[0].buf = buf;
    msgs[0].len = sizeof(buf);

    msgs[1].addr = client->addr;
    msgs[1].flags = I2C_M_RD;
    msgs[1].buf = data;
    msgs[1].len = len;

    return i2c_transfer(client->adapter, msgs, 2);
}

static int i2c_write_reg(struct i2c_client *client, char *data, size_t len) {
    struct i2c_msg msg;
    
    if (len > 3) {
        pr_err("Data length greater than buffer size\n");
        return -EINVAL;
    }
    
    msg.addr = client->addr;
    msg.flags = 0;
    msg.buf = data;
    msg.len = len;
    
    return i2c_transfer(client->adapter, &msg, 1);
}

/*========= [FILE OPERATIONS] ===================================================*/

static ssize_t ads111x_read(struct file *file, char __user *userbuf, size_t len, loff_t *ppos) {
    struct ads111x_dev *ads111x;
    uint8_t reg;
    uint8_t data[2];
    int ret;
    
    if (len != sizeof(data)) {
        pr_err("Invalid read size\n");
        return -EINVAL;
    }

    if (copy_from_user(&reg, userbuf, sizeof(reg)) != 0) {
        pr_err("Failed to copy register from user space\n");
        return -EFAULT;
    }

    ads111x = container_of(file->private_data, struct ads111x_dev, ads111x_misc_device);
    ret = i2c_read_reg(ads111x->client, reg, data, sizeof(data));
    if (ret < 0) {
        pr_err("Error reading register: %d\n", ret);
        return ret;
    }

    if (copy_to_user(userbuf, data, sizeof(data)) != 0) {
        pr_err("Failed to copy data to user space\n");
        return -EFAULT;
    }

    return sizeof(data);
}

static ssize_t ads111x_write(struct file *file, const char __user *userbuf, size_t len, loff_t *offset) {
    struct ads111x_dev *ads111x;
    char buf[3];
    int ret;

    if (len > sizeof(buf)) {
        pr_err("Data length greater than buffer size\n");
        return -EINVAL;
    }

    if (copy_from_user(buf, userbuf, len) != 0) {
        pr_err("Failed to copy data from user space\n");
        return -EFAULT;
    }

    ads111x = container_of(file->private_data, struct ads111x_dev, ads111x_misc_device);
    ret = i2c_write_reg(ads111x->client, buf, len);
    if (ret < 0) {
        pr_err("Error writing register: %d\n", ret);
        return ret;
    }

    return len;
}


static long ads111x_ioctl(struct file *file, unsigned int cmd, unsigned long arg) {
    struct ads111x_dev *ads111x;
    ads111x = container_of(file->private_data, struct ads111x_dev, ads111x_misc_device);
    /*
     * Here will be the calls to i2c_transfer() as appropriate
     * passing ads111x->client as the device
     */
    pr_info("Ads111xIoctl() was invoked. cmd = %d, arg = %ld\n", cmd, arg);
    return 0;
}

static const struct file_operations ads111x_fops = {
    .owner = THIS_MODULE,
    .read = ads111x_read,
    .write = ads111x_write,
    .unlocked_ioctl = ads111x_ioctl,
};

/*========= [DRIVER PROBE AND REMOVE FUNCTIONS] ================================*/

static int ads111x_probe(struct i2c_client *client, const struct i2c_device_id *id) {
    struct ads111x_dev *ads111x;
    static int counter = 0;
    int ret_val;

    ads111x = devm_kzalloc(&client->dev, sizeof(struct ads111x_dev), GFP_KERNEL);
    if (!ads111x)
        return -ENOMEM;
        
    i2c_set_clientdata(client, ads111x);
    ads111x->client = client;

    snprintf(ads111x->name, sizeof(ads111x->name), "ads111x%02d", counter++);

    ads111x->ads111x_misc_device.name = ads111x->name;
    ads111x->ads111x_misc_device.minor = MISC_DYNAMIC_MINOR;
    ads111x->ads111x_misc_device.fops = &ads111x_fops;

    ret_val = misc_register(&ads111x->ads111x_misc_device);
    if (ret_val != 0) {
        pr_err("Failed to register device %s\n", ads111x->ads111x_misc_device.name);
        return ret_val;
    }

    pr_info("Device %s: assigned minor: %i\n", ads111x->ads111x_misc_device.name, ads111x->ads111x_misc_device.minor);

    return 0;
}

static void ads111x_remove(struct i2c_client *client) {
    struct ads111x_dev *ads111x;
    ads111x = i2c_get_clientdata(client);
    misc_deregister(&ads111x->ads111x_misc_device);
}

/*========= [DRIVER STRUCTURES] ================================================*/

static struct i2c_driver ads111x_driver = {
    .probe = ads111x_probe,
    .remove = ads111x_remove,
    .driver = {
        .name = "ads111x_driver",
        .owner = THIS_MODULE,
        .of_match_table = of_match_ptr(ads111x_dt_ids),
    },
};

/*========= [MODULE INITIALIZATION AND CLEANUP] ================================*/

module_i2c_driver(ads111x_driver);

MODULE_AUTHOR("Marcos Dominguez <mrds0690@gmail.com>");
MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("This module is a driver for IMD");
MODULE_INFO(ads111x_imd, "This is not for mere mortals");
