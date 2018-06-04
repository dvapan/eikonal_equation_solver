cmake_minimum_required(VERSION 3.1)

set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O0 -g")

find_package(PkgConfig)
pkg_check_modules(GLIB2 REQUIRED glib-2.0)

include_directories(${GLIB2_INCLUDE_DIRS})
link_directories(${GLIB2_LIBRARY_DIRS})


add_executable(test_eikonal_equation test_eikonal_equation.c)
target_link_libraries(test_eikonal_equation
  utils)

add_executable(test_nvector test_nvector.c)
target_link_libraries(test_nvector
  utils)

add_executable(test_fsm test_fsm.c)
target_link_libraries(test_fsm
  utils
  ${GLIB2_LIBRARIES})

add_executable(test_graph test_graph.c)
target_link_libraries(test_graph utils)

add_test(eikonal_equation "run_tests")
add_test(nvector "test_nvector")
add_test(fsm "test_fsm" )
add_test(graph "test_graph")
