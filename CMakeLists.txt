cmake_minimum_required(VERSION 4.3)

project(freestanding_meta)
add_library(freestanding_meta INTERFACE)
target_include_directories(freestanding_meta INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/include)

add_subdirectory(example)
