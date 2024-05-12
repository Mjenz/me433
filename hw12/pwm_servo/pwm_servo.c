#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/pwm.h"

#define OUTPUT_PIN 16 // the built in LED on the Pico

int waveform[200] = {0};

void initialize_PWM() {
  gpio_set_function(OUTPUT_PIN, GPIO_FUNC_PWM); // Set the LED Pin to be PWM
  uint slice_num = pwm_gpio_to_slice_num(OUTPUT_PIN); // Get PWM slice number
  float div = 200; // must be between 1-255
  pwm_set_clkdiv(slice_num, div); // divider
  uint16_t wrap = 12500; // when to rollover, must be less than 65535
  pwm_set_wrap(slice_num, wrap);
  pwm_set_enabled(slice_num, true); // turn on the PWM

  pwm_set_gpio_level(OUTPUT_PIN, wrap / 2); // set the duty cycle to 50%

}

void set_PWM(int input) {
    char ms[50];
    sprintf(ms,"setting PWM as %d\r\n",input);
    printf(ms);
    pwm_set_gpio_level(OUTPUT_PIN,input);

}

void back_and_forth() {
    int i;
    char ms[50];
    // 50 hz update = 200 points for 4 seconds
    // initialize the wavefrom

    printf("Initializing waveform\r\n");
    for (i = 0; i < 100; i++){
        int number = (i / 100.0) * 1625;
        waveform[i] = number;
        waveform[199-i] = number;
   
    }
    i = 0;
    printf("Running back and forth\r\n");
    for (i = 0; i < 200; i++){
        set_PWM(waveform[i]);
        sleep_ms(20);
    }
}
int main() {
    stdio_init_all();

    // initilize LED pin and blink
    gpio_init(PICO_DEFAULT_LED_PIN);
    gpio_set_dir(PICO_DEFAULT_LED_PIN,GPIO_OUT);

   
    initialize_PWM();
    int entry;
    int scaled;
    char ms[50];
    
    while(1) {
        printf("waiting for input\r\n");
        scanf("%d", &entry);
        scaled = (entry / 100.0) * 1375 + 250;
        sprintf(ms, "You typed: %d, scaled to %d\r\n", entry, scaled);

        printf(ms);

        if (entry == 999){
            printf("Back and forth triggered\r\n");
            back_and_forth();
        }
        else{
            set_PWM(scaled);
        }

        // blink three times for cheers
        gpio_put(PICO_DEFAULT_LED_PIN,1);
        sleep_ms(50);
        gpio_put(PICO_DEFAULT_LED_PIN,0);
        sleep_ms(50);
        gpio_put(PICO_DEFAULT_LED_PIN,1);
        sleep_ms(50);
        gpio_put(PICO_DEFAULT_LED_PIN,0);
        sleep_ms(50);
        gpio_put(PICO_DEFAULT_LED_PIN,1);
        sleep_ms(50);
        gpio_put(PICO_DEFAULT_LED_PIN,0);
        sleep_ms(50);

  }
}
