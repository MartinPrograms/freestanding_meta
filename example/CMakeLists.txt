project(freestanding_meta_example LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 26)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS ON)
set(CMAKE_CXX_STANDARD_LIBRARIES "")
set(CMAKE_CXX_IMPLICIT_LINK_LIBRARIES "")
set(CMAKE_CXX_IMPLICIT_LINK_DIRECTORIES "")

add_executable(freestanding_meta_example main.cpp)
target_link_libraries(freestanding_meta_example PRIVATE freestanding_meta)
target_compile_options(freestanding_meta_example PRIVATE -freflection)