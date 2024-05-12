add_executable(pwm_servo
        pwm_servo.c
        )

# pull in common dependencies and additional uart hardware support
target_link_libraries(pwm_servo pico_stdlib hardware_pwm)


pico_enable_stdio_usb(pwm_servo 1)
pico_enable_stdio_uart(pwm_servo 0)

# create map/bin/hex file etc.
pico_add_extra_outputs(pwm_servo)






