# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.5)

file(MAKE_DIRECTORY
  "/Users/michaeljenz/Documents/ME433/pico/pico-sdk/tools/pioasm"
  "/Users/michaeljenz/documents/ME433/pico/pico-hw/build/pioasm"
  "/Users/michaeljenz/documents/ME433/pico/pico-hw/build/pico-sdk/src/rp2_common/pico_cyw43_driver/pioasm"
  "/Users/michaeljenz/documents/ME433/pico/pico-hw/build/pico-sdk/src/rp2_common/pico_cyw43_driver/pioasm/tmp"
  "/Users/michaeljenz/documents/ME433/pico/pico-hw/build/pico-sdk/src/rp2_common/pico_cyw43_driver/pioasm/src/PioasmBuild-stamp"
  "/Users/michaeljenz/documents/ME433/pico/pico-hw/build/pico-sdk/src/rp2_common/pico_cyw43_driver/pioasm/src"
  "/Users/michaeljenz/documents/ME433/pico/pico-hw/build/pico-sdk/src/rp2_common/pico_cyw43_driver/pioasm/src/PioasmBuild-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "/Users/michaeljenz/documents/ME433/pico/pico-hw/build/pico-sdk/src/rp2_common/pico_cyw43_driver/pioasm/src/PioasmBuild-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "/Users/michaeljenz/documents/ME433/pico/pico-hw/build/pico-sdk/src/rp2_common/pico_cyw43_driver/pioasm/src/PioasmBuild-stamp${cfgdir}") # cfgdir has leading slash
endif()
