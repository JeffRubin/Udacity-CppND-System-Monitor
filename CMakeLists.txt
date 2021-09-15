# Changes for thread support from: https://stackoverflow.com/questions/1620918/cmake-and-libpthread

cmake_minimum_required(VERSION 2.6)
project(monitor)

find_package(Curses REQUIRED)
find_package (Threads REQUIRED)

include_directories(${CURSES_INCLUDE_DIRS})
include_directories(include)

file(GLOB SOURCES "src/*.cpp")

add_executable(monitor ${SOURCES})

set_property(TARGET monitor PROPERTY CXX_STANDARD 17)
target_link_libraries(monitor ${CURSES_LIBRARIES} ${CMAKE_THREAD_LIBS_INIT})
target_compile_options(monitor PRIVATE -Wall -Wextra)
