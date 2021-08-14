#include <iostream>
#define DEBUG_
#include "safe.hpp"
#if ((defined(_MSVC_LANG) && _MSVC_LANG >= 201103L) || __cplusplus>=201103L)

void p(arr_<arr_<int>> *c){
  c->for_each([](arr_<int>& x, size_t i){
    x.for_each([](int &y, size_t j){
      std::cout << y << " " << std::flush;
    });
    std::cout << std::endl;
  });
}

#elif ((defined(_MSVC_LANG) && _MSVC_LANG >= 199711L) || __cplusplus>=199711L)

void p(int &x, size_t i){
  x = int(i) + 1;
} 

#endif

int main(){
#if ((defined(_MSVC_LANG) && _MSVC_LANG >= 201103L) || __cplusplus>=201103L)
  arr_<arr_<int>> a;
  a.alloc2(4, 4, __LINE__);

  a.for_each([](arr_<int>& x, size_t i){
    x.for_each([](int &y, size_t j){
      y = 1;
    });
  });

  p(&a);

  a.for_each([](arr_<int>& x, size_t i){
    x.for_each([](int &y, size_t j){
      std::cout << y << " " << std::flush;
    });
    std::cout << std::endl;
  });

  bool r = a.alloc(10, __LINE__);
  std::cout << a.size() << " " << r << std::endl;

#elif ((defined(_MSVC_LANG) && _MSVC_LANG >= 199711L) || __cplusplus>=199711L)

  arr_<int> a(4, __LINE__);
  a.print();
  a.for_each(p);
  a.print();

  bool r = a.alloc(10, __LINE__);
  std::cout << a.size() << " " << r << std::endl;

#endif
  return 0;
}