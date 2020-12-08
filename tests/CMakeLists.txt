project(tests)

add_subdirectory(googletest)

add_compile_options(-Wall -Wextra -pedantic)

add_executable(tests scmap.cpp oamap.cpp)
target_link_libraries(tests gtest gtest_main hashmap)