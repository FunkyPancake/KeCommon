cmake_minimum_required(VERSION 3.25)


project(PID LANGUAGES CXX C)

target_sources(
        ${TARGET_NAME}
        PRIVATE
        PidController.cpp
)

target_include_directories(
        ${TARGET_NAME}
        PUBLIC
        .
)