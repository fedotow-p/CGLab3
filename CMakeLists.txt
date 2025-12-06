cmake_minimum_required(VERSION 4.0)
project(CGLab3)

set(CMAKE_CXX_STANDARD 17)

add_executable(CGLab3 main.cpp
        geometry.h
        tgaimage.h
        tgaimage.cpp
        model.h
        model.cpp
        geometry.cpp
)
