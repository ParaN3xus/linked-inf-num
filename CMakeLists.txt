cmake_minimum_required(VERSION 3.13)
project(linked_inf_num)
set(CMAKE_CXX_STANDARD 11)

add_subdirectory(src/uint_linked_list)
add_subdirectory(src/inf_int)
add_executable(linked_inf_num src/main.cpp)

target_link_libraries(linked_inf_num uint_linked_list inf_int)
