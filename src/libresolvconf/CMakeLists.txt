find_package(RAGEL 6.6 REQUIRED)
RAGEL_TARGET(parser parser.rl ${CMAKE_CURRENT_SOURCE_DIR}/parser.c COMPILE_FLAGS "-eLG2")

add_library(resolvconf SHARED dynarray_cit.c dynarray.c error.c parser.c resolvconf.c)
target_include_directories(resolvconf
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include/libresolvconf)
target_compile_options(resolvconf PRIVATE -Wall)

install(TARGETS resolvconf LIBRARY)
install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/include/libresolvconf DESTINATION include)