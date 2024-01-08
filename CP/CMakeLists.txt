project(ProjectName)
cmake_minimum_required(VERSION 3.22)

include_directories(include)

find_package(PkgConfig)

pkg_check_modules(PC_ZeroMQ QUIET zmq)

find_path(ZeroMQ_INCLUDE_DIR
        NAMES zmq.hpp
        PATHS ${PC_ZeroMQ_INCLUDE_DIRS}
        )

find_library(ZeroMQ_LIBRARY
        NAMES zmq
        PATHS ${PC_ZeroMQ_LIBRARY_DIRS}
        )

add_executable(server ./src/server.cpp ./src/manager.cpp)
add_executable(client ./src/client.cpp ./src/game.cpp)

target_include_directories(server PUBLIC ${ZeroMQ_INCLUDE_DIR})
target_include_directories(client PUBLIC ${ZeroMQ_INCLUDE_DIR})
target_link_libraries(server PUBLIC ${ZeroMQ_LIBRARY} sqlite3)
target_link_libraries(client PUBLIC ${ZeroMQ_LIBRARY} sqlite3)
