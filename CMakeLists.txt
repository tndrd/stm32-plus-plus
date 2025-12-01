cmake_minimum_required(VERSION 3.20)
project(stm32-plus-plus CXX ASM)

set(CMAKE_CXX_STANDARD 17)

set(LWIP_DIR ${CMAKE_CURRENT_SOURCE_DIR}/lib/stm32_mw_lwip)

# Turnaround to remove bridgeif_fdb.c from sources
# It is not used but does break a build
file(RENAME ${LWIP_DIR}/src/netif/bridgeif_fdb.c ${LWIP_DIR}/src/netif/__bridgeif_fdb.c)
file(TOUCH  ${LWIP_DIR}/src/netif/bridgeif_fdb.c)

set (LWIP_INCLUDE_DIRS
    "${LWIP_DIR}/src/include"
    "${LWIP_DIR}/system"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/ethernet"
)
add_subdirectory(lib/stm32_mw_lwip)

target_include_directories(lwipcore PUBLIC ${LWIP_INCLUDE_DIRS})

include(sources.cmake)

add_library(stm32pp-core EXCLUDE_FROM_ALL ${STM32PP_CORE_SOURCES} ${STM32PP_COMMON_SOURCES})
target_include_directories(stm32pp-core PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src/)

add_library(stm32pp-audio EXCLUDE_FROM_ALL ${STM32PP_AUDIO_SOURCES})
target_link_libraries(stm32pp-audio PUBLIC stm32pp-core)

add_library(stm32pp-canopen EXCLUDE_FROM_ALL ${STM32PP_CANOPEN_SOURCES})
target_link_libraries(stm32pp-canopen PUBLIC stm32pp-core)

add_library(stm32pp-device EXCLUDE_FROM_ALL ${STM32PP_DEVICE_SOURCES})
target_link_libraries(stm32pp-device PUBLIC stm32pp-core)

add_library(stm32pp-ethernet EXCLUDE_FROM_ALL ${STM32PP_ETHERNET_SOURCES})
target_link_libraries(stm32pp-ethernet PUBLIC stm32pp-core lwipcore)

add_library(stm32pp-gfx EXCLUDE_FROM_ALL ${STM32PP_GFX_SOURCES})
target_link_libraries(stm32pp-gfx PUBLIC stm32pp-core)

add_library(stm32pp-modbus EXCLUDE_FROM_ALL ${STM32PP_MODBUS_SOURCES})
target_link_libraries(stm32pp-modbus PUBLIC stm32pp-core lwipcore)

add_library(stm32pp-objnet EXCLUDE_FROM_ALL ${STM32PP_OBJNET_SOURCES})
target_link_libraries(stm32pp-objnet PUBLIC stm32pp-core)

add_library(stm32pp-radio EXCLUDE_FROM_ALL ${STM32PP_RADIO_SOURCES})
target_link_libraries(stm32pp-radio PUBLIC stm32pp-core)

add_library(stm32pp-usb EXCLUDE_FROM_ALL ${STM32PP_USB_SOURCES})
target_link_libraries(stm32pp-usb PUBLIC stm32pp-core)

add_library(stm32pp-serial EXCLUDE_FROM_ALL ${STM32PP_SERIAL_SOURCES})
target_link_libraries(stm32pp-serial PUBLIC stm32pp-usb)

# ONB Interfaces
add_library(stm32pp-objnet-can-if EXCLUDE_FROM_ALL ${STM32PP_OBJNET_CAN_IF_SOURCES})
target_link_libraries(stm32pp-objnet-can-if PUBLIC stm32pp-objnet)

add_library(stm32pp-objnet-uart-if EXCLUDE_FROM_ALL ${STM32PP_OBJNET_UART_IF_SOURCES})
target_link_libraries(stm32pp-objnet-uart-if PUBLIC stm32pp-objnet)

add_library(stm32pp-objnet-usbhid-if EXCLUDE_FROM_ALL ${STM32PP_OBJNET_USBHID_IF_SOURCES})
target_link_libraries(stm32pp-objnet-usbhid-if PUBLIC stm32pp-objnet stm32pp-usb)

add_library(stm32pp-objnet-udp-if EXCLUDE_FROM_ALL ${STM32PP_OBJNET_UDP_IF_SOURCES})
target_link_libraries(stm32pp-objnet-udp-if PUBLIC stm32pp-objnet stm32pp-ethernet)

add_library(stm32pp-objnet-radio-if EXCLUDE_FROM_ALL ${STM32PP_OBJNET_RADIO_IF_SOURCES})
target_link_libraries(stm32pp-objnet-radio-if PUBLIC stm32pp-objnet stm32pp-radio)

add_library(stm32pp-objnet-ble-if EXCLUDE_FROM_ALL ${STM32PP_OBJNET_BLE_IF_SOURCES})
target_link_libraries(stm32pp-objnet-ble-if PUBLIC stm32pp-objnet)