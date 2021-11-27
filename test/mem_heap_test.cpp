#define DEBUG_
#define SAFE_USE_NAMESPACE
#define FILENAME __FILE__

#include <iostream>
#include <cassert>
#include "safe.hpp"
#include "plf_nanotimer.h"

const char * title_1 = "Total elapsed time for ";
const char * title_2 = " ms. \n";
const char * title_3 = "Passed.\n";
const char * title_4 = "Failed.\n";

#define PASSED_MACRO title_3
#define FAILED_MACRO title_4
#define TEST_IF(msg, lhs, rhs) std::cout<<msg<<' '<<((lhs==rhs)?PASSED_MACRO:FAILED_MACRO);

int main(){
  plf::nanotimer timer;
  safe::mem_heap_debug<int> mem_heap;
  int test_a = 12345;
  int test_b = 67890;

  //create new integer pointers to be used;

  int * test_1 = nullptr, * test_2 = nullptr, * test_3 = nullptr, * test_4 = &test_a, * test_5 = &test_b;

  //first test check if mem_heap works

  timer.start();
  mem_heap.add_address(test_1);

  mem_heap.remove_address(test_1);

  //add the 2nd and 3rd
  mem_heap.add_address(test_2);
  mem_heap.add_address(test_3);

  //remove the 2nd & 3rd
  mem_heap.remove_address(test_2);
  mem_heap.remove_address(test_3);

  //allocate then test again
  test_1 = new int[2];
  test_2 = new int[4];
  test_3 = new int[8];

  mem_heap.add_address(test_1);

  mem_heap.remove_address(test_1);

  mem_heap.add_address(test_2);

  mem_heap.remove_address(test_2);

  mem_heap.add_address(test_3);

  mem_heap.remove_address(test_3);

  //arrangement test
  //1x 2 3
  mem_heap.add_address(test_1);
  mem_heap.add_address(test_2);
  mem_heap.add_address(test_3);

  mem_heap.remove_address(test_1);

  //test if 0 == test_2 && 0 == test_3
  TEST_IF("[0] == test_2", mem_heap[0], test_2)
  TEST_IF("[1] == test_3", mem_heap[1], test_3)


  //2 3x 1

  mem_heap.add_address(test_1);
  TEST_IF("[2] == test_1", mem_heap[2], test_1)
  mem_heap.remove_address(test_3);
  TEST_IF("[0] == test_2", mem_heap[0], test_2)
  TEST_IF("[1] == test_1", mem_heap[1], test_1)

  //2 1 3x
  mem_heap.add_address(test_3);
  mem_heap.remove_address(test_3);
  TEST_IF("[0] == test_2", mem_heap[0], test_2)
  TEST_IF("[1] == test_1", mem_heap[1], test_1)

  // 2 1 a+ 3+ b+
  mem_heap.add_address(test_4);
  mem_heap.add_address(test_3);
  mem_heap.add_address(test_5);
  TEST_IF("[0] == test_2", mem_heap[0], test_2)
  TEST_IF("[1] == test_1", mem_heap[1], test_1)
  TEST_IF("[2] == test_4", mem_heap[2], test_4)
  TEST_IF("[3] == test_3", mem_heap[3], test_3)
  TEST_IF("[4] == test_5", mem_heap[4], test_5)

  delete [] test_1;
  delete [] test_2;
  delete [] test_3;

  mem_heap.delete_list();

  std::cout << title_1 << FILENAME << ": " << timer.get_elapsed_ms() << title_2;
  return 0;
}