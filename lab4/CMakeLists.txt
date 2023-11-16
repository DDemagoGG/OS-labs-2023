add_library(lib1 SHARED src/implementation1.c)
add_library(lib2 SHARED src/implementation2.c)

target_include_directories(lib1 PUBLIC include)
target_include_directories(lib2 PUBLIC include)

add_executable(main1 main1.cpp)
add_executable(main2 main2.cpp)

target_link_libraries(main1 PRIVATE lib1)
target_include_directories(main2 PRIVATE include)