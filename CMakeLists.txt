cmake_minimum_required(VERSION 3.15.4)
project(rasterizer)

add_definitions("/W3")

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

file(GLOB SOURCES src/*.cpp)
include_directories(include)
add_executable(main ${SOURCES})
target_link_libraries(main ${CONAN_LIBS})
