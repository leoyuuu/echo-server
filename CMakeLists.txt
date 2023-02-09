cmake_minimum_required(VERSION 3.4.1)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall")


add_executable(echo-server-tcp tcp-echo-server.c)
add_executable(echo-server-udp udp-echo-server.c)