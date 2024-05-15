#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"

// data variable
volatile uint8_t data[14];

// address
#define ADDR 0x68
// config registers
#define CONFIG 0x1A
#define GYRO_CONFIG 0x1B
#define ACCEL_CONFIG 0x1C
#define PWR_MGMT_1 0x6B
#define PWR_MGMT_2 0x6C
// sensor data registers:
#define ACCEL_XOUT_H 0x3B
#define ACCEL_XOUT_L 0x3C
#define ACCEL_YOUT_H 0x3D
#define ACCEL_YOUT_L 0x3E
#define ACCEL_ZOUT_H 0x3F
#define ACCEL_ZOUT_L 0x40
#define TEMP_OUT_H   0x41
#define TEMP_OUT_L   0x42
#define GYRO_XOUT_H  0x43
#define GYRO_XOUT_L  0x44
#define GYRO_YOUT_H  0x45
#define GYRO_YOUT_L  0x46
#define GYRO_ZOUT_H  0x47
#define GYRO_ZOUT_L  0x48
#define WHO_AM_I     0x75

void MPU6050_init() {
    printf("initializing\r\n");
    char ms[50];
   // use the "handheld device dynamic" optimal setting (see datasheet)
    uint8_t buf[2];

    // send register number followed by its corresponding value
    buf[0] = PWR_MGMT_1; // send to this register
    buf[1] = 0x00; // the number 0
    // printf("sending\r\n");

    // this function does start bit and reads ACK bit
    i2c_write_blocking(i2c_default, ADDR, buf, 2, false);

    // reset it for good measure
    // buf[0] = 0x6B; 
    // buf[1] = 0x80;
    // i2c_write_blocking(i2c_default, ADDR, buf, 2, false);
    // printf("Initiailzation has happened\r\n");
    // make sure that it is working
    uint8_t wai = WHO_AM_I;
    i2c_write_blocking(i2c_default, ADDR, &wai, 1, true);  // true to keep master control of bus
    i2c_read_blocking(i2c_default, ADDR, buf, 1,false);
    sprintf(ms,"%d\r\n",buf[0]);
    // printf(ms);
    // if it is the wrong result, stop the code and blink!
    if(buf[0]!= 0x68){
        while(1){
            printf(ms);
            gpio_put(PICO_DEFAULT_LED_PIN,1);
        }
    }
}

void MPU6050_accel_config(){
    uint8_t buf[2];

    buf[0] = ACCEL_CONFIG; // send to this register
    buf[1] = 0b00001000; // set to +-2g range
    i2c_write_blocking(i2c_default, ADDR, buf, 2, false);
}

void MPU6050_gyro_config(){
    uint8_t buf[2];

    buf[0] = GYRO_CONFIG; // send to this register
    buf[1] = 0b00011000; // set to +-2000 range
    i2c_write_blocking(i2c_default, ADDR, buf, 2, false);
}

void print_binary(uint8_t num) {
    int size = sizeof(uint8_t) * 8; // Size of short in bits

    // Iterate through each bit of the short
    for (int i = size - 1; i >= 0; i--) {
        // Bitwise AND with 1 to check the least significant bit
        // If it's 1, print 1; if it's 0, print 0
        printf("%d", (num >> i) & 1);
    }
    printf("\n");
}

uint8_t  * MPU6050_read_burst(uint8_t * buf) {

    // read 14 bytes
    // uint8_t where[14] = {ACCEL_XOUT_H,ACCEL_XOUT_L,ACCEL_YOUT_H,ACCEL_YOUT_L,ACCEL_ZOUT_H,ACCEL_ZOUT_L,TEMP_OUT_H,TEMP_OUT_L,GYRO_XOUT_H,GYRO_XOUT_L,GYRO_YOUT_H,GYRO_YOUT_L,GYRO_ZOUT_H,GYRO_ZOUT_L};
    uint8_t where = ACCEL_XOUT_H;
    i2c_write_blocking(i2c_default, ADDR, &where, 1, true);  // true to keep master control of bus
    i2c_read_blocking(i2c_default, ADDR, buf, 14,false);
    sleep_ms(10);
    // for(int i=0;i<14;i++){
    //     print_binary(buf[i]);
    // }
    return buf;
    
}


 
 float * MPU6050_bitshift(uint8_t * buf, float * formatted){
    char ms[20];
    signed short middle[7];
    float placeholder;
    // short bitmask = 0b1111111100000000;
    for(int i = 0;i<14;i++) {
        if(i%2 == 0){ // only every other one
            // sprintf(ms,"buf[%d]: %d \n",i, buf[i]);
            // printf(ms);
            // print_binary(buf[i]);
            // sprintf(ms,"buf[%d]: %d \n",i+1, buf[i+1]);
            // printf(ms);
            // print_binary(buf[i+1]);
            
            middle[i/2] = (buf[i]<<8|(buf[i+1]));
            // sprintf(ms,"combined [%d]: %d\n",i/2,middle[i/2]);
            // printf(ms);
            // print_binary((uint8_t)((middle[i/2]&0b11111111000000000)>>8));
            // print_binary((uint8_t)middle[i/2]&0b00000000011111111);

        }
    }

    for(int i = 0;i<7;i++) {
        if(i < 3){
            formatted[i] = (middle[i] * 0.000061);
            // sprintf(ms,"%f g\r\n",formatted[i]); // display accel
            // printf(ms);

            // placeholder = (middle[i] * 0.000061);
            // sprintf(ms,"%f g\r\n",placeholder); // display accel
            // printf(ms);
        } else if(i > 3) {
            formatted[i] = (middle[i]* 0.007630);
            // sprintf(ms,"%f deg/s\r\n",formatted[i]); // display gyro
            // printf(ms);

            // placeholder = (middle[i] * 0.007630);
            // sprintf(ms,"%f deg/s\n",placeholder); // display accel
            // printf(ms);
        } else {
            formatted[i] = ((middle[i] / 340.0) + 36.53);
            // sprintf(ms,"%f C\r\n",formatted[i]); // display temp
            // printf(ms);

            // placeholder = ((middle[i] / 340.0) + 36.53);
            // sprintf(ms,"%f C\r\n",placeholder); // display temp
            // printf(ms);
        }
    }

    return formatted;
}

int main() {
    // run initialize functions
    stdio_init_all();

    // I2C is "open drain", pull ups to keep signal high when no data is being sent
    i2c_init(i2c_default, 400 * 1000);
    gpio_set_function(PICO_DEFAULT_I2C_SDA_PIN, GPIO_FUNC_I2C);
    gpio_set_function(PICO_DEFAULT_I2C_SCL_PIN, GPIO_FUNC_I2C);
    // gpio_pull_up(PICO_DEFAULT_I2C_SDA_PIN);
    // gpio_pull_up(PICO_DEFAULT_I2C_SCL_PIN);

    // initialize the heartbeat LED
    gpio_init(PICO_DEFAULT_LED_PIN);
    gpio_set_dir(PICO_DEFAULT_LED_PIN,GPIO_OUT);

    // call init function
    MPU6050_init(); // gp7 is output, gp0 is input
    MPU6050_accel_config();
    MPU6050_gyro_config();
    sleep_ms(250); // sleep so that data polling and register update don't collide

    char ms[50];
    uint8_t data[14];
    uint8_t * read;
    float * complete;
    float final[7];
    // make it so that the function takes in final

    while(1){
        read = MPU6050_read_burst(data);
        complete = MPU6050_bitshift(read, final);
        for(int i = 0;i<7;i++) {
            if(i < 3){
                sprintf(ms,"%f g\r\n",complete[i]); // display accel
                printf(ms);

            } else if(i > 3) {
                sprintf(ms,"%f deg/s\r\n",complete[i]); // display gyro
                printf(ms);

            } else {
                sprintf(ms,"%f C\r\n",complete[i]); // display temp
                printf(ms);

            }
        }
        printf("\n____________________\n");

    }
}