cmake_minimum_required(VERSION 3.20)
project(CAST_128)

set(CMAKE_CXX_STANDARD 17)

add_executable(CAST_128 main.cpp image_bmp24.cpp image_bmp24.h CAST_128.cpp CAST_128.h)
