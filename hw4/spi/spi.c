/**
 * Copyright (c) 2020 Raspberry Pi (Trading) Ltd.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <stdio.h>
#include <string.h>
#include <math.h>
#include <time.h>  
#include "pico/stdlib.h"
#include "pico/binary_info.h"
#include "hardware/spi.h"

#define READ_BIT 0x80

int32_t t_fine;

uint16_t dig_T1;
int16_t dig_T2, dig_T3;
uint16_t dig_P1;
int16_t dig_P2, dig_P3, dig_P4, dig_P5, dig_P6, dig_P7, dig_P8, dig_P9;
uint8_t dig_H1, dig_H3;
int8_t dig_H6;
int16_t dig_H2, dig_H4, dig_H5;

void printBinary(unsigned long num) {
    if (num > 1)
        printBinary(num / 2);
    printf("%lu", num % 2);
}

#ifdef PICO_DEFAULT_SPI_CSN_PIN
static inline void cs_select() {
    asm volatile("nop \n nop \n nop");
    gpio_put(PICO_DEFAULT_SPI_CSN_PIN, 0);  // Active low
    asm volatile("nop \n nop \n nop");
}

static inline void cs_deselect() {
    asm volatile("nop \n nop \n nop");
    gpio_put(PICO_DEFAULT_SPI_CSN_PIN, 1);
    asm volatile("nop \n nop \n nop");
}
#endif

#if defined(spi_default) && defined(PICO_DEFAULT_SPI_CSN_PIN)
// take in the 10 bit number we want and channel A or B?
static void write_register(uint8_t AorB, long data) {
    uint8_t buf[2];
    long full;

    uint8_t given = 0b011;
    //[A/B BUF GA SHDN D9 D8 D7 D6 D5 D4 D3 D2 D1 D0 x x]
    //[input 0 1 1 input ...]
    // Cast AorB, given, and number to unsigned long
     full = ((unsigned long)AorB << 16) | ((unsigned long)given << 12) | ((unsigned long)data << 2); 
    // Assign the results of bitwise AND operations to variables
    unsigned int one_result = ((full & 0b1111111100000000UL )>> 8); // Use UL suffix to specify unsigned long constant
    unsigned int two_result = full & 0b0000000011111111UL; // Use UL suffix to specify unsigned long constant

    buf[0] = one_result;// first 8 bit number
    buf[1] = two_result; // second 8 bit number
    char message[50];
    sprintf(message,"AorB %d \t given %d data %lu %lu %lu %lu\n",AorB, given, data, full,buf[0],buf[1]);
    printf(message);
    printf("AorB    ");
    printBinary(AorB);
    printf("   given   ");
    printBinary(given);
    printf("    data    ");
    printBinary(data);
    printf("   full    ");
    printBinary(full);
    printf("   buf[0]    ");
    printBinary(buf[0]);
    printf("     buf[1]    ");
    printBinary(buf[1]);
    printf("\n\n");

    cs_select(); // make this pin go low
    // here is a pointer (buf) to the array of data i would like to send it has two componenets
    spi_write_blocking(spi_default, buf, 2); 
    cs_deselect(); // make this pin go high
    // sleep_ms(10);
}

static void read_registers(uint8_t reg, uint8_t *buf, uint16_t len) {
    // For this particular device, we send the device the register we want to read
    // first, then subsequently read from the device. The register is auto incrementing
    // so we don't need to keep sending the register we want, just the first.
    reg |= READ_BIT;
    cs_select();
    spi_write_blocking(spi_default, &reg, 1);
    sleep_ms(10);
    spi_read_blocking(spi_default, 0, buf, len);
    cs_deselect();
    // sleep_ms(10);
}

#endif

int main() {
    stdio_init_all();
// #if !defined(spi_default) || !defined(PICO_DEFAULT_SPI_SCK_PIN) || !defined(PICO_DEFAULT_SPI_TX_PIN) || !defined(PICO_DEFAULT_SPI_RX_PIN) || !defined(PICO_DEFAULT_SPI_CSN_PIN)
// #warning spi/bme280_spi example requires a board with SPI pins
//     puts("Default SPI pins were not defined");
// #else
    char msg[100];
    scanf(msg, "%s");
    printf("Hello, bme280! Reading raw data from registers via SPI...\n");

    // LED to make sure its running
    const uint LED_PIN = PICO_DEFAULT_LED_PIN;
    gpio_init(LED_PIN);
    gpio_set_dir(LED_PIN, GPIO_OUT);

    // This example will use SPI0 at 0.5MHz.
    spi_init(spi_default, 8000 * 1000);
    gpio_set_function(PICO_DEFAULT_SPI_RX_PIN, GPIO_FUNC_SPI);
    gpio_set_function(PICO_DEFAULT_SPI_SCK_PIN, GPIO_FUNC_SPI);
    gpio_set_function(PICO_DEFAULT_SPI_TX_PIN, GPIO_FUNC_SPI);
  
    // Chip select is active-low, so we'll initialise it to a driven-high state
    gpio_init(PICO_DEFAULT_SPI_CSN_PIN); // initialize
    gpio_set_dir(PICO_DEFAULT_SPI_CSN_PIN, GPIO_OUT); // set as output
    gpio_put(PICO_DEFAULT_SPI_CSN_PIN, 1); // set it high

    // See if SPI is working - interrograte the device for its I2C ID number, should be 0x60
    uint8_t id;
    read_registers(0xD0, &id, 1);
    printf("Chip ID is 0x%x\n", id);

    // write_register(0b00111111, 0b11111111); // example


    // sin wave
    char message[50];
    int length = 100;
    unsigned long array[length];

    for(int i = 0; i<length;i++){
        
        double x = (double)i /length * 2 * M_PI; // Adjust the range of x to [0, 2*pi]
        array[i] = (long)((sin(x)+1) * 1023.0/2.0);
        sprintf(message,"\t%lu\t",array[i]);
        printf(message);
    }

    // triangle wave

    float slope = 1024.0/49.0/2/2;
    float previous = 0;
    unsigned long array2[length];

    for(int ii = 0; ii < length/2;ii++){
        array2[ii] = slope + previous;
        array2[99-ii] = slope + previous;
        previous = array2[ii];
        sprintf(message,"\t%lu\t",array2[ii]);
        printf(message);
    }


    int counter = 0;
    int pin = 0; // 0 --> outA; 1 --> outB

    // sin wave
    // while (true) {
    //     // LED on
    //     gpio_put(LED_PIN, 1);

    //     write_register(pin, array[counter]);
    //     // sprintf(message,"%lu\n",array[counter]);
    //     // printf(message);
    //     counter = counter + 1;
    //     if (counter > 99){
    //         counter = 0;
    //     }

    // }

    // triangle wave
     while (true) {
        // LED on
        gpio_put(LED_PIN, 1);

        write_register(pin, array2[counter]);
        // sprintf(message,"%lu\n",array[counter]);
        // printf(message);
        counter = counter + 1;
        if (counter > 99){
            counter = 0;
        }

    }

}
