// based on adafruit and sparkfun libraries
#include <string.h> // for memset
#include <stdio.h>
#include "ssd1306.h"
#include "hardware/i2c.h"
#include "pico/stdlib.h"
#include "font.h"
#include "hardware/adc.h"


unsigned char SSD1306_ADDRESS = 0b0111100; // 7bit i2c address FIXED
unsigned char ssd1306_buffer[513]; // 128x32/8. Every bit is a pixel except first byte

void drawChar(int x, int y, char input){
    char ms[50];
    for(int ii = 0; ii < 5;ii++){
        // get column of pixles we need to draw
        char c = ASCII[input][ii];
        for(int jj = 0; jj < 8;jj++){
            char bit = (c>>jj)&0b1;

            if (bit == 0b1){
                // write the pixel, pay attention to how it is shifted
                ssd1306_drawPixel(x+ii,y+jj,1);
            }
            else {
                ssd1306_drawPixel(x+ii,y+jj,0);

            }
            // sprintf(ms,"x: %d  y: %d  input: %c  ii: %d  jj: %d  c: %c  bit: %c\r\n",x,y,input,ii,jj,c,bit);
            // printf(ms);

        }
    }

}
void ssd1306_setup() {
    // first byte in ssd1306_buffer is a command
    ssd1306_buffer[0] = 0x40;
    // give a little delay for the ssd1306 to power up
    //_CP0_SET_COUNT(0);
    //while (_CP0_GET_COUNT() < 48000000 / 2 / 50) {
    //}
    sleep_ms(20);
    ssd1306_command(SSD1306_DISPLAYOFF);
    ssd1306_command(SSD1306_SETDISPLAYCLOCKDIV);
    ssd1306_command(0x80);
    ssd1306_command(SSD1306_SETMULTIPLEX);
    ssd1306_command(0x1F); // height-1 = 31
    ssd1306_command(SSD1306_SETDISPLAYOFFSET);
    ssd1306_command(0x0);
    ssd1306_command(SSD1306_SETSTARTLINE);
    ssd1306_command(SSD1306_CHARGEPUMP);
    ssd1306_command(0x14);
    ssd1306_command(SSD1306_MEMORYMODE);
    ssd1306_command(0x00);
    ssd1306_command(SSD1306_SEGREMAP | 0x1);
    ssd1306_command(SSD1306_COMSCANDEC);
    ssd1306_command(SSD1306_SETCOMPINS);
    ssd1306_command(0x02);
    ssd1306_command(SSD1306_SETCONTRAST);
    ssd1306_command(0x8F);
    ssd1306_command(SSD1306_SETPRECHARGE);
    ssd1306_command(0xF1);
    ssd1306_command(SSD1306_SETVCOMDETECT);
    ssd1306_command(0x40);
    ssd1306_command(SSD1306_DISPLAYON);
    ssd1306_clear(); // resets the screen
    ssd1306_update(); // sends all the pixles
}

// send a command instruction (not pixel data)
void ssd1306_command(unsigned char c) {
    //i2c_master_start();
    //i2c_master_send(ssd1306_write);
    //i2c_master_send(0x00); // bit 7 is 0 for Co bit (data bytes only), bit 6 is 0 for DC (data is a command))
    //i2c_master_send(c);
    //i2c_master_stop();

    uint8_t buf[2];
    buf[0] = 0x00;
    buf[1] =c;
    i2c_write_blocking(i2c_default, SSD1306_ADDRESS, buf, 2, false);
}

// update every pixel on the screen
void ssd1306_update() {
    ssd1306_command(SSD1306_PAGEADDR);
    ssd1306_command(0);
    ssd1306_command(0xFF);
    ssd1306_command(SSD1306_COLUMNADDR);
    ssd1306_command(0);
    ssd1306_command(128 - 1); // Width

    unsigned short count = 512; // WIDTH * ((HEIGHT + 7) / 8)
    unsigned char * ptr = ssd1306_buffer; // first address of the pixel buffer
    /*
    i2c_master_start();
    i2c_master_send(ssd1306_write);
    i2c_master_send(0x40); // send pixel data
    // send every pixel
    while (count--) {
        i2c_master_send(*ptr++);
    }
    i2c_master_stop();
    */

    i2c_write_blocking(i2c_default, SSD1306_ADDRESS, ptr, 513, false);
}



// zero every pixel value
void ssd1306_clear() {
    memset(ssd1306_buffer, 0, 512); // make every bit a 0, memset in string.h
    ssd1306_buffer[0] = 0x40; // first byte is part of command
}


// int main() {

//     // initilize LED pin and blink
//     gpio_init(PICO_DEFAULT_LED_PIN);
//     gpio_set_dir(PICO_DEFAULT_LED_PIN,GPIO_OUT);

//     gpio_put(PICO_DEFAULT_LED_PIN,1);
//     sleep_ms(500);
//     gpio_put(PICO_DEFAULT_LED_PIN,0);
//     sleep_ms(500);

//     // init i2c communication
//     i2c_init(i2c_default, 2000 * 1000);
//     gpio_set_function(PICO_DEFAULT_I2C_SDA_PIN, GPIO_FUNC_I2C);
//     gpio_set_function(PICO_DEFAULT_I2C_SCL_PIN, GPIO_FUNC_I2C);


//     // intialize functions
//     ssd1306_setup();
//     stdio_init_all();

//     // initialize ADC
//     adc_init(); // init the adc module
//     adc_gpio_init(26); // set ADC0 pin to be adc input instead of GPIO
//     adc_select_input(0); // select to read from ADC0

//     // vars for ADC
//     const float conversion_factor = 3.3f / (1 << 12);
//     uint16_t result;
//     float converted;



//     // write a character
//     char message[25];
//     sprintf(message,"Starting up...");
//     drawString(2,0,message);
//     ssd1306_update();
//     sleep_ms(300);


//     while (1) {
//         // collect voltage
        
//         result = adc_read();
//         converted = result * conversion_factor;

//         unsigned long start = to_us_since_boot(get_absolute_time());

//         sprintf(message,"Voltage: %.3f V",converted);
//         drawString(1,0,message);
//         ssd1306_update();
//         unsigned long finish = to_us_since_boot(get_absolute_time());

//         // calculate and print out FPS
//         float FPS = 1000000.0/(finish - start);
//         sprintf(message,"FPS: %.3f V",FPS);
//         drawString(1,15,message);
//         ssd1306_update();
//         // blink GP25
//         // ssd1306_drawPixel(0,0,1);
//         // ssd1306_update();
//         gpio_put(PICO_DEFAULT_LED_PIN,1);
//         sleep_ms(10);
//         // ssd1306_drawPixel(0,0,0);
//         // ssd1306_update();
//         gpio_put(PICO_DEFAULT_LED_PIN,0);
//         sleep_ms(10);
//     }

// }