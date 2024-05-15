# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.5)

file(MAKE_DIRECTORY
  "/Users/michaeljenz/Documents/pico/pico-sdk/tools/elf2uf2"
  "/Users/michaeljenz/Documents/pico/me433/hw13/build/elf2uf2"
  "/Users/michaeljenz/Documents/pico/me433/hw13/build/imu/elf2uf2"
  "/Users/michaeljenz/Documents/pico/me433/hw13/build/imu/elf2uf2/tmp"
  "/Users/michaeljenz/Documents/pico/me433/hw13/build/imu/elf2uf2/src/ELF2UF2Build-stamp"
  "/Users/michaeljenz/Documents/pico/me433/hw13/build/imu/elf2uf2/src"
  "/Users/michaeljenz/Documents/pico/me433/hw13/build/imu/elf2uf2/src/ELF2UF2Build-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "/Users/michaeljenz/Documents/pico/me433/hw13/build/imu/elf2uf2/src/ELF2UF2Build-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "/Users/michaeljenz/Documents/pico/me433/hw13/build/imu/elf2uf2/src/ELF2UF2Build-stamp${cfgdir}") # cfgdir has leading slash
endif()
