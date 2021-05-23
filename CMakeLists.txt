cmake_minimum_required(VERSION 3.13)


project(KeCommon LANGUAGES CXX C)


target_sources(${TARGET_NAME}
        PRIVATE
        DeviceDrivers/flexcan/flexcan.cpp
        )

target_include_directories(
        ${TARGET_NAME}
        PUBLIC
        Interfaces
        DeviceDrivers/flexcan
)

