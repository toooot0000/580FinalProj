cmake_minimum_required(VERSION 3.2)

add_library(linear Mat.h Vec.h)
set_target_properties(linear PROPERTIES CXX_STANDARD 20 LINKER_LANGUAGE CXX)