#ifndef PVZZOMBS_SAFE_HEADER
#define PVZZOMBS_SAFE_HEADER

#include <iostream>
#include <exception>

#if ((defined(_MSVC_LANG) && _MSVC_LANG >= 201103L) || __cplusplus>=201103L)
#include <initializer_list>
#endif

#ifdef SAFE_USE_NAMESPACE
namespace safe {
#endif

#if ((defined(_MSVC_LANG) && _MSVC_LANG >= 199711L) || __cplusplus>=199711L)
#define nullptr NULL
#endif

// Constant strings
const char * m_1 = "-- Found memory leak at ";
const char * m_2 = ", releasing...\n";
const char * m_3 = "Bad Memory Allocation";
const char * m_4 = "Dangling Memory should not be released";
const char * m_5 = "Array index is out of bounds";
const char * m_6 = "-- Memory allocation: ";
const char * m_7 = "-- Memory released: ";
const char * m_8 = "-- There is a problem releasing memory!\n";
const char * m_9 = "-- There is a problem allocating memory!\n";
const char * m_10 = " at line ";
const char * m_11 = " ~ at line ";
const char * m_12 = " ~ size ";
const char * m_13 = "-- New array at line ";
const char * m_14 = "-- Array released...";
const char * m_15 = "-- Array is empty";
const char * m_16 = "-- Array index out of bounds : index ";
const char * m_17 = "Array is not initialized";
const char * m_18 = "-- Warning: An array with size 0 will be initialized with a size of 1\n";
const char * m_19 = "-- Address found at ";

#if defined(DEBUG_) || defined(SAFE_USE_FUNCTIONALITY)

template <typename T>
class heap_linked_list{
public:
  heap_linked_list * LEFT;
  heap_linked_list * RIGHT;
  T * address_holder;

  heap_linked_list(){
    LEFT = nullptr;
    RIGHT = nullptr;
    address_holder = nullptr;
  }

  heap_linked_list(T * addr_t){
    LEFT = nullptr;
    RIGHT = nullptr;
    address_holder = addr_t;
  }

};

template <typename T>
class mem_heap_debug{
private:
  heap_linked_list<T> * HEAD;
  heap_linked_list<T> * TAIL;
  bool HEAD_INIT;
  bool TAIL_INIT;
  bool destroyed_;

public:
  mem_heap_debug(){
    destroyed_ = false;
    HEAD_INIT = false;
    TAIL_INIT = false;
    HEAD = nullptr;
    TAIL = nullptr;
  }

  bool add_address(T * addr_t){
    if(!HEAD_INIT){
      //newly used heap
      HEAD = new (std::nothrow) heap_linked_list<T>(addr_t);
      HEAD_INIT = true;
      //this  should always work
      TAIL = HEAD;
    }else{
      heap_linked_list<T> * temp_ = HEAD;
      while(!(temp_ == nullptr)){
        if(temp_->address_holder == addr_t){
          return false;
        }
        if(!(temp_->RIGHT == nullptr)){
          temp_ = temp_->RIGHT;
        }else{
          break;
        }
      }

      //always work from right then left
      TAIL->RIGHT = new (std::nothrow) heap_linked_list<T>(addr_t);
      //point the tail rights to the previous tail
      //*T --> R
      //then
      //T <-- *R
      TAIL->RIGHT->LEFT = TAIL;

      //now the more fun stuff
      //T = R
      TAIL = TAIL->RIGHT;
      if(!TAIL_INIT){
        TAIL_INIT = true;
      }
    }
    return true;
  }

  bool remove_address(T * addr_t){
    if(HEAD_INIT){

      heap_linked_list<T> * temp_ = HEAD;
      bool has_left, has_right;
      while(!(temp_ == nullptr)){
        has_left = false; has_right = false;
        if(temp_->address_holder == addr_t){
          //if the current node is the one
          //that has the address we want to find:
          //check whether it has a left node
          if( !(temp_->LEFT == nullptr) )
            has_left = true;

          if( !(temp_->RIGHT == nullptr) )
            has_right = true;

          if( has_left && has_right ){
            //detach itself
            // L = M = R
            // L _ M - R
            // L _ R - M
            // L = R - M
            // L = R (x M)

            temp_->LEFT->RIGHT = temp_->RIGHT;
            temp_->RIGHT->LEFT = temp_->LEFT;
          }else if( has_right && !has_left ){
            //we need to replace the head ofcourse
            temp_->RIGHT->LEFT = nullptr;
            HEAD = temp_->RIGHT;
            //check if the new head has no longer node
            //if it is, then it means it is the tail
            //we remove the bool TAIL_INT
            if(HEAD->RIGHT == nullptr){
              TAIL = HEAD;
              TAIL_INIT = false;
            }
          }else if( has_left && !has_right ){
            temp_->LEFT->RIGHT = nullptr;
            //check if the left node of the tail
            //contains another left node
            //if nott, then there is only one node
            //existing and it is the head
            if(temp_->LEFT->LEFT == nullptr){
              TAIL = HEAD;
              TAIL_INIT = false;
            }
          }else{
            //this the head
            HEAD_INIT = false;
          }

          //note that we did not deleted the
          //allocation of addr_t, we just
          //deleted this node so we know
          //that it no longer exists
          delete temp_;
          return true;
        }
        if(!(temp_->RIGHT == nullptr)){
          temp_ = temp_->RIGHT;
        }else{
          return false;
        }
      }
    }
    return false;
  }

  bool free_address(T * addr_t){
    if(HEAD_INIT){

      heap_linked_list<T> * temp_ = HEAD;
      bool has_left, has_right;
      while(!(temp_ == nullptr)){
        has_left = false; has_right = false;
        if(temp_->address_holder == addr_t){
          //if the current node is the one
          //that has the address we want to find:
          //check whether it has a left node
          if( !(temp_->LEFT == nullptr) )
            has_left = true;

          if( !(temp_->RIGHT == nullptr) )
            has_right = true;

          if( has_left && has_right ){
            //detach itself
            // L = M = R
            // L _ M - R
            // L _ R - M
            // L = R - M
            // L = R (x M)

            temp_->LEFT->RIGHT = temp_->RIGHT;
            temp_->RIGHT->LEFT = temp_->LEFT;
          }else if( has_right && !has_left ){
            //we need to replace the head ofcourse
            temp_->RIGHT->LEFT = nullptr;
            HEAD = temp_->RIGHT;
            //check if the new head has no longer node
            //if it is, then it means it is the tail
            //we remove the bool TAIL_INT
            if(HEAD->RIGHT == nullptr){
              TAIL = HEAD;
              TAIL_INIT = false;
            }
          }else if( has_left && !has_right ){
            temp_->LEFT->RIGHT = nullptr;
            //check if the left node of the tail
            //contains another left node
            //if nott, then there is only one node
            //existing and it is the head
            if(temp_->LEFT->LEFT == nullptr){
              TAIL = HEAD;
              TAIL_INIT = false;
            }
          }else{
            //this the head
            HEAD_INIT = false;
          }

          //note that we did not deleted the
          //allocation of addr_t, we just
          //deleted this node so we know
          //that it no longer exists
          //std::cout << m_7 << (void *)temp_->address_holder << m_11 << "\n";
          delete [] temp_->address_holder;
          delete temp_;
          return true;
        }
        if(!(temp_->RIGHT == nullptr)){
          temp_ = temp_->RIGHT;
        }else{
          return false;
        }
      }
    }
    return false;
  }

  void destroy(){
    if(!destroyed_){
      if(HEAD_INIT){
        //we delete everything
        heap_linked_list<T> * temp_ = HEAD;
        while(!(temp_ == nullptr)){
          if( !(temp_->address_holder == nullptr) ){
#ifdef DEBUG_
            std::cout << m_1 << (void *)temp_->address_holder << m_2;
#endif
            delete [] temp_->address_holder;
          }
          if(!(temp_->RIGHT == nullptr)){
            temp_ = temp_->RIGHT;
            delete temp_->LEFT;
          }else{
            delete temp_;
            break;
          }
        }
      }
    }
    destroyed_ = true;
  }
  
  bool find_address(T * addr_t){
    if(HEAD_INIT){
      heap_linked_list<T> * temp_ = HEAD;
      while(!(temp_ == nullptr)){
        if(temp_->address_holder == addr_t){
          return true;
        }
        if(!(temp_->RIGHT == nullptr)){
          temp_ = temp_->RIGHT;
        }else{
          break;
        }
      }
    }
    return false;
  }

  void free_address(){
    if(HEAD_INIT){
      //we delete everything
      heap_linked_list<T> * temp_ = HEAD;
      while(!(temp_ == nullptr)){
        if( !(temp_->address_holder == nullptr) ){
#ifdef DEBUG_
          std::cout << m_1 << (void *)temp_->address_holder << m_2;
#endif
          delete [] temp_->address_holder;
        }
        if(!(temp_->RIGHT == nullptr)){
          temp_ = temp_->RIGHT;
          delete temp_->LEFT;
        }else{
          delete temp_;
          break;
        }
      }
    }
    HEAD_INIT = false;
  }

  ~mem_heap_debug(){
    if(!destroyed_){
      destroy();
    }
  }

};

#else

template <typename T>
class mem_heap_debug{
public:
  mem_heap_debug(){
  }
  bool add_address(T * addr_t){
    return false;
  }
  bool remove_address(T * addr_t){
    return false;
  }
  bool free_address(T * addr_t){
    return false;
  }
  void destroy(){
  }
  bool find_address(T * addr_t){
    return false;
  }
  void free_address(){
  }
};

#endif

class bad_memory_alloc_error_debug: public std::exception{
  virtual const char * what() const throw(){
    return m_3;
  }
} BMARD_;

class bad_memory_release_error_debug: public std::exception{
  virtual const char * what() const throw(){
    return m_4;
  }
} BMRED_;

class bad_memory_access_error_debug: public std::exception{
  virtual const char * what() const throw(){
    return m_5;
  }
} BMCRD_;

class bad_array_uninitialized_error_debug: public std::exception{
  virtual const char * what() const throw(){
    return m_17;
  }
} BAUED_;

#if defined(DEBUG_) || defined(SAFE_USE_FUNCTIONALITY)
template <typename T>
T * new_(size_t size, mem_heap_debug<T> &heap_, int line=0){
  T * d = NULL;
  d = new (std::nothrow) T[size];
  if(d == NULL || size == 0 || d == nullptr){
    throw BMARD_;
  }

  bool result_ = heap_.add_address(d);
#ifdef DEBUG_
  if(result_){
    std::cout << m_6 << (void *)d << m_12 << size << m_10 << line << "\n";
  }else{
    std::cout << m_9;
  }
#endif
  return d;
}
/// D
template <typename T>
void del_(T * block, mem_heap_debug<T> &heap_, int line=0){
  if(block == NULL || block == nullptr){
    throw BMRED_;
  }

  bool result_ = heap_.remove_address(block);

  if(result_){
    delete[] block;
#ifdef DEBUG_
    std::cout << m_7 << (void *)block << m_11 << line << "\n";
  }else{
    std::cout << m_8;
#endif
  }
}

#else
template <typename T>
T * new_(size_t size, mem_heap_debug<T> &heap_, int line=0){
  T * d = NULL;
  d = new (std::nothrow) T[size];
  if(d == NULL || size == 0 || d == nullptr){
    throw BMARD_;
  }
  return d;
}
/// D
template <typename T>
void del_(T * block, mem_heap_debug<T> &heap_, int line=0){
  if(block == NULL || block == nullptr){
    throw BMRED_;
  }
  delete[] block;
}

#endif

#if defined(DEBUG_) || defined(SAFE_USE_FUNCTIONALITY)

template <typename T>
class arr_{
  size_t size_;
  bool destroy;
  bool init_;
  T * data;

#if ((defined(_MSVC_LANG) && _MSVC_LANG >= 201103L) || __cplusplus>=201103L)
  arr_ & operator= (arr_<T> &&) = delete;
#endif
  template <class X> void swap_(X& a, X& b){
    X c(a); a=b; b=c;
  }
  void swap(arr_<T> & f, arr_<T> & l){
#if ((defined(_MSVC_LANG) && _MSVC_LANG >= 201103L) || __cplusplus>=201103L)
    std::swap(f.size_, l.size_);
    std::swap(f.destroy, l.destroy);
    std::swap(f.init_, l.init_);
    std::swap(f.data, l.data);
#else
    swap_(f.size_, l.size_);
    swap_(f.destroy, l.destroy);
    swap_(f.init_, l.init_);
    swap_(f.data, l.data);
#endif
  }

public:
  bool _assert_null_equals_zero(){
#if ((defined(_MSVC_LANG) && _MSVC_LANG >= 201103L) || __cplusplus>=201103L)
    //note that we define nullptr to be NULL
    //so were left checking with NULL == 0?
    return nullptr == 0;
#else
    int * _test_ptr_here = NULL;
    return _test_ptr_here == nullptr;
#endif
  }
  bool _try_leak(){
    T * a = nullptr;
    a = new (std::nothrow) T;
    if(a == nullptr){
      return false;
    }
    return true;
  }
  T * _get_address(){
    if(data == nullptr){
#ifdef DEBUG_
      std::cout << m_17 << std::endl;
#endif
      throw BMCRD_;
    }
    return data;
  }

  arr_(): size_(0), destroy(false), init_(false){
    data = nullptr;
  }
  arr_(size_t s, unsigned int line=0): size_(s), destroy(true), init_(true){
    if(s == 0){
#ifdef DEBUG_
      std::cout << m_18;
#endif
      s = 1;
      size_ = 1;
    }
    data = new (std::nothrow) T[s];
#ifdef DEBUG_
    std::cout << m_13 << line << std::endl;
#endif
  }
#if ((defined(_MSVC_LANG) && _MSVC_LANG >= 201103L) || __cplusplus>=201103L)
  arr_(std::initializer_list<T> arr_list): size_(arr_list.size()), destroy(true), init_(true){
    if(size_ == 0){
      destroy = false;
      init_ = false;
#ifdef DEBUG_
  std::cout << m_15 << std::endl;
#endif
    }else{
      data = new (std::nothrow) T[size_];
      size_t i=0;
      for(const T & elem : arr_list){
        data[i] = elem;
        ++i;
      }
#ifdef DEBUG_
  std::cout << m_13 << std::endl;
#endif
    }
  }
#endif
  arr_(const arr_<T> & rhs_): size_(rhs_.size()), destroy(true), init_(true){
    data = nullptr;
    if(rhs_.size() == 0){
#ifdef DEBUG_
      std::cout << m_18;
#endif
      size_ = 1;
    }
    data = new (std::nothrow) T[size_];
    for(size_t i=0; i<size_; i++){
      data[i] = rhs_.at(i);
    }
#ifdef DEBUG_
    std::cout << m_13 << std::endl;
#endif
  }
  arr_ & operator= (const arr_<T> & rhs_){
    arr_<T> t_(rhs_);
    swap(*this, t_);
    return *this;
  }
#if ((defined(_MSVC_LANG) && _MSVC_LANG >= 201103L) || __cplusplus>=201103L)
  arr_(arr_<T> && rhs_): size_(0), destroy(false), init_(false){
    data = nullptr;
    swap(*this, rhs_);
  }
#endif
  ~arr_(){
    if(destroy){
      delete [] data;
#ifdef DEBUG_
      std::cout << m_14 << std::endl;
#endif
    }
  }

  bool alloc(size_t s, unsigned int line=0){
    if(init_){
      return false;
    }
    if(s == 0){
#ifdef DEBUG_
      std::cout << m_18;
#endif
      s = 1;
      size_ = 1;
    }
    data = new (std::nothrow) T[s];
    if(data == nullptr){
      return false;
    }else{
      size_ = s;
      destroy = true;
      init_ = true;
    }
    return true;
  }

  void print(){
    for(size_t i=0; i<size_; i++){
      std::cout << data[i] << " " << std::flush;
    }
    std::cout << std::endl;
  }

  T & at(long index) const {
    if(!init_){
      throw BAUED_;
    }
    long t = (long)(size_-1);
    if(index < 0 || index > t){
#ifdef DEBUG_
      std::cout << m_16 << index << std::endl;
#endif
      throw BMCRD_;
    }
    return data[index];
  }

  T & operator[] (long index) const {
    if(!init_){
      throw BAUED_;
    }
    long t = (long)(size_-1);
    if(index < 0 || index > t){
#ifdef DEBUG_
      std::cout << m_16 << index << std::endl;
#endif
      throw BMCRD_;
    }
    return data[index];
  }

  size_t size() const {
    return size_;
  }

  size_t length() const {
    return size_;
  }

  template <typename Func>
  void for_each( Func fun ){
    for(size_t i=0; i<size_; i++){
      fun(data[i], i);
    }
  }

  bool alloc2(size_t s, size_t o, unsigned int line=0){
    bool ok_inner_arr;

    if(init_){
      return false;
    }
    if(s == 0){
#ifdef DEBUG_
      std::cout << m_18;
#endif
      s = 1;
      size_ = 1;
    }
    if(o == 0){
#ifdef DEBUG_
      std::cout << m_18;
#endif
      o = 1;
    }

    data = new (std::nothrow) T[s];
    if(data == nullptr){
      return false;
    }else{
      for(size_t i=0; i<s; i++){
        ok_inner_arr = data[i].alloc(o);
        if(!ok_inner_arr){
          delete [] data;
          data = nullptr;
          return false;
        }
      }
      size_ = s;
      destroy = true;
      init_ = true;
    }
    return true;
  }

};

#else

template <typename T>
class arr_{
  size_t size_;
  bool destroy;
  bool init_;
  T * data;

#if ((defined(_MSVC_LANG) && _MSVC_LANG >= 201103L) || __cplusplus>=201103L)
  arr_ & operator= (arr_<T> &&) = delete;
#endif
  template <class X> void swap_(X& a, X& b){
    X c(a); a=b; b=c;
  }
  void swap(arr_<T> & f, arr_<T> & l){
#if ((defined(_MSVC_LANG) && _MSVC_LANG >= 201103L) || __cplusplus>=201103L)
    std::swap(f.size_, l.size_);
    std::swap(f.destroy, l.destroy);
    std::swap(f.init_, l.init_);
    std::swap(f.data, l.data);
#else
    swap_(f.size_, l.size_);
    swap_(f.destroy, l.destroy);
    swap_(f.init_, l.init_);
    swap_(f.data, l.data);
#endif
  }

public:
  bool _assert_null_equals_zero(){
#if ((defined(_MSVC_LANG) && _MSVC_LANG >= 201103L) || __cplusplus>=201103L)
    //note that we define nullptr to be NULL
    //so were left checking with NULL == 0?
    return nullptr == 0;
#else
    int * _test_ptr_here = NULL;
    return _test_ptr_here == nullptr;
#endif
  }
  bool _try_leak(){
    T * a = nullptr;
    a = new (std::nothrow) T;
    if(a == nullptr){
      return false;
    }
    return true;
  }
  T * _get_address(){
    return data;
  }

  arr_(): size_(0), destroy(false), init_(false){
    data = nullptr;
  }
  arr_(size_t s, unsigned int line=0): size_(s), destroy(true), init_(true){
    data = new (std::nothrow) T[s];
  }
#if ((defined(_MSVC_LANG) && _MSVC_LANG >= 201103L) || __cplusplus>=201103L)
  arr_(std::initializer_list<T> arr_list): size_(arr_list.size()), destroy(true), init_(true){
    if(size_ == 0){
      destroy = false;
      init_ = false;
    }else{
      data = new (std::nothrow) T[size_];
      size_t i=0;
      for(const T & elem : arr_list){
        data[i] = elem;
        ++i;
      }
    }
  }
#endif
  arr_(const arr_<T> & rhs_): size_(rhs_.size()), destroy(true), init_(true){
    data = nullptr;
    if(rhs_.size() == 0){
      size_ = 1;
    }
    data = new (std::nothrow) T[size_];
    for(size_t i=0; i<size_; i++){
      data[i] = rhs_.at(i);
    }
  }
  arr_ & operator= (const arr_<T> & rhs_){
    arr_<T> t_(rhs_);
    swap(*this, t_);
    return *this;
  }
#if ((defined(_MSVC_LANG) && _MSVC_LANG >= 201103L) || __cplusplus>=201103L)
  arr_(arr_<T> && rhs_): size_(0), destroy(false), init_(false){
    data = nullptr;
    swap(*this, rhs_);
  }
#endif
  ~arr_(){
    if(destroy){
      delete [] data;
    }
  }

  bool alloc(size_t s, unsigned int line=0){
    if(init_){
      return false;
    }
    if(s == 0){
      s = 1;
      size_ = 1;
    }
    data = new (std::nothrow) T[s];
    if(data == nullptr){
      return false;
    }else{
      size_ = s;
      destroy = true;
      init_ = true;
    }
    return true;
  }

  void print(){
    for(size_t i=0; i<size_; i++){
      std::cout << data[i] << " " << std::flush;
    }
    std::cout << std::endl;
  }

  T & at(long index) const {
    return data[index];
  }

  T & operator[] (long index) const {
    return data[index];
  }

  size_t size() const {
    return size_;
  }

  size_t length() const {
    return size_;
  }

  template <typename Func>
  void for_each( Func fun ){
    for(size_t i=0; i<size_; i++){
      fun(data[i], i);
    }
  }

  bool alloc2(size_t s, size_t o, unsigned int line=0){
    bool ok_inner_arr;

    if(init_){
      return false;
    }
    if(s == 0){
      s = 1;
      size_ = 1;
    }
    if(o == 0){
      o = 1;
    }

    data = new (std::nothrow) T[s];
    if(data == nullptr){
      return false;
    }else{
      for(size_t i=0; i<s; i++){
        ok_inner_arr = data[i].alloc(o);
        if(!ok_inner_arr){
          delete [] data;
          data = nullptr;
          return false;
        }
      }
      size_ = s;
      destroy = true;
      init_ = true;
    }
    return true;
  }

};

#endif

#ifdef SAFE_USE_NAMESPACE
};
#endif

#endif