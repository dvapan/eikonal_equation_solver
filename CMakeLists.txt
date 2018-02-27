cmake_minimum_required(VERSION 3.1)

project(eikonal_equation_solver)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

include_directories(src)

enable_testing()

add_subdirectory(src)
add_subdirectory(test)


