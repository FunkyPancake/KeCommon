cmake_minimum_required(VERSION 3.25)


project(Bsw LANGUAGES CXX C)

target_sources(
        ${TARGET_NAME}
        PRIVATE
        Bootloader/Bootloader.c
)

target_include_directories(
        ${TARGET_NAME}
        PUBLIC
        Bootloader
        Calibration
        Communication
        DeviceDrivers
        Utils
)
add_subdirectory(Utils)
add_subdirectory(Communication)
add_subdirectory(DeviceDrivers)
add_subdirectory(Diag)