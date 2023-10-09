find_package(RAGEL 6.6 REQUIRED)
RAGEL_TARGET(parser parser.rl ${CMAKE_CURRENT_SOURCE_DIR}/parser.c COMPILE_FLAGS -G2)

add_library(resolvconf SHARED error.c parser.c resolvconf.c vector_it.c vector.c)
target_include_directories(resolvconf PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

install(TARGETS resolvconf LIBRARY)
file(GLOB resolvconf_HEADERS "${CMAKE_CURRENT_SOURCE_DIR}/include/*.h")
install(FILES ${resolvconf_HEADERS} DESTINATION include/libresolvconf)