//#define _CRTDBG_MAP_ALLOC

#define DEBUG_

#include <iostream>
#include "safe.hpp"

mem_heap_debug<int> gb;

int main(){
  ///_CrtSetDbgFlag ( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
  int * hello = new_<int>(1, gb);
  int * cow = new_<int>(2, gb);
  int * jerk = new_<int>(3, gb);
  int * bucks;

  del_<int>(cow, gb);
  del_<int>(cow, gb);
  del_<int>(hello, gb);
  del_<int>(hello, gb);

  hello = new_<int>(2, gb);
  del_<int>(jerk, gb);
  del_<int>(hello, gb);

  cow = new_<int>(1, gb);
  jerk = new_<int>(2, gb);
  del_<int>(jerk, gb);
  del_<int>(cow, gb);

  hello = new_<int>(1, gb);
  cow = new_<int>(2, gb);
  jerk = new_<int>(3, gb);
  bucks = new_<int>(4, gb);

  del_<int>(bucks, gb);
  del_<int>(cow, gb);
  del_<int>(hello, gb);
  del_<int>(jerk, gb);

  hello = new_<int>(1, gb);
  cow = new_<int>(2, gb);
  jerk = new_<int>(3, gb);
  bucks = new_<int>(4, gb);

  del_<int>(cow, gb);
  del_<int>(jerk, gb);
  del_<int>(hello, gb);
  del_<int>(bucks, gb);

  hello = new_<int>(1, gb);
  cow = new_<int>(2, gb);
  jerk = new_<int>(3, gb);
  bucks = new_<int>(4, gb);

  del_<int>(hello, gb);
  del_<int>(bucks, gb);
  del_<int>(jerk, gb);
  del_<int>(cow, gb);

  hello = new_<int>(1, gb);
  cow = new_<int>(2, gb);
  jerk = new_<int>(3, gb);
  bucks = new_<int>(4, gb);

  del_<int>(hello, gb);
  del_<int>(bucks, gb);
  del_<int>(cow, gb);
  del_<int>(jerk, gb);

  int * stacks[4];
  for(int i = 0; i < 4; i++){
    for(int j = 0; j < 4; j++){
      unsigned int tt = i + j;
      stacks[(tt) % 4] = new_<int>(2, gb, __LINE__);
      stacks[(tt + 1) % 4] = new_<int>(2, gb, __LINE__);
      stacks[(tt + 2) % 4] = new_<int>(2, gb, __LINE__);
      stacks[(tt + 3) % 4] = new_<int>(2, gb, __LINE__);

      //gb.free_address();

      for(int x=0; x<4; x++){
        del_<int>(stacks[x], gb, __LINE__); 
      }
    }
  }

  for(int i = 0; i < 4; i++){
    for(int j = 0; j < 4; j++){

      for(int x=0; x<4; x++){
        stacks[x] = new_<int>(2, gb, __LINE__); 
      }

      gb.free_address();

      unsigned int tt = i + j;
      del_<int>(stacks[(tt) % 4], gb, __LINE__);
      del_<int>(stacks[(tt + 1) % 4], gb, __LINE__);
      del_<int>(stacks[(tt + 2) % 4], gb, __LINE__);
      del_<int>(stacks[(tt + 3) % 4], gb, __LINE__);

    }
  }

  gb.destroy();
  //std::getchar();
  return 0;
}
