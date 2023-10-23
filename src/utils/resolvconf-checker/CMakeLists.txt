add_executable(resolvconf-checker main.c)
target_link_libraries(resolvconf-checker resolvconf)
install(TARGETS resolvconf-checker RUNTIME)