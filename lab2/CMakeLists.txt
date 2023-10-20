cmake_minimum_required(VERSION 3.10)
project(gtest)

find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(include/)

add_executable(main ./src/Experiment.cpp ../main.cpp)

enable_testing()
add_executable(tests ./test/test.cpp ./src/Experiment.cpp)
target_link_libraries(GTest::GTest INTERFACE gtest_main)
target_link_libraries(tests PRIVATE GTest::GTest)