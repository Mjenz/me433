add_executable(uart
        uart.c
        ssd1306.c
        )

# pull in common dependencies and additional uart hardware support
target_link_libraries(uart pico_stdlib hardware_uart hardware_i2c hardware_adc)


pico_enable_stdio_usb(uart 1)
pico_enable_stdio_uart(uart 0)

# create map/bin/hex file etc.
pico_add_extra_outputs(uart)






