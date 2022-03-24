cmake_minimum_required(VERSION 3.2)

add_library(renderer renderer.h renderer.cpp)
set_target_properties(renderer PROPERTIES CXX_STANDARD 20)