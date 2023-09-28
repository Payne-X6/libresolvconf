cmake_minimum_required(VERSION 3.8.12...3.26)
project(libresolvconf C)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_SOURCE_DIR}/cmake)

add_subdirectory(src/libresolvconf)
add_subdirectory(src/example)