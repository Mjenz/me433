static const int PWM_lookup[101][2] = {
    {12500, 0},
    {12500, 416},
    {12500, 833},
    {12500, 1250},
    {12500, 1666},
    {12500, 2083},
    {12500, 2500},
    {12500, 2916},
    {12500, 3333},
    {12500, 3750},
    {12500, 4166},
    {12500, 4583},
    {12500, 5000},
    {12500, 5416},
    {12500, 5833},
    {12500, 6250},
    {12500, 6666},
    {12500, 7083},
    {12500, 7500},
    {12500, 7916},
    {12500, 8333},
    {12500, 8750},
    {12500, 9166},
    {12500, 9583},
    {12500, 10000},
    {12500, 10416},
    {12500, 10833},
    {12500, 11250},
    {12500, 11666},
    {12500, 12083},
    {12500, 12500},
    {12500, 12500},
    {12500, 12500},
    {12500, 12500},
    {12500, 12500},
    {12500, 12500},
    {12500, 12500},
    {12500, 12500},
    {12500, 12500},
    {12500, 12500},
    {12500, 12500},
    {12500, 12500},
    {12500, 12500},
    {12500, 12500},
    {12500, 12500},
    {12500, 12500},
    {12500, 12500},
    {12500, 12500},
    {12500, 12500},
    {12500, 12500},
    {12500, 12500},
    {12500, 12500},
    {12500, 12500},
    {12500, 12500},
    {12500, 12500},
    {12500, 12500},
    {12500, 12500},
    {12500, 12500},
    {12500, 12500},
    {12500, 12500},
    {12500, 12500},
    {12500, 12500},
    {12500, 12500},
    {12500, 12500},
    {12500, 12500},
    {12500, 12500},
    {12500, 12500},
    {12500, 12500},
    {12500, 12500},
    {12500, 12500},
    {12083, 12500},
    {11666, 12500},
    {11250, 12500},
    {10833, 12500},
    {10416, 12500},
    {10000, 12500},
    {9583, 12500},
    {9166, 12500},
    {8750, 12500},
    {8333, 12500},
    {7916, 12500},
    {7500, 12500},
    {7083, 12500},
    {6666, 12500},
    {6250, 12500},
    {5833, 12500},
    {5416, 12500},
    {5000, 12500},
    {4583, 12500},
    {4166, 12500},
    {3750, 12500},
    {3333, 12500},
    {2916, 12500},
    {2500, 12500},
    {2083, 12500},
    {1666, 12500},
    {1250, 12500},
    {833,  12500},
    {416,  12500},
    {0,    12500}
};

#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/pwm.h"
#define PWM_A1 16
#define STATE_A2 17 
#define PWM_B1 18 
#define STATE_B2 19 


void initialize_state() {
    gpio_init(STATE_A2);
    gpio_set_dir(STATE_A2,GPIO_OUT);
    gpio_init(STATE_B2);
    gpio_set_dir(STATE_B2,GPIO_OUT);

    // set initially as forward (==0)
    gpio_put(STATE_A2,0);
    gpio_put(STATE_B2,0);
}

void initialize_PWM() {
  gpio_set_function(PWM_A1, GPIO_FUNC_PWM); // Set the LED Pin to be PWM
  uint slice_num = pwm_gpio_to_slice_num(PWM_A1); // Get PWM slice number
  float div = 200; // must be between 1-255
  pwm_set_clkdiv(slice_num, div); // divider
  uint16_t wrap = 12500; // when to rollover, must be less than 65535
  pwm_set_wrap(slice_num, wrap);
  pwm_set_enabled(slice_num, true); // turn on the PWM
  pwm_set_gpio_level(PWM_A1, wrap / 2); // set the duty cycle to 50%

  gpio_set_function(PWM_B1, GPIO_FUNC_PWM); // Set the LED Pin to be PWM
  uint slice_num2 = pwm_gpio_to_slice_num(PWM_B1); // Get PWM slice number
  pwm_set_clkdiv(slice_num, div); // divider
  pwm_set_wrap(slice_num2, wrap);
  pwm_set_enabled(slice_num2, true); // turn on the PWM
  pwm_set_gpio_level(PWM_B1, wrap / 2); // set the duty cycle to 50%
}

void set_PWM(int input1, int input2) {
    // char ms[50];
    // sprintf(ms,"setting PWM as %d\r\n",input1);
    // printf(ms);
    pwm_set_gpio_level(PWM_A1,input1);
    pwm_set_gpio_level(PWM_B1,input2);


}

void set_state(int input1, int input2) {
    // char ms[50];
    // sprintf(ms,"setting state as %d\r\n",input1);
    // printf(ms);
    gpio_put(STATE_A2,input1);
    gpio_put(STATE_B2,input2);


}

int main(){
    stdio_init_all();

    // initilize LED pin and blink
    gpio_init(PICO_DEFAULT_LED_PIN);
    gpio_set_dir(PICO_DEFAULT_LED_PIN,GPIO_OUT);
    gpio_put(PICO_DEFAULT_LED_PIN,1);
    sleep_ms(500);
    gpio_put(PICO_DEFAULT_LED_PIN,0);
   
    initialize_PWM();
    initialize_state();

    int pwm1;
    int pwm2;
    int dir1;
    int dir2;
    int scaled1;
    int scaled2;
    char ms[50];
    int pwm_found[2];
    
    while(1) {

        // Controller code:
        printf("Enter PWM (out of 0-99)\r\n");
        scanf("%d", &pwm1);
        pwm_found[0] = PWM_lookup[pwm1][1];
        pwm_found[1] = PWM_lookup[pwm1][2];

        // set_state(dir1,dir2);
        set_PWM(pwm_found[0],pwm_found[1]);


        sprintf(ms, "pwm1: %d, pwm2  %d\r\n", pwm_found[0], pwm_found[1]);
        printf(ms);
        // Initial code:
        // printf("Enter PWM (out of 100)\r\n");
        // scanf("%d %d", &pwm1, &pwm2);

        // sprintf(ms, "You typed: %d, and  %d\r\n", pwm1, pwm2);
        // printf(ms);

        // printf("Enter direction (back = 1, forward = 0)\r\n");
        // scanf("%d %d", &dir1, &dir2);

        // sprintf(ms, "You typed: %d, and  %d\r\n", dir1, dir2);
        // printf(ms);


        // scaled1 = (pwm1 / 100.0) * 12500;
        // scaled2 = (pwm2 / 100.0) * 12500;
        

        // set_state(dir1,dir2);
        // set_PWM(scaled1,scaled2);
    }
}