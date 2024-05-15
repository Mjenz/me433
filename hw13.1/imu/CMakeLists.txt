add_executable(imu
        imu.c
        )

# pull in common dependencies and additional uart hardware support
target_link_libraries(imu pico_stdlib hardware_i2c)

# target_sources(main PUBLIC
#         ${CMAKE_CURRENT_LIST_DIR}/main.c
#         ${CMAKE_CURRENT_LIST_DIR}/usb_descriptors.c
#         )


# # Make sure TinyUSB can find tusb_config.h
# target_include_directories(main PUBLIC ${CMAKE_CURRENT_LIST_DIR})

pico_enable_stdio_usb(imu 1)
pico_enable_stdio_uart(imu 0)

# create map/bin/hex file etc.
pico_add_extra_outputs(imu)

