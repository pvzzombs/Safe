#include <iostream>
#include <exception>
//#include <vector>
//#include <stdlib.h>
//#include <crtdbg.h>

#ifdef DEBUG_

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
      HEAD = new heap_linked_list<T>(addr_t);
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
      TAIL->RIGHT = new heap_linked_list<T>(addr_t);
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

  void destroy(){
    if(!destroyed_){
      if(HEAD_INIT){
        //we delete everything
        heap_linked_list<T> * temp_ = HEAD;
        while(!(temp_ == nullptr)){
          if( !(temp_->address_holder == nullptr) ){
#ifdef DEBUG_
            std::cout << "Found memory leak at " << temp_->address_holder << ", releasing...\n";
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

  void free_address(){
    if(HEAD_INIT){
      //we delete everything
      heap_linked_list<T> * temp_ = HEAD;
      while(!(temp_ == nullptr)){
        if( !(temp_->address_holder == nullptr) ){
#ifdef DEBUG_
          std::cout << "Found memory leak at " << temp_->address_holder << ", releasing...\n";
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
  };
  void destroy(){
  }
  void free_address(){
  }
};

#endif

class bad_memory_alloc_error_debug: public std::exception{
  virtual const char * what() const throw(){
    return "Bad Memory Allocation";
  }
} BMARD_;

class bad_memory_release_error_debug: public std::exception{
  virtual const char * what() const throw(){
    return "Dangling Memory should not be released";
  }
} BMRED_;

class bad_memory_access_error_debug: public std::exception{
  virtual const char * what() const throw(){
    return "Array index is out of bounds";
  }
} BMCRD_;

#ifdef DEBUG_
template <typename T>
T * new_(size_t size, mem_heap_debug<T> &heap_, int line=0){
  T * d = NULL;
  d = new T[size];
  if(d == NULL || size == 0 || d == nullptr){
    throw BMARD_;
  }

  bool result_ = heap_.add_address(d);

  if(result_){
    std::cout << "Allocation: " << d << " ~ Memory Allocation ~ size " << size << " at line " << line << "\n";
  }else{
    std::cout << "There is a problem allocating memory!\n";
  }

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
    std::cout << "Allocation: " << block << " ~ Memory Released ~ at line " << line << "\n";
  }else{
    std::cout << "There is a problem releasing memory!\n";
  }
}

#else
template <typename T>
T * new_(size_t size, mem_heap_debug<T> &heap_, int line=0){
  T * d = NULL;
  d = new T[size];
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

#ifdef DEBUG_

template <typename T>
class arr_{
  bool destroy;
  bool init_;
  size_t size_;
  T * data;
  arr_(const arr_<T> &) = delete;
  arr_(arr_<T> &&) = delete;
  arr_ & operator= (const arr_<T> &) = delete;
  arr_ & operator= (arr_<T> &&) = delete;
public:
  arr_(): size_(0), destroy(false), init_(false){
    data = nullptr;
  }
  arr_(size_t s, unsigned int line=0): size_(s), destroy(true), init_(true){
    data = new T[s];
    std::cout << "New array at line " << line << std::endl;
  }

  ~arr_(){
    if(destroy){
      delete [] data;
      std::cout << "Array released..." << std::endl;
    }else{
      std::cout << "Array is empty? " << std::endl;
    }
  }

  bool alloc(size_t s, unsigned int line=0){
    if(init_){
      return false;
    }
    data = new T[s];
    if(data == nullptr){
      return false;
    }else{
      size_ = s;
      destroy = true;
    }
    return true;
  }

  void print(){
    for(size_t i=0; i<size_; i++){
      std::cout << data[i] << " " << std::flush;
    }
    std::cout << std::endl;
  }

  T & at(long index){
    if(index < 0 || index > (size_-1)){
      std::cout << "Array index out of bounds : index " << index << std::endl;
      throw BMCRD_;
    }
    return data[index];
  }

  T & operator[] (long index){
    if(index < 0 || index > (size_-1)){
      std::cout << "Array index out of bounds : index " << index << std::endl;
      throw BMCRD_;
    }
    return data[index];
  }

  size_t size(){
    return size_;
  }

  size_t length(){
    return size_;
  }

  template <typename Func>
  void for_each( Func fun ){
    for(size_t i=0; i<size_; i++){
      fun(data[i], i);
    }
  }

};

#else

template <typename T>
class arr_{
  bool destroy;
  bool init_;
  size_t size_;
  T * data;
  arr_(const arr_<T> &) = delete;
  arr_(arr_<T> &&) = delete;
  arr_ & operator= (const arr_<T> &) = delete;
  arr_ & operator= (arr_<T> &&) = delete;
public:
  arr_(): size_(0), destroy(false), init_(false){
    data = nullptr;
  }
  arr_(size_t s, unsigned int line=0): size_(s), destroy(true), init_(true){
    data = new T[s];
  }

  ~arr_(){
    if(destroy){
      delete [] data;
    }
  }

  bool alloc(size_t s, unsigned int line=0){
    if(init_){
      return false;
    }
    data = new T[s];
    if(data == nullptr){
      return false;
    }else{
      size_ = s;
      destroy = true;
    }
    return true;
  }

  void print(){
    for(size_t i=0; i<size_; i++){
      std::cout << data[i] << " " << std::flush;
    }
    std::cout << std::endl;
  }

  T & at(long index){
    return data[index];
  }

  T & operator[] (long index){
    return data[index];
  }

  size_t size(){
    return size_;
  }

  size_t length(){
    return size_;
  }

  template <typename Func>
  void for_each( Func fun ){
    for(size_t i=0; i<size_; i++){
      fun(data[i], i);
    }
  }

};

#endif