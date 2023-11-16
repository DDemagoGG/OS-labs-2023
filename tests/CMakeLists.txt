include(FetchContent)

FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG        release-1.11.0
)

FetchContent_MakeAvailable(googletest)
add_library(GTest::GTest INTERFACE IMPORTED)
target_link_libraries(GTest::GTest INTERFACE gtest_main)


set(CMAKE_CXX_STANDARD 17)


add_executable(lab1_test lab1_test.cpp ../lab1/src/parent.cpp)

target_include_directories(lab1_test PRIVATE ../lab1/include)
target_link_libraries(lab1_test PRIVATE GTest::GTest)

add_test(lab1_test lab1_test)


add_executable(lab2_test lab2_test.cpp ../lab2/src/Experiment.cpp)

target_include_directories(lab2_test PRIVATE ../lab2/include)
target_link_libraries(lab2_test PRIVATE GTest::GTest)

add_test(lab2_test lab2_test)


add_executable(lab3_test lab3_test.cpp ../lab3/src/parent.cpp)

target_include_directories(lab3_test PRIVATE ../lab3/include)
target_link_libraries(lab3_test PRIVATE GTest::GTest)

add_test(lab3_test lab3_test)


add_library(1 SHARED ../lab4/src/implementation1.c)
add_library(2 SHARED ../lab4/src/implementation2.c)

target_include_directories(1 PUBLIC ../lab4/include)
target_include_directories(2 PUBLIC ../lab4/include)

add_executable(lab4_static_test lab4_static_test.cpp)
add_executable(lab4_dynamic_test lab4_dynamic_test.cpp)

target_link_libraries(lab4_static_test PRIVATE 1)
target_include_directories(lab4_dynamic_test PRIVATE ../lab4/include)

target_link_libraries(lab4_static_test PRIVATE GTest::GTest)
target_link_libraries(lab4_dynamic_test PRIVATE GTest::GTest)

add_test(lab4_static_test lab4_static_test)
add_test(lab4_dynamic_test lab4_dynamic_test)