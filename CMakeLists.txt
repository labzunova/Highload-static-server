cmake_minimum_required(VERSION 3.16)
project(Highload_static_server)

#set(CMAKE_CXX_FLAGS "-Werror -Wpedantic -std=c++1z -pthread")
set(CMAKE_CXX_FLAGS "-pthread")
set(CMAKE_CXX_STANDARD 20)

set(HEADERS include/ThreadPool.h include/Parser.h include/Parser.h)
set(SRC src/ThreadPool.cpp  src/Handler.cpp src/Parser.cpp)

add_executable(Highload_static_server ${HEADERS} ${SRC} main.cpp)
