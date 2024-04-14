/*=============================================================================
 * Author: Marcos Dominguez <mrds0690@gmail.com>
 * Date: 2021/07/02
 * Version: 1
 *===========================================================================*/

/*=====[Avoid multiple inclusion - begin]====================================*/

#ifndef __ADS111X_DRIVER_H__
#define __ADS111X_DRIVER_H__

/*=====[Inclusions of public function dependencies]==========================*/

#include <stdint.h>
#include <stddef.h>


/*=====[C++ - begin]=========================================================*/

#ifdef __cplusplus
extern "C" {
#endif

/*=====[Definition macros of public constants]===============================*/

#define ADS111x_CONFIG_ERROR      0xFF
#define ADS111x_CONFIG_SUCCESS    0x00


/*=====[Public function-like macros]=========================================*/


/*=====[Definitions of public data types]====================================*/
typedef void (*I2C_INITIALIZATION)(void);
typedef void (*I2C_WRITE)(uint8_t, uint8_t, uint16_t);
typedef void (*I2C_READ)(uint8_t, uint8_t, uint16_t*);

/**
 * @brief ADS111x Hal driver
 * 
 */
typedef struct {
    I2C_INITIALIZATION Init;
    I2C_WRITE Write; // Write function pointer (addr,reg, data)
    I2C_READ  WriteRead; // WriteRead function pointer (addt, reg, *data)
} ads111x_i2c_t;

/**
 * @brief ADS111x device options
 * 
 */
typedef enum {
    ADS1113 = 1,
    ADS1114,
    ADS1115
} ads111x_device_t;

/**
 * @brief ADS111x address options
 * 
 */
typedef enum {
    ADS111X_ADDR_0 = 0x48, 
    ADS111X_ADDR_1 = 0x49,
    ADS111X_ADDR_2 = 0x4A,
    ADS111X_ADDR_3 = 0x4B,
} ads111x_addr_t;

/**
 * @brief ADS111x registers
 * 
 */
typedef enum {
    ADS111X_CONVERSION_REG = 0x00,
    ADS111X_CONFIG_REG = 0x01,
    ADS111X_LO_THRESH_REG = 0x02,
    ADS111X_HI_THRESH_REG = 0x03,
} ads111x_reg_t;
/**
 * @brief ADS111x gain options
 * 
 */
typedef enum {
    ADS111X_PGA_6144 = 0x00,
    ADS111X_PGA_4096 = 0x01,
    ADS111X_PGA_2048 = 0x02,
    ADS111X_PGA_1024 = 0x03,
    ADS111X_PGA_0512 = 0x04,
    ADS111X_PGA_0256 = 0x05,
} ads111x_pga_t;

/**
 * @brief ADS111x conversion options
 * 
 */
typedef enum {
    ADS111X_MODE_CONTINUOUS = 0x00,
    ADS111X_MODE_SINGLE = 0x01,
} ads111x_mode_t;
/**
 * @brief ADS111x conversion rate options
 * 
 */
typedef enum {
    ADS111X_DATA_RATE_8SPS = 0x00,
    ADS111X_DATA_RATE_16SPS = 0x01,
    ADS111X_DATA_RATE_32SPS = 0x02,
    ADS111X_DATA_RATE_64SPS = 0x03,
    ADS111X_DATA_RATE_128SPS = 0x04,
    ADS111X_DATA_RATE_250SPS = 0x05,
    ADS111X_DATA_RATE_475SPS = 0x06,
    ADS111X_DATA_RATE_860SPS = 0x07,
} ads111x_data_rate_t;

/**
 * @brief ADS111x comparator mode options
 * 
 */
typedef enum {
    ADS111X_COMP_MODE_TRADITIONAL = 0x00,
    ADS111X_COMP_MODE_WINDOW = 0x01,
} ads111x_comp_mode_t;

/**
 * @brief ADS111x comparator polarity options
 * 
 */
typedef enum {
    ADS111X_COMP_POL_ACTIVE_LOW = 0x00,
    ADS111X_COMP_POL_ACTIVE_HIGH = 0x01,
} ads111x_comp_pol_t;

/**
 * @brief ADS111x comparator latch options
 * 
 */
typedef enum {
    ADS111X_COMP_LAT_NON_LATCHING = 0x00,
    ADS111X_COMP_LAT_LATCHING = 0x01,
} ads111x_comp_lat_t;
/**
 * @brief ADS111x comparator queue options
 * 
 */
typedef enum {
    ADS111X_COMP_QUE_1_CONV = 0x00,
    ADS111X_COMP_QUE_2_CONV = 0x01,
    ADS111X_COMP_QUE_4_CONV = 0x02,
    ADS111X_COMP_QUE_DISABLE = 0x03,
} ads111x_comp_que_t;

/**
 * @brief ADS111x Analog input options
 * 
 */
typedef enum {
    ADS111X_MUX_AN0_AN1 = 0x00,
    ADS111X_MUX_AN0_AN3 = 0x01,
    ADS111X_MUX_AN1_AN3 = 0x02,
    ADS111X_MUX_AN2_AN3 = 0x03,
    ADS111X_MUX_AN0_GND = 0x04,
    ADS111X_MUX_AN1_GND = 0x05,
    ADS111X_MUX_AN2_GND = 0x06,
    ADS111X_MUX_AN3_GND = 0x07,
} ads111x_mux_t;

/**
 * @brief ADS111x start conversion order
 * 
 */
typedef enum {
    ADS111X_DO_NOTHING = 0x00,
    ADS111X_START_CONVERSION = 0x01,
} ads111x_os_t;

/**
 * @brief ADS111x configuration fields
 * 
 */
typedef enum {
    ADS111X_CONFIG_FIELD_COMP_QUE,
    ADS111X_CONFIG_FIELD_COMP_LAT,
    ADS111X_CONFIG_FIELD_COMP_POL,
    ADS111X_CONFIG_FIELD_COMP_MODE,
    ADS111X_CONFIG_FIELD_DR,
    ADS111X_CONFIG_FIELD_MODE,
    ADS111X_CONFIG_FIELD_PGA,
    ADS111X_CONFIG_FIELD_MUX,
    ADS111X_CONFIG_FIELD_OS,

    ADS111X_CONFIG_FIELD_QTY,
}ads111x_config_field_t;

/**
 * @brief ADS111x configuration structure
 * 
 */
typedef struct {
    uint16_t configuration;
    ads111x_device_t device;
    ads111x_addr_t i2c_address;
} ads111x_obj_t;

/*=====[Prototypes (declarations) of public functions]=======================*/
/**
 * @brief Iinitialize ADS111x device with default values except for the device type, gain and device address
 * 
 * @param ptr_asd111x ADS111x device object
 * @param i2c_address ADS111x device address
 * @param gain        ADS111x gain
 * @param device      ADS111x device type
 * @param port        Driver comunication functions
 * @return uint8_t 
 */
uint8_t ADS111x_Init(ads111x_obj_t *ptr_asd111x, ads111x_addr_t i2c_address, ads111x_pga_t gain, ads111x_device_t device, ads111x_i2c_t *port);
/**
 * @brief Start conversion order on ADS111x device
 * 
 * @param ptr_asd111x ADS111x device object
 */
void ADS111x_StartConversion(ads111x_obj_t *ptr_asd111x);
/**
 * @brief Read conversion result from ADS111x device
 * 
 * @param ptr_asd111x ADS111x device object
 * @return int16_t    Result of conversion
 */
int16_t ADS111x_Read(ads111x_obj_t *ptr_asd111x);
/**
 * @brief ADS111x low threshold used in comparator mode
 * 
 * @param ptr_asd111x  ADS111x device object
 * @param threshold    Threshold value
 */
void ADS111x_SetThresholdLow(ads111x_obj_t *ptr_asd111x, uint16_t threshold);
/**
 * @brief ADS111x high threshold used in comparator mode
 * 
 * @param ptr_asd111x   ADS111x device object
 * @param threshold     Threshold value
 */
void ADS111x_SetThresholdHigh(ads111x_obj_t *ptr_asd111x, uint16_t threshold);
/**
 * @brief ADS111x Analog input selection
 * 
 * @param ptr_asd111x   ADS111x device object
 * @param mux           Analog input selection value (see ads111x_mux_t)
 */
void ADS111x_SetMultiplexer(ads111x_obj_t *ptr_asd111x, ads111x_mux_t mux);
/**
 * @brief ADS111x Gain selection
 * 
 * @param ptr_asd111x   ADS111x device object
 * @param gain          Gain selection value (see ads111x_pga_t)
 */
void ADS111x_SetGain(ads111x_obj_t *ptr_asd111x, ads111x_pga_t gain);
/**
 * @brief ADS111x Sample rate selection
 * 
 * @param ptr_asd111x   ADS111x device object
 * @param dr            Sample rate selection value (see ads111x_dr_t)
 */
void ADS111x_SetDataRate(ads111x_obj_t *ptr_asd111x, ads111x_data_rate_t dr);
/**
 * @brief ADS111x Operating mode selection
 * 
 * @param ptr_asd111x   ADS111x device object
 * @param mode          Single-shot or continuous mode selection value (see ads111x_mode_t)
 */
void ADS111x_SetMode(ads111x_obj_t *ptr_asd111x, ads111x_mode_t mode);
/**
 * @brief ADS111x Comparator mode selection
 * 
 * @param ptr_asd111x   ADS111x device object
 * @param mode          Comparator mode selection value (see ads111x_comp_mode_t)
 */
void ADS111x_SetComparatorMode(ads111x_obj_t *ptr_asd111x, ads111x_comp_mode_t mode);
/**
 * @brief ADS111x Comparator polarity selection
 * 
 * @param ptr_asd111x   ADS111x device object
 * @param polarity      Comparator polarity selection value (see ads111x_comp_pol_t)
 */
void ADS111x_SetComparatorPolarity(ads111x_obj_t *ptr_asd111x, ads111x_comp_pol_t pol);
/**
 * @brief ADS111x Comparator Latch selection
 * 
 * @param ptr_asd111x   ADS111x device object
 * @param lat           Comparator Latch selection value (see ads111x_comp_lat_t)
 */
void ADS111x_SetComparatorLatching(ads111x_obj_t *ptr_asd111x, ads111x_comp_lat_t lat);
/**
 * @brief ADS111x Comparator queue selection
 * 
 * @param ptr_asd111x   ADS111x device object
 * @param queue         Comparator queue selection value (see ads111x_comp_queue_t)
 */
void ADS111x_SetComparatorQueue(ads111x_obj_t *ptr_asd111x, ads111x_comp_que_t que);

/*=====[C++ - end]===========================================================*/

#ifdef __cplusplus
}
#endif

/*=====[Avoid multiple inclusion - end]======================================*/

#endif /* __ADS111X_DRIVER_H__ */
