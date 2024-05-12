add_executable(zero_to_pico
        zero_to_pico.c
        )

# pull in common dependencies and additional uart hardware support
target_link_libraries(zero_to_pico pico_stdlib hardware_uart hardware_irq hardware_i2c)


pico_enable_stdio_usb(zero_to_pico 1)
pico_enable_stdio_uart(zero_to_pico 0)

# create map/bin/hex file etc.
pico_add_extra_outputs(zero_to_pico)






