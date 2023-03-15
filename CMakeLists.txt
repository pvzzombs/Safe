cmake_minimum_required(VERSION 3.1)

set (CMAKE_BUILD_TYPE Debug FORCE)

project ( Safe CXX )

set ( CXX_STANDARD 11 ) 

include_directories (include)

set (SAFE_HEADER include/safe.hpp)

#set (COVERAGE_CXX_FLAGS "--coverage -g -O0")

#set (CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} ${COVERAGE_CXX_FLAGS}")

add_executable ( HEAP test/heap.cpp ${SAFE_HEADER} )

add_executable ( BOUNDS test/bounds.cpp ${SAFE_HEADER} )

add_executable ( DEMO test/demo.cpp ${SAFE_HEADER} )

add_executable ( TEST_1 test/test1.cpp ${SAFE_HEADER} )

enable_testing()

add_test ( HEAP_TEST HEAP )

add_test ( BOUNDS_TEST BOUNDS )

add_test ( DEMO_TEST DEMO )

add_test ( TEST_1 TEST_1 )
