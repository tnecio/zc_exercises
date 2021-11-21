cmake_minimum_required(VERSION 3.20)
project(libvirt_exercise)

set(CMAKE_CXX_STANDARD 14)

add_executable(libvirt_exercise main.cpp)
target_link_libraries(libvirt_exercise virt)