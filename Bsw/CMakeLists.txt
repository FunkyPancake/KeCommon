cmake_minimum_required(VERSION 3.25)


project(Bsw LANGUAGES CXX C)

target_sources(
        ${TARGET_NAME}
        PRIVATE
        Diag/CanTp.cpp
        Diag/Uds.cpp
        Bootloader/Bootloader.c
)

target_include_directories(
        ${TARGET_NAME}
        PUBLIC
        Calibration
        DeviceDrivers
)