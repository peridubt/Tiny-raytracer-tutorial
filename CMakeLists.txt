cmake_minimum_required(VERSION 4.0)
project(tiny_raytracer)

set(CMAKE_CXX_STANDARD 26)

add_executable(tiny_raytracer main.cpp
        geometry.h
        rendering.h
)
