add_executable(ssd1306
    ssd1306.c
            )


target_link_libraries(ssd1306 hardware_i2c hardware_adc pico_stdlib)

pico_enable_stdio_usb(ssd1306 1)
pico_enable_stdio_uart(ssd1306 0)

# create map/bin/hex file etc.
pico_add_extra_outputs(ssd1306)

