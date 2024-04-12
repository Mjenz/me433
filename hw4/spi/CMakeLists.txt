add_executable(spi
            spi.c
            )
# Pull in our pico_stdlib which pulls in commonly used features
target_link_libraries(spi pico_stdlib pico_binary_info hardware_spi)

pico_enable_stdio_usb(spi 1)
pico_enable_stdio_uart(spi 0)

# create map/bin/hex file etc.
pico_add_extra_outputs(spi)

