cmake_minimum_required(VERSION 3.13)


project(Communication LANGUAGES CXX C)

target_sources(
        ${TARGET_NAME}
        PRIVATE
        CAN/FlexCan/FlexCan.cpp
        SPI/LpSpi/LpSpi.cpp
        UART/LpUartDma/LpUartDma.cpp
)

target_include_directories(
        ${TARGET_NAME}
        PUBLIC
        .
        CAN
        SPI
        UART
)