cmake_minimum_required(VERSION 3.1)
project(algorithms)
find_package(GSL REQUIRED)
add_library(ndvector STATIC
    ndvector.c)
target_link_libraries(ndvector
    ${GSL_LIBRARIES})

add_executable(${PROJECT_NAME} "main.c")
