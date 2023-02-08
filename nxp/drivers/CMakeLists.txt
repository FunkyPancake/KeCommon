cmake_minimum_required(VERSION 3.13)

project(drivers VERSION 1.0.0 LANGUAGES CXX C)

target_sources(KeCommon PRIVATE
        fsl_cache.c
        fsl_clock.c
        fsl_common.c
        fsl_dmamux.c
        fsl_edma.c
        fsl_flexcan.c
        fsl_flexcan_edma.c
        fsl_gpio.c
        fsl_lpspi.c
        fsl_smc.c
        fsl_lpuart.c
        freertos/fsl_lpspi_freertos.c
        freertos/fsl_lpuart_freertos.c
    )
target_include_directories(KeCommon PUBLIC
        .
        freertos
    )
