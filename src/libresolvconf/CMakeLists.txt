find_package(RAGEL 6.6 REQUIRED)
RAGEL_TARGET(parser parser.rl ${CMAKE_CURRENT_SOURCE_DIR}/parser.c COMPILE_FLAGS -G2)

add_library(resolvconf parser.c resolvconf.c vector.c vector_it.c)
target_include_directories(resolvconf PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)