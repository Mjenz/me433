#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/adc.h"
int main() {
    stdio_init_all();
    while (!stdio_usb_connected()) {
        sleep_ms(100);
    }
    printf("Start!\n");
    // initialize the adc here
 
    while (1) {
        char message[100];
        scanf("%s", message);
        printf("message: %s\r\n",message);
        sleep_ms(50);
    }
}