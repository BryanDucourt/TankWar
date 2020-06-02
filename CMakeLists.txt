cmake_minimum_required(VERSION 3.15)
project(tank C)

set(CMAKE_C_STANDARD 99)

add_executable(tank main.c tank.h draw.h)