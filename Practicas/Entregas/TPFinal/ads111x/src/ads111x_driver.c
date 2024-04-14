/*=============================================================================
 * Author: Marcos Dominguez <mrds0690@gmail.com>
 * Date: 2021/10/08
 * Version: 1
 *===========================================================================*/

/*=====[Inclusions of function dependencies]=================================*/

#include "ads111x_driver.h"

/*=====[Definition macros of private constants]==============================*/
#define ADS111X_GENERAL_CALL_ADDR 0x00
#define ADS111X_CMD_RESET         0x06
#define ADS111X_HS_MASTER_CODE    0x03
#define ADS111X_HS_MODE           (0x08 | ADS111X_HS_MASTER_CODE)

#define ADS_111x_RESET_CONFIG    0x8583

#define ADS111X_CONFIG_FIELD_COMP_QUE_BIT       0x00
#define ADS111X_CONFIG_FIELD_COMP_LAT_BIT       0x02
#define ADS111X_CONFIG_FIELD_COMP_POL_BIT       0x03
#define ADS111X_CONFIG_FIELD_COMP_MODE_BIT      0x04
#define ADS111X_CONFIG_FIELD_DR_BIT             0x05
#define ADS111X_CONFIG_FIELD_MODE_BIT           0x08
#define ADS111X_CONFIG_FIELD_PGA_BIT            0x09
#define ADS111X_CONFIG_FIELD_MUX_BIT            0x0C
#define ADS111X_CONFIG_FIELD_OS_BIT             0x0F

#define ADS111X_CONFIG_FIELD_COMP_QUE_MASK (0x03 << ADS111X_CONFIG_FIELD_COMP_QUE_BIT)
#define ADS111X_CONFIG_FIELD_COMP_LAT_MASK (0x01 << ADS111X_CONFIG_FIELD_COMP_LAT_BIT)
#define ADS111X_CONFIG_FIELD_COMP_POL_MASK (0x01 << ADS111X_CONFIG_FIELD_COMP_POL_BIT)
#define ADS111X_CONFIG_FIELD_COMP_MODE_MASK (0x01 << ADS111X_CONFIG_FIELD_COMP_MODE_BIT)
#define ADS111X_CONFIG_FIELD_DR_MASK (0x07 << ADS111X_CONFIG_FIELD_DR_BIT)
#define ADS111X_CONFIG_FIELD_MODE_MASK (0x01 << ADS111X_CONFIG_FIELD_MODE_BIT)
#define ADS111X_CONFIG_FIELD_PGA_MASK (0x07 << ADS111X_CONFIG_FIELD_PGA_BIT)
#define ADS111X_CONFIG_FIELD_MUX_MASK (0x07 << ADS111X_CONFIG_FIELD_MUX_BIT)
#define ADS111X_CONFIG_FIELD_OS_MASK (0x01 << ADS111X_CONFIG_FIELD_OS_BIT)
/*=====[Definitions of private functions]====================================*/

static void ADS111x_WriteRegister(ads111x_addr_t i2c_address, ads111x_reg_t reg, uint16_t value);

static uint16_t ADS111x_ReadRegister(uint8_t i2c_address, uint8_t reg);

static void ADS111x_SetConfiguration(ads111x_obj_t *ptr_asd111x, ads111x_config_field_t field, uint8_t field_config);
/*=====[Definitions of extern global variables]==============================*/

/*=====[Definitions of public global variables]==============================*/

/*=====[Definitions of private global variables]=============================*/
static ads111x_i2c_t ads111x_i2c;
static const uint16_t ads111x_config_mask[ADS111X_CONFIG_FIELD_QTY] = {
    [ADS111X_CONFIG_FIELD_COMP_QUE] = ADS111X_CONFIG_FIELD_COMP_QUE_MASK,
    [ADS111X_CONFIG_FIELD_COMP_LAT] = ADS111X_CONFIG_FIELD_COMP_LAT_MASK,
    [ADS111X_CONFIG_FIELD_COMP_POL] = ADS111X_CONFIG_FIELD_COMP_POL_MASK,
    [ADS111X_CONFIG_FIELD_COMP_MODE] = ADS111X_CONFIG_FIELD_COMP_MODE_MASK,
    [ADS111X_CONFIG_FIELD_DR] = ADS111X_CONFIG_FIELD_DR_MASK,
    [ADS111X_CONFIG_FIELD_MODE] = ADS111X_CONFIG_FIELD_MODE_MASK,
    [ADS111X_CONFIG_FIELD_PGA] = ADS111X_CONFIG_FIELD_PGA_MASK,
    [ADS111X_CONFIG_FIELD_MUX] = ADS111X_CONFIG_FIELD_MUX_MASK,
    [ADS111X_CONFIG_FIELD_OS] = ADS111X_CONFIG_FIELD_OS_MASK
};
static const uint16_t ads111x_config_bit[ADS111X_CONFIG_FIELD_QTY] = {
    [ADS111X_CONFIG_FIELD_COMP_QUE] = ADS111X_CONFIG_FIELD_COMP_QUE_BIT,
    [ADS111X_CONFIG_FIELD_COMP_LAT] = ADS111X_CONFIG_FIELD_COMP_LAT_BIT,
    [ADS111X_CONFIG_FIELD_COMP_POL] = ADS111X_CONFIG_FIELD_COMP_POL_BIT,
    [ADS111X_CONFIG_FIELD_COMP_MODE] = ADS111X_CONFIG_FIELD_COMP_MODE_BIT,
    [ADS111X_CONFIG_FIELD_DR] = ADS111X_CONFIG_FIELD_DR_BIT,
    [ADS111X_CONFIG_FIELD_MODE] = ADS111X_CONFIG_FIELD_MODE_BIT,
    [ADS111X_CONFIG_FIELD_PGA] = ADS111X_CONFIG_FIELD_PGA_BIT,
    [ADS111X_CONFIG_FIELD_MUX] = ADS111X_CONFIG_FIELD_MUX_BIT,
    [ADS111X_CONFIG_FIELD_OS] = ADS111X_CONFIG_FIELD_OS_BIT
};
/*=====[Implementation of public functions]==================================*/

uint8_t ADS111x_Init(ads111x_obj_t *ptr_asd111x, ads111x_addr_t i2c_address, ads111x_pga_t gain, ads111x_device_t device, ads111x_i2c_t *port) {
    uint8_t result = ADS111x_CONFIG_ERROR;
    if (port != NULL) {
        ads111x_i2c = *port;
        port->Init();
    }
    if (ptr_asd111x!=NULL) {
        ptr_asd111x->device = device;
        ptr_asd111x->i2c_address = i2c_address;
        ptr_asd111x->configuration = ADS_111x_RESET_CONFIG & (~ADS111X_CONFIG_FIELD_OS_MASK);
        ADS111x_SetGain(ptr_asd111x, gain);
        result = ADS111x_CONFIG_SUCCESS;
    }

    return result;
}

void ADS111x_StartConversion(ads111x_obj_t *ptr_asd111x) {
    ADS111x_SetConfiguration(ptr_asd111x, ADS111X_CONFIG_FIELD_OS, ADS111X_START_CONVERSION);
    ptr_asd111x->configuration &=~ADS111X_CONFIG_FIELD_OS_MASK; // Clear OS bit to avoid unnecessary conversion cmd when a configuration is performed
}

int16_t ADS111x_Read(ads111x_obj_t *ptr_asd111x) {
    int16_t data = 0;
    data = ADS111x_ReadRegister(ptr_asd111x->i2c_address,ADS111X_CONVERSION_REG);   
    return data;
}


void ADS111x_SetThresholdLow(ads111x_obj_t *ptr_asd111x, uint16_t threshold) {
    ADS111x_WriteRegister(ptr_asd111x->i2c_address,ADS111X_LO_THRESH_REG,threshold);
}

void ADS111x_SetThresholdHigh(ads111x_obj_t *ptr_asd111x, uint16_t threshold) {
    ADS111x_WriteRegister(ptr_asd111x->i2c_address,ADS111X_HI_THRESH_REG,threshold);
}

void ADS111x_SetMultiplexer(ads111x_obj_t *ptr_asd111x, ads111x_mux_t mux) {
    ADS111x_SetConfiguration(ptr_asd111x, ADS111X_CONFIG_FIELD_MUX, mux);
}

void ADS111x_SetGain(ads111x_obj_t *ptr_asd111x, ads111x_pga_t gain) {
    ADS111x_SetConfiguration(ptr_asd111x, ADS111X_CONFIG_FIELD_PGA, gain);
}

void ADS111x_SetDataRate(ads111x_obj_t *ptr_asd111x, ads111x_data_rate_t dr) {
    ADS111x_SetConfiguration(ptr_asd111x, ADS111X_CONFIG_FIELD_DR, dr);
}

void ADS111x_SetMode(ads111x_obj_t *ptr_asd111x, ads111x_mode_t mode) {
    ADS111x_SetConfiguration(ptr_asd111x, ADS111X_CONFIG_FIELD_MODE, mode);
}

void ADS111x_SetComparatorMode(ads111x_obj_t *ptr_asd111x, ads111x_comp_mode_t mode) {
    if(ptr_asd111x->device != ADS1113) {
        ADS111x_SetConfiguration(ptr_asd111x, ADS111X_CONFIG_FIELD_COMP_MODE, mode);
    }
}

void ADS111x_SetComparatorPolarity(ads111x_obj_t *ptr_asd111x, ads111x_comp_pol_t pol) {
    if(ptr_asd111x->device != ADS1113) {
        ADS111x_SetConfiguration(ptr_asd111x, ADS111X_CONFIG_FIELD_COMP_POL, pol);
    }
}
void ADS111x_SetComparatorLatching(ads111x_obj_t *ptr_asd111x, ads111x_comp_lat_t lat) {
    if(ptr_asd111x->device != ADS1113) {
        ADS111x_SetConfiguration(ptr_asd111x, ADS111X_CONFIG_FIELD_COMP_LAT, lat);
    }
}

void ADS111x_SetComparatorQueue(ads111x_obj_t *ptr_asd111x, ads111x_comp_que_t que) {
    if(ptr_asd111x->device != ADS1113) {
        ADS111x_SetConfiguration(ptr_asd111x, ADS111X_CONFIG_FIELD_COMP_QUE, que);
    }
}

/*=====[Implementations of private functions]===============================*/

static void ADS111x_WriteRegister(ads111x_addr_t i2c_address, ads111x_reg_t reg, uint16_t value) {
    ads111x_i2c.Write(i2c_address, reg, value);
}

static uint16_t ADS111x_ReadRegister(uint8_t i2c_address, uint8_t reg) {
    uint16_t value = 0;
    ads111x_i2c.WriteRead(i2c_address, reg, &value);
    return value;
  
}

static void ADS111x_SetConfiguration(ads111x_obj_t *ptr_asd111x, ads111x_config_field_t field, uint8_t field_config) {
    if (field_config <= (ads111x_config_mask[field]>>ads111x_config_bit[field])) {
        ptr_asd111x->configuration &= ~(ads111x_config_mask[field]);
        ptr_asd111x->configuration |= field_config << ads111x_config_bit[field];
        ADS111x_WriteRegister(ptr_asd111x->i2c_address,ADS111X_CONFIG_REG,ptr_asd111x->configuration);
    }
}
