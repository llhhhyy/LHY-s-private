cmake_minimum_required(VERSION 3.27)
project(hw2_code)

# 设置编译器为 GCC
set(CMAKE_CXX_COMPILER "g++")

# 设置 C++ 标准为 C++23
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(.)

add_executable(hw2_code
#        main.cc
        main_ref.cc
        MurmurHash3.h)
