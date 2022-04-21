cmake_minimum_required(VERSION 3.20)

find_package(Catch2 REQUIRED)

# These tests can use the Catch2-provided main
#add_executable(tests test.cpp)
#target_link_libraries(tests PRIVATE Catch2::Catch2WithMain)

# These tests need their own main
#add_executable(custom-main-tests test.cpp test-main.cpp)
#target_link_libraries(custom-main-tests PRIVATE Catch2::Catch2)

include(CTest)
include(Catch)

add_executable(Test_KdTree Test_KdTree.cpp)
target_link_libraries(Test_KdTree PRIVATE Catch2::Catch2WithMain)
target_link_libraries(Test_KdTree PUBLIC dataStruct)


catch_discover_tests(Test_KdTree)