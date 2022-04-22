cmake_minimum_required(VERSION 3.2)

add_library(renderer Renderer.h Renderer.cpp Mesh.cpp Mesh.h Tri.cpp Tri.h RayCast.h RayCast.cpp)
set_target_properties(renderer PROPERTIES CXX_STANDARD 20)
target_include_directories(renderer PUBLIC ./)