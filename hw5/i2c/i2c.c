/**
 * Copyright (c) 2021 Raspberry Pi (Trading) Ltd.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 **/

#include <stdio.h>
#include "hardware/i2c.h"
#include "pico/binary_info.h"
#include "pico/stdlib.h"

 // Example code to talk to a MCP23008/MCP23S08
 
 // device has default bus address of 0x76
#define ADDR _u(0b0100000) // 7 bit address

// hardware registers we care about
#define REG_IODIR _u(0x00)
#define REG_GPIO _u(0x09)
#define REG_OLAT _u(0x0A)


void MCP23008_init() {
    // use the "handheld device dynamic" optimal setting (see datasheet)
    uint8_t buf[2];

    // send register number followed by its corresponding value
    buf[0] = REG_IODIR;
    buf[1] = 0b01111111; // only GP7 is an output, all rest are inputs

    // this function does start bit and reads ACK bit
    i2c_write_blocking(i2c_default, ADDR, buf, 2, false);

}

uint8_t MCP23008_read() {
    // BMP280 data registers are auto-incrementing and we have 3 temperature and
    // pressure registers each, so we start at 0xF7 and read 6 bytes to 0xFC
    // note: normal mode does not require further ctrl_meas and config register writes

    uint8_t buf[1];
    uint8_t reg = REG_GPIO;
    i2c_write_blocking(i2c_default, ADDR, &reg, 1, true);  // true to keep master control of bus
    i2c_read_blocking(i2c_default, ADDR, &buf[0], 1, false);  // false - finished with bus
    return buf[0] & 1;

}

void MCP23008_set_output(int state) {

    uint8_t buf[2];

    buf[0] = REG_OLAT;
    buf[1] = state<<7; // only GP7 is an output, all rest are inputs
    i2c_write_blocking(i2c_default, ADDR, buf, 2, false);  // true to keep master control of bus
  
}


int main() {
    stdio_init_all();

    // I2C is "open drain", pull ups to keep signal high when no data is being sent
    i2c_init(i2c_default, 100 * 1000);
    gpio_set_function(PICO_DEFAULT_I2C_SDA_PIN, GPIO_FUNC_I2C);
    gpio_set_function(PICO_DEFAULT_I2C_SCL_PIN, GPIO_FUNC_I2C);

    // initialize the heartbeat LED
    gpio_init(PICO_DEFAULT_LED_PIN);
    gpio_set_dir(PICO_DEFAULT_LED_PIN,GPIO_OUT);

    // call init function
    MCP23008_init(); // gp7 is output, gp0 is input

    sleep_ms(250); // sleep so that data polling and register update don't collide

    while (1) {
        // blink GP25
        gpio_put(PICO_DEFAULT_LED_PIN,1);
        sleep_ms(20);
        gpio_put(PICO_DEFAULT_LED_PIN,0);
        sleep_ms(20);
        
        // if gp0 is high * third
        if (MCP23008_read()  == 1){
            MCP23008_set_output(0);
            sleep_ms(20);

        }
        // if gp0 is low * fourth
        else if (MCP23008_read() == 0 ) {
            MCP23008_set_output(1);
            sleep_ms(20);

        }
    }
}
