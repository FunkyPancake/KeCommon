cmake_minimum_required(VERSION 3.13)


project(Utils LANGUAGES CXX C)

target_sources(
    ${TARGET_NAME}
    PRIVATE
)

target_include_directories(
    ${TARGET_NAME}
    PUBLIC
)
add_subdirectory(PidController)