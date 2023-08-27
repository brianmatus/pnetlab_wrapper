cmake_minimum_required(VERSION 3.26)
project(pnet_wrapper)

set(CMAKE_CXX_STANDARD 17)

add_executable(pnet_wrapper main.cpp)
set(CMAKE_EXE_LINKER_FLAGS "-static-libgcc -static-libstdc++") #To be library independent