
# enable_testing()
# add_executable(tests ./test/test.cpp ./src/parent.cpp)
# target_link_libraries(GTest::GTest INTERFACE gtest_main)
# target_link_libraries(tests PRIVATE GTest::GTest)

add_executable(lab1 ./src/parent.cpp main.cpp)

target_include_directories(lab1 PRIVATE include)


add_executable(child ./src/child.cpp)

target_include_directories(child PRIVATE include)

add_dependencies(lab1 child)