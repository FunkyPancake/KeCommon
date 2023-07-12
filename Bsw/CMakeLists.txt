cmake_minimum_required(VERSION 3.13)


project(Bsw LANGUAGES CXX C)

target_sources(
        ${TARGET_NAME}
        PRIVATE
        Diag/CanTp.cpp
        Diag/Uds.cpp
)

target_include_directories(
        ${TARGET_NAME}
        PUBLIC
        Calibration
)