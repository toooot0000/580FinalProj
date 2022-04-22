cmake_minimum_required(VERSION 3.20)
project(580_Final_Proj)

set(CMAKE_CXX_STANDARD 20)

add_subdirectory(./util)
add_subdirectory(./linear)
add_subdirectory(./renderer)
add_subdirectory(./dataStructure)

add_subdirectory(./lib/lodepng-master)
add_subdirectory(./lib/tgaimage)


add_executable(580_Final_Proj main.cpp ObjLoader.h ObjLoader.cpp)
target_include_directories(580_Final_Proj PRIVATE ./lib)

target_link_libraries(
        580_Final_Proj
        PUBLIC linear
        PUBLIC tgaimage
        PUBLIC lodepng
        PUBLIC renderer
        PUBLIC util
        PUBLIC dataStruct
)

add_custom_target(
        copy-files ALL
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/obj
        ${CMAKE_BINARY_DIR}/obj
        )
add_dependencies(580_Final_Proj copy-files)


# Test Related
add_subdirectory(./test)