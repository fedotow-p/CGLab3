cmake_minimum_required(VERSION 4.0)
project(CGLab3)

set(CMAKE_CXX_STANDARD 20)

add_executable(CGLab3 main.cpp
        geometry.h
        tgaimage.h
        tgaimage.cpp
        model.h
        model.cpp
        shader.h
)
