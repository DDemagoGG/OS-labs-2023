add_executable(lab1 ./src/parent.cpp main.cpp)

target_include_directories(lab1 PRIVATE include)


add_executable(child1 ./src/child.cpp)

target_include_directories(child1 PRIVATE include)

add_dependencies(lab1 child1)