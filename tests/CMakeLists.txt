project(tests)

add_subdirectory(googletest)

add_compile_options(-Wall -Wextra -pedantic)

add_executable(tests separate.cpp)
target_link_libraries(tests gtest gtest_main hashmap)