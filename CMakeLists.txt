cmake_minimum_required(VERSION 3.1)

set (CMAKE_BUILD_TYPE Debug FORCE)

project ( Safe CXX )

set ( CXX_STANDARD 11 ) 

#if ( "${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU" OR "${CMAKE_CXX_COMPILER_ID}" STREQUAL "MINGW")
#  set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
#  MESSAGE("-- GNU Toolchain detected")
#elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
#  MESSAGE("-- MSVC Toolchain detected")
#  set ( CXX_STANDARD 11 )
#else()
#  set ( CXX_STANDARD 11 ) 
#endif()

include_directories (include)

set (SAFE_HEADER include/safe.hpp)

add_executable ( HEAP examples/heap.cpp ${SAFE_HEADER} )

add_executable ( BOUNDS examples/bounds.cpp ${SAFE_HEADER} )

add_executable ( DEMO examples/demo.cpp ${SAFE_HEADER} )

add_executable ( TEST_1 unit_test/mem_heap_test.cpp ${SAFE_HEADER} )

enable_testing()

add_test ( HEAP_TEST HEAP )

add_test ( BOUNDS_TEST BOUNDS )

add_test ( DEMO_TEST DEMO )

add_test ( TEST_1 TEST_1 )
