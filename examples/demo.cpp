#define DEBUG_
#include "safe.hpp"

#if ((defined(_MSVC_LANG) && _MSVC_LANG >= 199711L) || __cplusplus>=199711L)
void iterateI(arr_<int> &x, size_t i){
  x.alloc(100, __LINE__);
}

void iterateK(int &y,  size_t j){
  y = int(j);
}

void iterateJ(arr_<int> &x, size_t i){
  x.for_each(iterateK);
}

void printALL(arr_<int> &x, size_t i){
  x.print();
}
#endif

int main(){
#if ((defined(_MSVC_LANG) && _MSVC_LANG >= 201103L) || __cplusplus>=201103L)

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

#elif ((defined(_MSVC_LANG) && _MSVC_LANG >= 199711L) || __cplusplus>=199711L)

  arr_< arr_<int> > a(100, __LINE__);
  a.for_each(iterateI);
  a.for_each(iterateJ);
  a.for_each(printALL);

#endif
  
  return 0;
} 
