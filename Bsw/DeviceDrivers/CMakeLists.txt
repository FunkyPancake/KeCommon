cmake_minimum_required(VERSION 3.13)


project(DeviceDrivers LANGUAGES CXX C)

target_sources(
        ${TARGET_NAME}
        PRIVATE
        Tle9461/Tle9461.cpp
)

target_include_directories(
        ${TARGET_NAME}
        PUBLIC
        Tle9461
)