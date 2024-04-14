#include <linux/module.h>
#include <linux/miscdevice.h>
#include <linux/i2c.h>
#include <linux/fs.h>
#include <linux/of.h>
#include <linux/string.h>

/* Private device structure */
struct mse_dev
{
	struct i2c_client *client;
	struct miscdevice mse_miscdevice;
	char name[9]; /* msedrvXX */
};

/*
 * Definicion de los ID correspondientes al Device Tree. Estos deben ser informados al
 * kernel mediante la macro MODULE_DEVICE_TABLE
 *
 * NOTA: Esta seccion requiere que CONFIG_OF=y en el kernel
 */

static const struct of_device_id mse_dt_ids[] =
{
    { .compatible = "mse,ads111x", },
    { /* sentinel */ }
};

MODULE_DEVICE_TABLE(of, mse_dt_ids);

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

    return i2c_transfer (client->adapter, msgs, 2);
}

static int i2c_write_reg(struct i2c_client *client, const char *data, size_t len) {

    struct i2c_msg msgs[1];
    
    static char buff_to_send[255];
    if(len>255){
        return -1;
    }
    else {
        memcpy(buff_to_send, data, len);
        msgs[0].addr = client->addr;
        msgs[0].flags = 0;
        msgs[0].buf = buff_to_send;
        msgs[0].len = len;

        return i2c_transfer (client->adapter, msgs, 1);
    }
}

/* User is reading data from /dev/msedrvXX */
static ssize_t ads111x_read(struct file *file, char __user *userbuf, size_t count, loff_t *ppos) {

    struct mse_dev *mse;

    int ret = 0;

    uint8_t reg = userbuf[0];
    if(count > 2) {
        pr_err("Tamaño del registro solicitado mayor a 2");
    }
    else {
        mse = container_of(file->private_data, struct mse_dev, mse_miscdevice);

        ret = i2c_read_reg(mse->client, reg, userbuf, count);
        if (ret < 0)
        {
            pr_err("Error al leer el registro: %d\n",ret);
        }
    }
    

    return ret;
}

static ssize_t ads111x_write(struct file *file, const char __user *buffer, size_t len, loff_t *offset)
{
    struct mse_dev *mse;

    int ret = 0;

    mse = container_of(file->private_data, struct mse_dev, mse_miscdevice);

    ret = i2c_write_reg(mse->client, buffer, len);
    if (ret < 0)
    {
        pr_err("Error al escribir el registro: %d\n",ret);
    }
    return ret;
}

static long mse_ioctl(struct file *file, unsigned int cmd, unsigned long arg)
{
    struct mse_dev *mse;

    mse = container_of(file->private_data, struct mse_dev, mse_miscdevice);

    /*
     * Aqui ira las llamadas a i2c_transfer() que correspondan pasando
     * como dispositivo mse->client
    */

    pr_info("mse_ioctl() fue invocada. cmd = %d, arg = %ld\n", cmd, arg);
    return 0;
}

static int mse_open(struct inode *inode, struct file *file)
{
    pr_info("mse_open() is called.\n");
    return 0;
};

static int mse_close(struct inode *inode, struct file *file)
{
    pr_info("mse_close() is called.\n");
    return 0;
};

/* declaracion de una estructura del tipo file_operations */

static const struct file_operations mse_fops =
{
    .owner = THIS_MODULE,
    .read = ads111x_read,
    .write = ads111x_write,
    .open = mse_open,
    .release = mse_close,
    .unlocked_ioctl = mse_ioctl,
};

/*--------------------------------------------------------------------------------*/

static int mse_probe(struct i2c_client *client, const struct i2c_device_id *id)
{
    struct mse_dev *mse;
    static int counter = 0;
    int ret_val;

    /* Allocate new private structure */
    mse = devm_kzalloc(&client->dev, sizeof(struct mse_dev), GFP_KERNEL);

    /* Store pointer to the device-structure in bus device context */
    i2c_set_clientdata(client,mse);

	/* Store pointer to I2C client device in the private structure */
	mse->client = client;

    /* Initialize the misc device, mse is incremented after each probe call */
    sprintf(mse->name, "ads111x%02d", counter++);

    mse->mse_miscdevice.name = mse->name;
    mse->mse_miscdevice.minor = MISC_DYNAMIC_MINOR;
    mse->mse_miscdevice.fops = &mse_fops;

    /* Register misc device */
    ret_val = misc_register(&mse->mse_miscdevice);

    if (ret_val != 0)
    {
        pr_err("No se pudo registrar el dispositivo %s\n", mse->mse_miscdevice.name);
        return ret_val;
    }

    pr_info("Dispositivo %s: minor asignado: %i\n", mse->mse_miscdevice.name, mse->mse_miscdevice.minor);

    return 0;
}

static void mse_remove(struct i2c_client * client)
{
    struct mse_dev * mse;

    /* Get device structure from bus device context */
    mse = i2c_get_clientdata(client);

    /* Deregister misc device */
    misc_deregister(&mse->mse_miscdevice);
}

/*--------------------------------------------------------------------------------*/

static struct i2c_driver mse_driver =
{
    .probe= mse_probe,
    .remove= mse_remove,
    .driver =
    {
        .name = "ads111x_driver",
        .owner = THIS_MODULE,
        .of_match_table = of_match_ptr(mse_dt_ids),
    },
};

/*----------------------------------------------------------------------*/



/**********************************************************************
 * Esta seccion define cuales funciones seran las ejecutadas al cargar o
 * remover el modulo respectivamente. Es hecho implicitamente,
 * termina declarando init() exit()
 **********************************************************************/
module_i2c_driver(mse_driver);

/**********************************************************************
 * Seccion sobre Informacion del modulo
 **********************************************************************/
MODULE_AUTHOR("Marcos Dominguez <mrds0690@gmail.com>");
MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Este modulo es un driver para IMD");
MODULE_INFO(mse_imd, "Esto no es para simples mortales");

