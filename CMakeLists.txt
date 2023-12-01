cmake_minimum_required(VERSION 3.25)


project(KeCommon LANGUAGES CXX C)


add_library(KeCommon
        #
        nxp/source/semihost_hardfault.c
        nxp/source/cpp_config.cpp
        #
        Bsw/DeviceDrivers/CAN/FlexCan/FlexCan.cpp
        Bsw/DeviceDrivers/SPI/LpSpi/LpSpi.cpp
        Bsw/DeviceDrivers/Tle9461/Tle9461.cpp
        Bsw/DeviceDrivers/UART/LpUart/LpUart.cpp
)
if (${MCU} MATCHES MKE16)
    target_sources(KeCommon
            PRIVATE
            nxp/startup/startup_mke16f16.cpp
            nxp/device/system_MKE16F16.c)
elseif (${MCU} MATCHES MKE18)
    target_sources(KeCommon
            PRIVATE
            nxp/startup/startup_mke18f16.cpp
            nxp/device/system_MKE18F16.c)
endif ()
target_include_directories(
        KeCommon
        PUBLIC
        Bsw/Interfaces
        nxp/device
        nxp/CMSIS
        nxp/source
        Bsw/DeviceDrivers/CAN/FlexCan
        Bsw/DeviceDrivers/SPI/LpSpi
        Bsw/DeviceDrivers/Tle9461
        Bsw/DeviceDrivers/UART/LpUart
        Interfaces
        nxp/device
        nxp/CMSIS
        nxp/source
        DeviceDrivers/FlexCan
        DeviceDrivers/LpSpi
        DeviceDrivers/Tle9461
        DeviceDrivers/LpUart
        Bsw/Utils/MapLookup
        Bsw/Utils/Calibration
)

add_subdirectory(nxp/drivers)
add_subdirectory(nxp/freertos)
add_subdirectory(Bsw)

target_compile_definitions(KeCommon
        PRIVATE
        ${MCU}
        ${MCU}_cm4
        #        CPU_MKE16F256VLH16
        #        CPU_MKE16F256VLH16_cm4
        FSL_RTOS_FREE_RTOS
        SDK_OS_FREE_RTOS
        SDK_DEBUGCONSOLE=0
        __MCUXPRESSO
        __USE_CMSIS
        NDEBUG
        __NEWLIB__
)

target_compile_options(KeCommon PRIVATE
        -O0 -fno-common -g3 -Wall -c -ffunction-sections -fdata-sections -ffreestanding
        -fno-builtin -fno-exceptions -fmerge-constants -mcpu=cortex-m4 -mfpu=fpv4-sp-d16 -mfloat-abi=hard
        -mthumb -fstack-usage -specs=nano.specs)