#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include "ads111x_driver.h"
#include "raspi_port.h"

/*====================================================================================================*/


#define MAX_TEMP_D_CELSIUS 500
#define TEMP_2_D_CELSIUS 20
#define TEMP_1_D_CELSIUS 10
#define TIME_TO_WAIT_FOR_CONV_IN_MS 1000

/*====================================================================================================*/

/**
 * @brief Initialize the ADC configuration for this custom application
 * 
 */
static void InitADS111xDriver(void); // Ahora solo una declaración estática aquí

/**
 * @brief Convert the raw data from the ADC to a temperature in Celsius
 * 
 * @param counts     The raw data from the ADC
 * @return uint16_t  The temperature in Celsius
 */
static uint16_t ConutsToTemp(uint16_t counts);

/**
 * @brief Convert temperature in Celsius to a raw data for the ADC
 * 
 * @param temp       Temperature in Celsius
 * @return uint16_t  The raw data for the ADC
 */
static uint16_t TempToCounts(uint16_t temp);

/**
 * @brief Set the New Set Treshold value
 * 
 * @param new_setpoint  The new setpoint value in Celsius
 */
static void SetNewSetTreshold(uint16_t new_setpoint);

static void readADC(void);

/*====================================================================================================*/

static ads111x_obj_t ads1115_0; ///< instance of ADS1115

static uint16_t temperature_setpoint = 240; ///< The default temperature setpoint in deci Celsius

/*====================================================================================================*/


int main(void)
{
    InitADS111xDriver();  // Inicializa el ADC
    
    while (1) {
        readADC();  // Realiza una lectura del ADC en cada iteración del bucle
    }
    
    return 0;
}

/*====================================================================================================*/

static void InitADS111xDriver(void) {
    ads111x_i2c_t ads111x_port = PORT_Init();
    ADS111x_Init(&ads1115_0, ADS111X_ADDR_0, ADS111X_PGA_4096, ADS1115, &ads111x_port); ///< Initialize the ADS1115 with address 0x48, PGA = 4096, and the ADS1115 as the device
    ADS111x_SetDataRate(&ads1115_0, ADS111X_DATA_RATE_16SPS); ///< Set the data rate to 16 samples per second
    ADS111x_SetMultiplexer(&ads1115_0, ADS111X_MUX_AN0_GND); ///< Select the Analog input to be AN0
    ADS111x_SetMode(&ads1115_0, ADS111X_MODE_SINGLE); ///< Set the mode to single shot
    ADS111x_SetComparatorQueue(&ads1115_0, ADS111X_COMP_QUE_2_CONV); ///< Set the comparator queue to 2 conversions
    SetNewSetTreshold(temperature_setpoint); ///< Set the initial setpoint
    ADS111x_StartConversion(&ads1115_0); ///< First conversion
}

static uint16_t ConutsToTemp(uint16_t counts) {
   return (counts * MAX_TEMP_D_CELSIUS) / 0x7FFF; //!< Convert the raw data to a temperature in Celsius
}

static uint16_t TempToCounts(uint16_t temp) {
   return (temp * 0x7FFF) / MAX_TEMP_D_CELSIUS; //!< Convert the temperature in Celsius to a raw data for the ADC
}

static void SetNewSetTreshold(uint16_t new_setpoint) {
   uint16_t thesh_lo = TempToCounts(new_setpoint - TEMP_2_D_CELSIUS); //!< Set the low threshold to 2 degrees below the setpoint
   uint16_t thesh_hi = TempToCounts(new_setpoint + TEMP_2_D_CELSIUS)  ; //!< Set the high threshold to 2 degrees above the setpoints
   ADS111x_SetThresholdLow(&ads1115_0, thesh_lo);
   ADS111x_SetThresholdHigh(&ads1115_0, thesh_hi);
}

static void readADC(void) {
    uint16_t adc_data = ADS111x_Read(&ads1115_0); // Lee los datos del ADC
    uint16_t temperature = ConutsToTemp(adc_data); // Convierte los datos del ADC a temperatura
    
    // Imprime la temperatura
    printf("Temperature: %d Celsius\n", temperature);
    
    // Espera un tiempo antes de la próxima lectura del ADC
    // Esto puede variar dependiendo de tus requisitos
    // En este ejemplo, esperamos 1 segundo
    sleep(1);
}
