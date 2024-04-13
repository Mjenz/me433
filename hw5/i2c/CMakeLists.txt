add_executable(i2c
            i2c.c
            )

target_link_libraries(i2c pico_stdlib pico_binary_info hardware_i2c)

pico_enable_stdio_usb(i2c 1)
pico_enable_stdio_uart(i2c 0)

# create map/bin/hex file etc.
pico_add_extra_outputs(i2c)

