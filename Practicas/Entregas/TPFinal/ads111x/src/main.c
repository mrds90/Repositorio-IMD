#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include "ads111x_driver.h"
#include "raspi_port.h"

/*====================================================================================================*/

/*====================================================================================================*/

// Declaración de funciones estáticas
static void InitADS111xDriver(void);
static void readADC(void);
static void printMenu(void);
static void handleMenuOption(int option);
static void changeDataRate(void);
static void changeMultiplexer(void);
static void changeMode(void);

// Variables globales
static ads111x_obj_t ads1115_0;
static uint8_t single_mode = 1;

int main(void)
{
    InitADS111xDriver(); // Inicializar el ADC

    int option;
    do {
        printMenu(); // Mostrar menú
        printf("Ingrese la opción deseada: ");
        scanf("%d", &option); // Leer la opción del usuario
        getchar(); // Consumir el carácter de nueva línea

        handleMenuOption(option); // Manejar la opción del menú

    } while (option != 0); // Salir del bucle si el usuario elige la opción 0

    return 0;
}

// Función para inicializar el driver ADS111x
static void InitADS111xDriver(void)
{
    ads111x_i2c_t ads111x_port = PORT_Init();
    ADS111x_Init(&ads1115_0, ADS111X_ADDR_0, ADS111X_PGA_4096, ADS1115, &ads111x_port);
    ADS111x_SetDataRate(&ads1115_0, ADS111X_DATA_RATE_16SPS);
    ADS111x_SetMultiplexer(&ads1115_0, ADS111X_MUX_AN0_GND);
    ADS111x_SetMode(&ads1115_0, ADS111X_MODE_SINGLE);
    single_mode = 1;
    ADS111x_SetComparatorQueue(&ads1115_0, ADS111X_COMP_QUE_2_CONV);
    ADS111x_StartConversion(&ads1115_0);
}

// Función para realizar una lectura del ADC
static void readADC(void)
{   
    if(single_mode) {
        ADS111x_StartConversion(&ads1115_0);
        sleep(1);
    }
    uint16_t adc_data = ADS111x_Read(&ads1115_0);
    printf("\n\nData from ADC: %X \n\n", adc_data);
}

// Función para imprimir el menú de opciones
static void printMenu(void)
{
    printf("\n--- Menú de Opciones ---\n");
    printf("1. Realizar una lectura del ADC\n");
    printf("2. Cambiar el data rate\n");
    printf("3. Cambiar el multiplexer\n");
    printf("4. Cambiar el modo de operación\n");
    printf("0. Salir\n");
}

// Función para manejar la opción seleccionada por el usuario
static void handleMenuOption(int option)
{
    switch (option)
    {
    case 1:
        readADC(); // Realizar una lectura del ADC
        break;
    case 2:
        changeDataRate(); // Cambiar el data rate
        break;
    case 3:
        changeMultiplexer(); // Cambiar el multiplexer
        break;
    case 4:
        changeMode(); // Cambiar el modo de operación
        break;
    case 0:
        printf("Saliendo...\n");
        break;
    default:
        printf("Opción no válida. Por favor, intente de nuevo.\n");
        break;
    }
}

// Función para cambiar el data rate
static void changeDataRate(void)
{
    printf("Seleccione el nuevo data rate:\n");
    printf("1. 8 SPS\n");
    printf("2. 16 SPS\n");
    printf("3. 32 SPS\n");
    printf("4. 64 SPS\n");
    printf("5. 128 SPS\n");
    printf("6. 250 SPS\n");
    printf("7. 475 SPS\n");
    printf("8. 860 SPS\n");
    printf("Ingrese la opción deseada: ");
    
    int option;
    scanf("%d", &option);
    getchar(); // Consumir el carácter de nueva línea

    switch (option)
    {
    case 1:
        ADS111x_SetDataRate(&ads1115_0, ADS111X_DATA_RATE_8SPS);
        break;
    case 2:
        ADS111x_SetDataRate(&ads1115_0, ADS111X_DATA_RATE_16SPS);
        break;
    case 3:
        ADS111x_SetDataRate(&ads1115_0, ADS111X_DATA_RATE_32SPS);
        break;
    case 4:
        ADS111x_SetDataRate(&ads1115_0, ADS111X_DATA_RATE_64SPS);
        break;
    case 5:
        ADS111x_SetDataRate(&ads1115_0, ADS111X_DATA_RATE_128SPS);
        break;
    case 6:
        ADS111x_SetDataRate(&ads1115_0, ADS111X_DATA_RATE_250SPS);
        break;
    case 7:
        ADS111x_SetDataRate(&ads1115_0, ADS111X_DATA_RATE_475SPS);
        break;
    case 8:
        ADS111x_SetDataRate(&ads1115_0, ADS111X_DATA_RATE_860SPS);
        break;
    default:
        printf("Opción no válida. El data rate no ha sido cambiado.\n");
        break;
    }
}

// Función para cambiar el multiplexor
static void changeMultiplexer(void)
{
    printf("Seleccione el nuevo multiplexor:\n");
    printf("1. AN0 to GND\n");
    printf("2. AN1 to GND\n");
    printf("3. AN2 to GND\n");
    printf("4. AN3 to GND\n");
    printf("5. AN0 to AN1\n");
    printf("6. AN0 to AN3\n");
    printf("7. AN1 to AN3\n");
    printf("8. AN2 to AN3\n");
    printf("Ingrese la opción deseada: ");

    int option;
    scanf("%d", &option);
    getchar(); // Consumir el carácter de nueva línea

    switch (option)
    {
    case 1:
        ADS111x_SetMultiplexer(&ads1115_0, ADS111X_MUX_AN0_GND);
        break;
    case 2:
        ADS111x_SetMultiplexer(&ads1115_0, ADS111X_MUX_AN1_GND);
        break;
    case 3:
        ADS111x_SetMultiplexer(&ads1115_0, ADS111X_MUX_AN2_GND);
        break;
    case 4:
        ADS111x_SetMultiplexer(&ads1115_0, ADS111X_MUX_AN3_GND);
        break;
    case 5:
        ADS111x_SetMultiplexer(&ads1115_0, ADS111X_MUX_AN0_AN1);
        break;
    case 6:
        ADS111x_SetMultiplexer(&ads1115_0, ADS111X_MUX_AN0_AN3);
        break;
    case 7:
        ADS111x_SetMultiplexer(&ads1115_0, ADS111X_MUX_AN1_AN3);
        break;
    case 8:
        ADS111x_SetMultiplexer(&ads1115_0, ADS111X_MUX_AN2_AN3);
        break;
    default:
        printf("Opción no válida. El multiplexor no ha sido cambiado.\n");
        break;
    }
}

// Función para cambiar el modo de operación
static void changeMode(void)
{
    printf("Seleccione el nuevo modo de operación:\n");
    printf("1. Single-shot\n");
    printf("2. Continuous\n");
    printf("Ingrese la opción deseada: ");

    int option;
    scanf("%d", &option);
    getchar(); // Consumir el carácter de nueva línea

    switch (option)
    {
    case 1:
        ADS111x_SetMode(&ads1115_0, ADS111X_MODE_SINGLE);
        single_mode = 1;
        break;
    case 2:
        ADS111x_SetMode(&ads1115_0, ADS111X_MODE_CONTINUOUS);
        single_mode = 0;
        break;
    default:
        printf("Opción no válida. El modo de operación no ha sido cambiado.\n");
        break;
    }
}