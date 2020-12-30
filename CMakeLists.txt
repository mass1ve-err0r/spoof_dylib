cmake_minimum_required(VERSION 3.17)
project(spoof_dylib C)
set(CMAKE_C_STANDARD 11)

file(GLOB _inc "./includes/*.h")
file(GLOB _src "./src/*.c")

add_executable(spoof_dylib ${_inc} ${_src})