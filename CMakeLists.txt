cmake_minimum_required(VERSION 3.9)

project(libresolvconf VERSION 0.0.1 LANGUAGES C)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_SOURCE_DIR}/cmake)

include(CTest)
enable_testing()

add_subdirectory(docs)
add_subdirectory(src/libresolvconf)
add_subdirectory(src/example)
add_subdirectory(src/utils)
add_subdirectory(tests)