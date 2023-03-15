#define DEBUG_
#define SAFE_USE_NAMESPACE

#include "acutest.h"
#include "safe.hpp"

void test_mem_heap(){
  safe::mem_heap_debug<int> mem_heap;
  int test_a = 12345;
  int test_b = 67890;

  //create new integer pointers to be used;

  int * test_1 = nullptr, * test_2 = nullptr, * test_3 = nullptr, * test_4 = &test_a, * test_5 = &test_b;

  //first test check if mem_heap works

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
  TEST_CHECK(mem_heap[0] == test_2);
  TEST_MSG("Expected [0] == test_2");
  TEST_CHECK(mem_heap[1] == test_3);
  TEST_MSG("Expected [1] == test_3");


  //2 3x 1

  mem_heap.add_address(test_1);
  TEST_CHECK(mem_heap[2] == test_1);
  TEST_MSG("Expected [2] == test_1");
  mem_heap.remove_address(test_3);
  TEST_CHECK(mem_heap[0] == test_2);
  TEST_MSG("Expected [0] == test_2");
  TEST_CHECK(mem_heap[1] == test_1);
  TEST_MSG("Expected [1] == test_1");

  //2 1 3x
  mem_heap.add_address(test_3);
  mem_heap.remove_address(test_3);
  TEST_CHECK(mem_heap[0] == test_2);
  TEST_MSG("Expected [0] == test_2");
  TEST_CHECK(mem_heap[1] == test_1);
  TEST_MSG("Expected [1] == test_1");

  // 2 1 a+ 3+ b+
  mem_heap.add_address(test_4);
  mem_heap.add_address(test_3);
  mem_heap.add_address(test_5);
  TEST_CHECK(mem_heap[0] == test_2);
  TEST_MSG("Expected [0] == test_2");
  TEST_CHECK(mem_heap[1] == test_1);
  TEST_MSG("Expected [1] == test_1");
  TEST_CHECK(mem_heap[2] == test_4);
  TEST_MSG("Expected [2] == test_4");
  TEST_CHECK(mem_heap[3] == test_3);
  TEST_MSG("Expected [3] == test_3");
  TEST_CHECK(mem_heap[4] == test_5);
  TEST_MSG("Expected [4] == test_5");

  // test searching capabilities
  
  TEST_CHECK(mem_heap.find_address(test_3) == true);
  TEST_MSG("Expected address test_3 to be found");
  
  TEST_CHECK(mem_heap.find_address(test_5) == true);
  TEST_MSG("Expected address test_5 to be found");
  
  TEST_CHECK(mem_heap.find_address(test_4) == true);
  TEST_MSG("Expected address test_4 to be found");

  // clean up
  delete [] test_1;
  delete [] test_2;
  delete [] test_3;

  mem_heap.delete_list();
  TEST_CHECK(mem_heap[0] == nullptr);
  TEST_MSG("Expected mem_heap_debug to be empty");
  //TEST_IF("Fail the test", false, true)

  //std::cout << std::endl << title_1 << FILENAME << ": " << timer.get_elapsed_ms() << title_2;
  //std::cout << (TOTAL_TEST==PASS_TEST?"All Test Passed :)":"Test Failed :(") << std::endl;
}

TEST_LIST = {
  { "Mem Heap", test_mem_heap},
  { NULL, NULL }
};