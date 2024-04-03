/*
For this assignment, make a folder in your repo called HW3. Build code that
    Turns on an LED
    Waits until a button is pressed
    Turns off the LED
    Asks the user to enter a number of analog samples to take, between 1 and 100
    Reads the number entered by the user
    Reads the voltage on ADC0 that number of times at 100Hz
    Prints back the voltages in the units of volts
    then loops
Submit on Canvas a link to the folder in your repo and a short demo video.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "pico/stdlib.h"
#include "hardware/adc.h"

#define INPUT 16

const uint LED_PIN = PICO_DEFAULT_LED_PIN;


int main() {
    stdio_init_all();
    while (!stdio_usb_connected()) {
        sleep_ms(100);
    }

    const float conversion_factor = 3.3f / (1 << 12);

    gpio_init(LED_PIN);
    gpio_set_dir(LED_PIN, GPIO_OUT);
    gpio_init(INPUT);
    gpio_set_dir(INPUT,GPIO_IN);

    printf("Start!\n");
 
    adc_init(); // init the adc module
    adc_gpio_init(26); // set ADC0 pin to be adc input instead of GPIO
    adc_select_input(0); // select to read from ADC0

    int input = 0;
    char message[100];



    while (1) {
                
        // turn LED on and wait for the user to type something
        gpio_put(LED_PIN, 1);
        while(gpio_get(INPUT)==0){;}
        gpio_put(LED_PIN, 0);


        // How many ADC cycles to run?
        printf("Type in the number of ADC samples to take:\r\n");
        scanf("%s", message); // pick up response from user
        input = atoi(message); // turn message to int

        uint16_t result[input];
        float converted[input];

        for(int i = 0; i < input; i++){
            result[i] = adc_read();
            converted[i] = result[i]*conversion_factor;

            sleep_ms(10); // sleep so that you collect at 100Hz
        }
        for(int i = 0; i < input; i++){
            sprintf(message,"[%i]: %f V\r\n",i+1, converted[i]);
            printf(message);
        }
    }
}