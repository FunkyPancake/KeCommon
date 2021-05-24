cmake_minimum_required(VERSION 3.13)


project(KeCommon LANGUAGES CXX C)


add_library(KeCommon
        device/system_MKE16F16.c
        DeviceDrivers/flexcan/flexcan.cpp
        source/semihost_hardfault.c
        source/cpp_config.cpp
        )

add_subdirectory(drivers)
add_subdirectory(freertos)

target_include_directories(
        KeCommon
        PUBLIC
        Interfaces
        DeviceDrivers/flexcan
        device
        CMSIS
        source
)

target_compile_definitions(KeCommon
        PRIVATE
        CPU_MKE16F256VLH16
        CPU_MKE16F256VLH16_cm4
        FSL_RTOS_FREE_RTOS
        SDK_OS_FREE_RTOS
        SDK_DEBUGCONSOLE=1
        __MCUXPRESSO
        __USE_CMSIS
        DEBUG
        __NEWLIB__
        )

target_compile_options(KeCommon PRIVATE
        -O0 -fno-common -g3 -Wall -c -ffunction-sections -fno-rtti -fdata-sections -ffreestanding
        -fno-builtin -fno-exceptions -fmerge-constants -mcpu=cortex-m4 -mfpu=fpv4-sp-d16 -mfloat-abi=hard
        -mthumb -fstack-usage -specs=nano.specs)