cmake_minimum_required(VERSION 3.1)
project(ekeqsl)
find_package(GSL REQUIRED)
add_library(utils STATIC
  graph.c
  ndvector.c
  eikonal_equation.c
  triangle.c
  fsm.c
  )

target_link_libraries(utils
  ${GSL_LIBRARIES}
  m)

add_executable(${PROJECT_NAME} "main.c")
target_link_libraries(${PROJECT_NAME} utils)
