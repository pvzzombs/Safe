#define DEBUG_
#include "safe.hpp"
mem_heap_debug<int> gb;
int main(){
  arr_<int> a(10, __LINE__);
  a[10] = 12345; //Invalid index
  return 0;
} 
