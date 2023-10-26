
# enable_testing()
# add_executable(tests ./test/test.cpp ./src/Experiment.cpp)
# target_link_libraries(GTest::GTest INTERFACE gtest_main)
# target_link_libraries(tests PRIVATE GTest::GTest)

add_executable(lab2 ./src/Experiment.cpp main.cpp)

target_include_directories(lab2 PRIVATE include)