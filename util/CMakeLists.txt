cmake_minimum_required(VERSION 3.2)

add_library(util util.cpp util.h)
set_target_properties(util PROPERTIES CXX_STANDARD 20)
