cmake_minimum_required(VERSION 3.13)


project(Diag LANGUAGES CXX C)

target_sources(
        ${TARGET_NAME}
        PRIVATE
        CanTp.cpp
        Uds.cpp
)

target_include_directories(
        ${TARGET_NAME}
        PUBLIC
        .
)