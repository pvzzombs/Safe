#define DEBUG_
#include "safe.hpp"
int main(){
  arr_<arr_<int>> a(100, __LINE__);
  a.for_each([](arr_<int> &x, size_t i){
    //allocate an array  since this is 2d array of 10 by 10
    //the elements are not yet contain any allocation
    x.alloc(100, __LINE__);
  });
  
  a.for_each([](arr_<int>& x, size_t i){
    x.for_each([&i](int &y, size_t j){
      y = int(i*j);
    });
  });
  
  a.for_each([](arr_<int>& x, size_t i){
    x.print();
  });
  
  return 0;
} 
