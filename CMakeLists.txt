
project(HEAP CXX)

set ( CXX_STANDARD 17 )

add_executable( HEAP heap.cpp )

add_executable ( BOUNDS bounds.cpp )

add_executable ( DEMO demo.cpp )

enable_testing()

add_test( HEAP_TEST HEAP )

add_test( BOUNDS_TEST BOUNDS )

add_test( DEMO_TEST DEMO )
