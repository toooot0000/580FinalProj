cmake_minimum_required(VERSION 3.2)

add_library(linear INTERFACE)
set_target_properties(linear PROPERTIES CXX_STANDARD 20 LINKER_LANGUAGE CXX)
target_include_directories(linear INTERFACE ./)