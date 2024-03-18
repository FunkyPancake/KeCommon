cmake_minimum_required(VERSION 3.25)


project(KeCommon LANGUAGES CXX C)


add_library(KeCommon
        nxp/source/semihost_hardfault.c
        nxp/source/cpp_config.cpp
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
        nxp/device
        nxp/CMSIS/Include
        nxp/source
        Interfaces
)

add_subdirectory(nxp/drivers)
add_subdirectory(nxp/freertos)
add_subdirectory(Bsw)

target_compile_definitions(KeCommon
        PRIVATE
        ${MCU}
        ${MCU}_cm4
        FSL_RTOS_FREE_RTOS
        SDK_OS_FREE_RTOS
        SDK_DEBUGCONSOLE=0
        __MCUXPRESSO
        __USE_CMSIS
        NDEBUG
        __NEWLIB__
)
target_compile_options(KeCommon PRIVATE
        -O1 -fno-common -g3 -Wall -c -ffunction-sections -fdata-sections -ffreestanding
        -fno-exceptions -fmerge-constants -mcpu=cortex-m4 -mfpu=fpv4-sp-d16 -mfloat-abi=hard
        -mthumb -fstack-usage -specs=nano.specs)

if(DEFINED DSP)
    #Download and build new CMSIS DSP library, since nxp provides obsolete version
    set(CMSISCORE ${CMAKE_CURRENT_SOURCE_DIR}/nxp/CMSIS)
    include(FetchContent)
    FetchContent_Declare(
            CMSIS_DSP
            URL https://github.com/ARM-software/CMSIS-DSP/releases/download/v1.15.0/ARM.CMSIS-DSP.1.15.0.pack
    )
    FetchContent_MakeAvailable(CMSIS_DSP)
    add_definitions(-DARM_MATH_CM4)

    add_subdirectory(${cmsis_dsp_SOURCE_DIR}/Source bin_dsp)
    target_compile_options(CMSISDSP PRIVATE -mcpu=cortex-m4 -mfpu=fpv4-sp-d16 -mfloat-abi=hard -ffunction-sections -fdata-sections
            -Wsign-compare -Wdouble-promotion -Ofast -ffast-math -DNDEBUG -Wall -Wextra -Werror
            -fshort-enums -mfloat-abi=hard)
    target_include_directories(KeCommon PUBLIC ${cmsis_dsp_SOURCE_DIR}/Include)
    target_link_libraries(KeCommon CMSISDSP)

endif ()