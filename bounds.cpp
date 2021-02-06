#include <iostream>
//#define DEBUG_
#include "safe.hpp"

void p(arr_<arr_<int>> *c){
  c->for_each([](arr_<int>& x, size_t i){
    x.for_each([](int &y, size_t j){
      std::cout << y << " " << std::flush;
    });
    std::cout << std::endl;
  });
}

int main(){
  arr_<arr_<int>> a(4, __LINE__);
  a.for_each([](arr_<int>& x, size_t i){
    x.alloc(4, __LINE__);
  });

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


  return 0;
}