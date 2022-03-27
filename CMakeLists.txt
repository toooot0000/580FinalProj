cmake_minimum_required(VERSION 3.20)
project(580_Final_Proj)

set(CMAKE_CXX_STANDARD 20)

add_subdirectory(./util)
add_subdirectory(./linear)
add_subdirectory(./tgaimage)
add_subdirectory(./renderer)

add_executable(580_Final_Proj main.cpp ObjLoader.h ObjLoader.cpp)
target_link_libraries(580_Final_Proj PRIVATE linear PRIVATE tgaimage PRIVATE renderer PRIVATE util)

add_custom_target(
        copy-files ALL
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/obj
        ${CMAKE_BINARY_DIR}/obj
        )
add_dependencies(580_Final_Proj copy-files)