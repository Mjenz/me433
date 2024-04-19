/**
 * Copyright (c) 2020 Raspberry Pi (Trading) Ltd.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */


#include "pico/stdlib.h"
#include "hardware/uart.h"
#include "hardware/irq.h"
#include "hardware/i2c.h"
#include "ssd1306.h"


/// \tag::uart_advanced[]

#define UART_ID uart0
#define BAUD_RATE 115200
#define DATA_BITS 8
#define STOP_BITS 1
#define PARITY    UART_PARITY_NONE

// We are using pins 0 and 1, but see the GPIO function select table in the
// datasheet for information on which other pins can be used.
#define UART_TX_PIN 0
#define UART_RX_PIN 1

static int chars_rxed = 0;

volatile int x = 0;
volatile int y = 0;

// RX interrupt handler
void on_uart_rx() {
    while (uart_is_readable(UART_ID)) {
        uint8_t ch = uart_getc(UART_ID);

        // if it is a newline character, go to the next line unless we have maxed out the screen
        // otherwise wipe the screen and start over
        if (ch == '\r') {
            x = 0;
            if (y > 24){
                y = 0;
                ssd1306_clear();
            } else {
                y = y + 8;
            }
        } else {
            // draw the char
            drawChar(x,y,ch-32);
            ssd1306_update();
            // now you need to set upp the next char

            // check that you arent going over the screen
            if ((x + 10)>128){
                // if so check if you are at the bottom line
                if ((y+8) > 30){
                    // if so clear the screen and reset
                    y = 0;
                    x = 0;
                    ssd1306_clear();
                } else {
                    // otherwise go to the next line
                    y = y + 8;
                }
                // both these cases result in going to a new line, so reset x
                x = 0;
            } else {
                // if no new line, go to next x coordinate
                x = x + 6;
            }

        }
        // Can we send it back?
        // if (uart_is_writable(UART_ID)) {
        //     // Change it slightly first!
        //     uart_putc(UART_ID, ch);
        // }
        chars_rxed++;
    }
}

int main() {

    // initilize LED pin and blink
    gpio_init(PICO_DEFAULT_LED_PIN);
    gpio_set_dir(PICO_DEFAULT_LED_PIN,GPIO_OUT);

    gpio_put(PICO_DEFAULT_LED_PIN,1);
    sleep_ms(500);
    gpio_put(PICO_DEFAULT_LED_PIN,0);
    sleep_ms(500);
    // Set up our UART with a basic baud rate.
    uart_init(UART_ID, 2400);

    // Set the TX and RX pins by using the function select on the GPIO
    // Set datasheet for more information on function select
    gpio_set_function(UART_TX_PIN, GPIO_FUNC_UART);
    gpio_set_function(UART_RX_PIN, GPIO_FUNC_UART);

    // Actually, we want a different speed
    // The call will return the actual baud rate selected, which will be as close as
    // possible to that requested
    int __unused actual = uart_set_baudrate(UART_ID, BAUD_RATE);

    // Set UART flow control CTS/RTS, we don't want these, so turn them off
    uart_set_hw_flow(UART_ID, false, false);

    // Set our data format
    uart_set_format(UART_ID, DATA_BITS, STOP_BITS, PARITY);

    // Turn off FIFO's - we want to do this character by character
    uart_set_fifo_enabled(UART_ID, false);

    // Set up a RX interrupt
    // We need to set up the handler first
    // Select correct interrupt for the UART we are using
    int UART_IRQ = UART_ID == uart0 ? UART0_IRQ : UART1_IRQ;

    // And set up and enable the interrupt handlers
    irq_set_exclusive_handler(UART_IRQ, on_uart_rx);
    irq_set_enabled(UART_IRQ, true);

    // Now enable the UART to send interrupts - RX only
    uart_set_irq_enables(UART_ID, true, false);

    // OK, all set up.
    // Lets send a basic string out, and then run a loop and wait for RX interrupts
    // The handler will count them, but also reflect the incoming data back with a slight change!
    uart_puts(UART_ID, "\nHello, uart interrupts\n");

    
    // init i2c communication
    i2c_init(i2c_default, 2000 * 1000);
    gpio_set_function(PICO_DEFAULT_I2C_SDA_PIN, GPIO_FUNC_I2C);
    gpio_set_function(PICO_DEFAULT_I2C_SCL_PIN, GPIO_FUNC_I2C);


    // intialize functions
    ssd1306_setup();
    stdio_init_all();

    while (1)
        tight_loop_contents();

        gpio_put(PICO_DEFAULT_LED_PIN,1);
        sleep_ms(10);
        gpio_put(PICO_DEFAULT_LED_PIN,0);
        sleep_ms(10);
}

/// \end:uart_advanced[]
