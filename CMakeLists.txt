cmake_minimum_required(VERSION 3.23)
project(cutiedb)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(cutiedb
        main.cpp
        messages.hpp
        server.cpp
        server.hpp cuteLittle.cpp cute_little.hpp)
