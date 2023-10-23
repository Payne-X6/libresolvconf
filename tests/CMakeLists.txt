add_executable(test-libresolvconf main.c)
target_link_libraries(test-libresolvconf criterion)
add_test(NAME test-libresolvconf COMMAND test-libresolvconf)