cmake_minimum_required(VERSION 3.17)
project(assignment-6)

set(CMAKE_CXX_STANDARD 17)
add_compile_options(-Wall -Wextra -pedantic)

add_subdirectory(tests)

add_library(hashmap STATIC separate.h)
set_target_properties(hashmap PROPERTIES LINKER_LANGUAGE CXX)