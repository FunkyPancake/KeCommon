cmake_minimum_required(VERSION 3.25)

project(drivers VERSION 1.0.0 LANGUAGES CXX C)

if (DEFINED DAC)
    target_sources(KeCommon PRIVATE
            fsl_dac32.c
    )
endif ()

if (DEFINED ADc)
    target_sources(KeCommon PRIVATE
            fsl_adc12.c
    )
endif ()

if (DEFINED LPIT)
    target_sources(KeCommon PRIVATE
            fsl_lpit.c
    )
endif ()

if (DEFINED FTM)
    target_sources(KeCommon PRIVATE
            fsl_ftm.c
    )
endif ()

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
        fsl_lpuart_edma.c
        freertos/fsl_lpspi_freertos.c
        freertos/fsl_lpuart_freertos.c
        fsl_pdb.c
)
target_include_directories(KeCommon PUBLIC
        .
        freertos
)
