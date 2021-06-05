cmake_minimum_required(VERSION 3.0)

project(w2w)

add_executable(w2w w2w.c)
target_compile_options(w2w PRIVATE -Wall -Wextra)

install(TARGETS w2w RUNTIME DESTINATION bin)
