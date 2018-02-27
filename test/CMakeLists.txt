cmake_minimum_required(VERSION 3.1)

set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O0 -g")

find_package(PkgConfig)
pkg_check_modules(GLIB2 REQUIRED glib-2.0)

include_directories(${GLIB2_INCLUDE_DIRS})
link_directories(${GLIB2_LIBRARY_DIRS})


add_executable(run_tests run_tests.c)
target_link_libraries(run_tests
    ${GLIB2_LIBRARIES}
    utils)

add_test(run_tests "./test/run_tests")
