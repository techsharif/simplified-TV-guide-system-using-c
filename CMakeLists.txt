cmake_minimum_required(VERSION 3.10)
project(TV_guide_system C)

set(CMAKE_C_STANDARD 11)

add_executable(TV_guide_system main.c sched.c)