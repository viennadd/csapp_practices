cmake_minimum_required(VERSION 3.3)
project(csapp)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES sysio/sysio.h sysio/sysio.cpp inet/sock.h inet/sock.cpp concurrent/multi_threaded_echo.cpp concurrent/concurrent.h err_handling/error_wrapper.cpp err_handling/error_wrapper.h concurrent/concurrent.cpp)

add_executable(csapp ${SOURCE_FILES})