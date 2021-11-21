cmake_minimum_required(VERSION 3.16)
project(dll_exercise C)
enable_testing()

set(CMAKE_C_STANDARD 99)

add_library(dll_exercise library.c)

add_executable(test_main library.c test.c)
add_test(NAME TestMain COMMAND test_main)