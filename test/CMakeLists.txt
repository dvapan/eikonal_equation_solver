cmake_minimum_required(VERSION 3.1)

set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O0 -g")

find_package(PkgConfig)
pkg_check_modules(GLIB2 REQUIRED glib-2.0)

include_directories(${GLIB2_INCLUDE_DIRS})
link_directories(${GLIB2_LIBRARY_DIRS})

add_executable(test_ndvector test_ndvector.c)
target_link_libraries(test_ndvector
    ${GLIB2_LIBRARIES}
    ndvector)

add_test(test_ndvector "./test_ndvector")
