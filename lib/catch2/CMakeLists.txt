cmake_minimum_required(VERSION 3.2)

add_library(catch2 INTERFACE)
set_target_properties(catch2 PROPERTIES CXX_STANDARD 20 LINKER_LANGUAGE CXX)
target_include_directories(catch2 INTERFACE ./)