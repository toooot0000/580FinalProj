cmake_minimum_required(VERSION 3.2)

add_library(lodepng lodepng.h lodepng.cpp)
set_target_properties(lodepng PROPERTIES CXX_STANDARD 20)