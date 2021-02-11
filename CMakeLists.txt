cmake_minimum_required(VERSION 3.1)

project(HEAP CXX)

if ( "${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU" OR "${CMAKE_CXX_COMPILER_ID}" STREQUAL "MINGW")
  set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")
  MESSAGE("-- GNU Toolchain detected")
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
  MESSAGE("-- MSVC Toolchain detected")
  set ( CXX_STANDARD 14 )
else()
  set ( CXX_STANDARD 14 ) 
endif()

add_executable( HEAP heap.cpp )

add_executable ( BOUNDS bounds.cpp )

add_executable ( DEMO demo.cpp )

enable_testing()

add_test( HEAP_TEST HEAP )

add_test( BOUNDS_TEST BOUNDS )

add_test( DEMO_TEST DEMO )
