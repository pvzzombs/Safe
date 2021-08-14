//#define _CRTDBG_MAP_ALLOC

#define DEBUG_

#include <iostream>
#include "safe.hpp"

mem_heap_debug<int> garbage_bin;

int main(){
  ///_CrtSetDbgFlag ( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
  int * hello = new_<int>(1, garbage_bin);
  int * cow = new_<int>(2, garbage_bin);
  int * jerk = new_<int>(3, garbage_bin);
  int * bucks;

  del_<int>(cow, garbage_bin);
  del_<int>(cow, garbage_bin);
  del_<int>(hello, garbage_bin);
  del_<int>(hello, garbage_bin);

  hello = new_<int>(2, garbage_bin);
  del_<int>(jerk, garbage_bin);
  del_<int>(hello, garbage_bin);

  cow = new_<int>(1, garbage_bin);
  jerk = new_<int>(2, garbage_bin);
  del_<int>(jerk, garbage_bin);
  del_<int>(cow, garbage_bin);

  hello = new_<int>(1, garbage_bin);
  cow = new_<int>(2, garbage_bin);
  jerk = new_<int>(3, garbage_bin);
  bucks = new_<int>(4, garbage_bin);

  del_<int>(bucks, garbage_bin);
  del_<int>(cow, garbage_bin);
  del_<int>(hello, garbage_bin);
  del_<int>(jerk, garbage_bin);

  hello = new_<int>(1, garbage_bin);
  cow = new_<int>(2, garbage_bin);
  jerk = new_<int>(3, garbage_bin);
  bucks = new_<int>(4, garbage_bin);

  del_<int>(cow, garbage_bin);
  del_<int>(jerk, garbage_bin);
  del_<int>(hello, garbage_bin);
  del_<int>(bucks, garbage_bin);

  hello = new_<int>(1, garbage_bin);
  cow = new_<int>(2, garbage_bin);
  jerk = new_<int>(3, garbage_bin);
  bucks = new_<int>(4, garbage_bin);

  del_<int>(hello, garbage_bin);
  del_<int>(bucks, garbage_bin);
  del_<int>(jerk, garbage_bin);
  del_<int>(cow, garbage_bin);

  hello = new_<int>(1, garbage_bin);
  cow = new_<int>(2, garbage_bin);
  jerk = new_<int>(3, garbage_bin);
  bucks = new_<int>(4, garbage_bin);

  del_<int>(hello, garbage_bin);
  del_<int>(bucks, garbage_bin);
  del_<int>(cow, garbage_bin);
  del_<int>(jerk, garbage_bin);

  int * stacks[4];
  for(int i = 0; i < 4; i++){
    for(int j = 0; j < 4; j++){
      unsigned int tt = i + j;
      stacks[(tt) % 4] = new_<int>(2, garbage_bin, __LINE__);
      stacks[(tt + 1) % 4] = new_<int>(2, garbage_bin, __LINE__);
      stacks[(tt + 2) % 4] = new_<int>(2, garbage_bin, __LINE__);
      stacks[(tt + 3) % 4] = new_<int>(2, garbage_bin, __LINE__);

      //garbage_bin.free_address();

      for(int x=0; x<4; x++){
        del_<int>(stacks[x], garbage_bin, __LINE__); 
      }
    }
  }

  for(int i = 0; i < 4; i++){
    for(int j = 0; j < 4; j++){

      for(int x=0; x<4; x++){
        stacks[x] = new_<int>(2, garbage_bin, __LINE__); 
      }

      garbage_bin.free_address();

      unsigned int tt = i + j;
      del_<int>(stacks[(tt) % 4], garbage_bin, __LINE__);
      del_<int>(stacks[(tt + 1) % 4], garbage_bin, __LINE__);
      del_<int>(stacks[(tt + 2) % 4], garbage_bin, __LINE__);
      del_<int>(stacks[(tt + 3) % 4], garbage_bin, __LINE__);

    }
  }

  garbage_bin.destroy();
  //std::getchar();
  return 0;
}
