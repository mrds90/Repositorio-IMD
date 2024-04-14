/**
 * @file ADS111X_DRIVER.c
 * @brief This file contains the implementation of the ADS111X driver functions.
 */

/*========= [DEPENDENCIES] =====================================================*/

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>
#include "ads111x_driver.h"
#include "raspi_port.h"

/*========= [PRIVATE MACROS AND CONSTANTS] =====================================*/

/*========= [PRIVATE DATA TYPES] ===============================================*/

/*========= [PRIVATE FUNCTION DECLARATIONS] ====================================*/

/**
 * @brief Initializes the ADS111X driver.
 * @return 0 on success, -1 on failure.
 */
static int InitADS111xDriver(void);

/**
 * @brief Reads ADC data.
 * @return 0 on success, -1 on failure.
 */
static int ReadADC(void);

/**
 * @brief Prints the menu.
 */
static void PrintMenu(void);

/**
 * @brief Handles the menu option selected by the user.
 * @param option The selected option.
 * @return 0 on success, -1 on failure.
 */
static int HandleMenuOption(int option);

/**
 * @brief Changes the data rate.
 * @return 0 on success, -1 on failure.
 */
static int ChangeDataRate(void);

/**
 * @brief Changes the multiplexer setting.
 * @return 0 on success, -1 on failure.
 */
static int ChangeMultiplexer(void);

/**
 * @brief Changes the operation mode.
 * @return 0 on success, -1 on failure.
 */
static int ChangeMode(void);


/*========= [INTERRUPT FUNCTION DECLARATIONS] ==================================*/

/*========= [LOCAL VARIABLES] ==================================================*/

static ads111x_obj_t ADS1115_0;
static uint8_t single_mode = 1;

/*========= [STATE FUNCTION POINTERS] ==========================================*/

/*========= [PUBLIC FUNCTION IMPLEMENTATION] ===================================*/

int main(void) {
    if (InitADS111xDriver() < 0) { // Initialize ADC
        printf("Failed to initialize device\n");
        return EXIT_FAILURE;
    }
    int option;
    do {
        PrintMenu(); // Display menu
        printf("Enter desired option: ");
        scanf("%d", &option); // Read user option
        getchar(); // Consume newline character

        if (HandleMenuOption(option) < 0) { // Handle menu option
            printf("Operation could not be performed successfully\n");
        }
    } while (option != 0); // Exit loop if user chooses option 0

    return EXIT_SUCCESS;
}

/*========= [PRIVATE FUNCTION IMPLEMENTATION] ==================================*/

static int InitADS111xDriver(void) {
    ads111x_i2c_t ads111x_port = PORT_Init();
    if (ADS111x_Init(&ADS1115_0, ADS111X_ADDR_0, ADS111X_PGA_4096, ADS1115, &ads111x_port) < 0) {
        return -1;
    }
    if (ADS111x_SetDataRate(&ADS1115_0, ADS111X_DATA_RATE_16SPS) < 0) {
        return -1;
    }
    if (ADS111x_SetMultiplexer(&ADS1115_0, ADS111X_MUX_AN0_GND) < 0) {
        return -1;
    }
    if (ADS111x_SetMode(&ADS1115_0, ADS111X_MODE_SINGLE) < 0) {
        return -1;
    }
    single_mode = 1;
    if (ADS111x_SetComparatorQueue(&ADS1115_0, ADS111X_COMP_QUE_2_CONV) < 0) {
        return -1;
    }
    if (ADS111x_StartConversion(&ADS1115_0) < 0) {
        return -1;
    }
    return 0;
}

// Function to perform an ADC reading
static int ReadADC(void) {
    if (single_mode) {
        if (ADS111x_StartConversion(&ADS1115_0) < 0) {
            return -1;
        }
        sleep(1);
    }
    uint16_t adc_data = ADS111x_Read(&ADS1115_0);
    if (adc_data != -1) {
        printf("\n\nData from ADC: %X \n\n", adc_data);
    }
    return 0;
}

// Function to print the menu options
static void PrintMenu(void) {
    printf("\n--- Menu Options ---\n");
    printf("1. Perform ADC reading\n");
    printf("2. Change data rate\n");
    printf("3. Change multiplexer\n");
    printf("4. Change operation mode\n");
    printf("0. Exit\n");
}

// Function to handle the selected option by the user
static int HandleMenuOption(int option) {
    int ret = 0;
    switch (option) {
        case 1:
            ret = ReadADC(); // Perform ADC reading
            break;

        case 2:
            ret = ChangeDataRate(); // Change data rate
            break;

        case 3:
            ret = ChangeMultiplexer(); // Change multiplexer
            break;

        case 4:
            ret = ChangeMode(); // Change operation mode
            break;

        case 0:
            printf("Exiting...\n");
            break;

        default:
            printf("Invalid option. Please try again.\n");
            break;
    }
    return ret;
}

// Function to change the data rate
static int ChangeDataRate(void) {
    printf("Select new data rate:\n");
    printf("1. 8 SPS\n");
    printf("2. 16 SPS\n");
    printf("3. 32 SPS\n");
    printf("4. 64 SPS\n");
    printf("5. 128 SPS\n");
    printf("6. 250 SPS\n");
    printf("7. 475 SPS\n");
    printf("8. 860 SPS\n");
    printf("Enter desired option: ");

    int option;
    scanf("%d", &option);
    getchar(); // Consume newline character

    switch (option) {
        case 1:
            if (ADS111x_SetDataRate(&ADS1115_0, ADS111X_DATA_RATE_8SPS) < 0) {
                return -1;
            }
            break;

        case 2:
            if (ADS111x_SetDataRate(&ADS1115_0, ADS111X_DATA_RATE_16SPS) < 0) {
                return -1;
            }
            break;

        case 3:
            if (ADS111x_SetDataRate(&ADS1115_0, ADS111X_DATA_RATE_32SPS) < 0) {
                return -1;
            }
            break;

        case 4:
            if (ADS111x_SetDataRate(&ADS1115_0, ADS111X_DATA_RATE_64SPS) < 0) {
                return -1;
            }
            break;

        case 5:
            if (ADS111x_SetDataRate(&ADS1115_0, ADS111X_DATA_RATE_128SPS) < 0) {
                return -1;
            }
            break;

        case 6:
            if (ADS111x_SetDataRate(&ADS1115_0, ADS111X_DATA_RATE_250SPS) < 0) {
                return -1;
            }
            break;

        case 7:
            if (ADS111x_SetDataRate(&ADS1115_0, ADS111X_DATA_RATE_475SPS) < 0) {
                return -1;
            }
            break;

        case 8:
            if (ADS111x_SetDataRate(&ADS1115_0, ADS111X_DATA_RATE_860SPS) < 0) {
                return -1;
            }
            break;

        default:
            printf("Invalid option. Data rate has not been changed.\n");
            break;
    }
    return 0;
}

// Function to change the multiplexer setting
static int ChangeMultiplexer(void) {
    printf("Select new multiplexer:\n");
    printf("1. AN0 to GND\n");
    printf("2. AN1 to GND\n");
    printf("3. AN2 to GND\n");
    printf("4. AN3 to GND\n");
    printf("5. AN0 to AN1\n");
    printf("6. AN0 to AN3\n");
    printf("7. AN1 to AN3\n");
    printf("8. AN2 to AN3\n");
    printf("Enter desired option: ");

    int option;
    scanf("%d", &option);
    getchar(); // Consume newline character

    switch (option) {
        case 1:
            if (ADS111x_SetMultiplexer(&ADS1115_0, ADS111X_MUX_AN0_GND) < 0) {
                return -1;
            }
            break;

        case 2:
            if (ADS111x_SetMultiplexer(&ADS1115_0, ADS111X_MUX_AN1_GND) < 0) {
                return -1;
            }
            break;

        case 3:
            if (ADS111x_SetMultiplexer(&ADS1115_0, ADS111X_MUX_AN2_GND) < 0) {
                return -1;
            }
            break;

        case 4:
            if (ADS111x_SetMultiplexer(&ADS1115_0, ADS111X_MUX_AN3_GND) < 0) {
                return -1;
            }
            break;

        case 5:
            if (ADS111x_SetMultiplexer(&ADS1115_0, ADS111X_MUX_AN0_AN1) < 0) {
                return -1;
            }
            break;

        case 6:
            if (ADS111x_SetMultiplexer(&ADS1115_0, ADS111X_MUX_AN0_AN3) < 0) {
                return -1;
            }
            break;

        case 7:
            if (ADS111x_SetMultiplexer(&ADS1115_0, ADS111X_MUX_AN1_AN3) < 0) {
                return -1;
            }
            break;

        case 8:
            if (ADS111x_SetMultiplexer(&ADS1115_0, ADS111X_MUX_AN2_AN3) < 0) {
                return -1;
            }
            break;

        default:
            printf("Invalid option. Multiplexer has not been changed.\n");
            break;
    }
    return 0;
}

// Function to change the operation mode
static int ChangeMode(void) {
    printf("Select new operation mode:\n");
    printf("1. Single-shot\n");
    printf("2. Continuous\n");
    printf("Enter desired option: ");

    int option;
    scanf("%d", &option);
    getchar(); // Consume newline character

    switch (option) {
        case 1:
            if (ADS111x_SetMode(&ADS1115_0, ADS111X_MODE_SINGLE) < 0) {
                return -1;
            }
            single_mode = 1;
            break;

        case 2:
            if (ADS111x_SetMode(&ADS1115_0, ADS111X_MODE_CONTINUOUS) < 0) {
                return -1;
            }
            single_mode = 0;
            break;

        default:
            printf("Invalid option. Operation mode has not been changed.\n");
            break;
    }
    return 0;
}

/*========= [INTERRUPT FUNCTION IMPLEMENTATION] ================================*/
