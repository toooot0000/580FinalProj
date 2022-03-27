cmake_minimum_required(VERSION 3.2)

add_library(renderer Renderer.h Renderer.cpp Mesh.cpp Mesh.h)
set_target_properties(renderer PROPERTIES CXX_STANDARD 20)