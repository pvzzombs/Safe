# Safe
[![Build Status](https://img.shields.io/github/actions/workflow/status/pvzzombs/Safe/cmake.yml?branch=main)](https://github.com/pvzzombs/Safe/actions)
[![CircleCI](https://circleci.com/gh/pvzzombs/Safe.svg?style=svg)](https://circleci.com/gh/pvzzombs/Safe)
[![Total alerts](https://img.shields.io/lgtm/alerts/g/pvzzombs/Safe.svg?logo=lgtm&logoWidth=18)](https://lgtm.com/projects/g/pvzzombs/Safe/alerts/)  
My simple C++ header for detecting memory leaks and invalid array access. Supports C++03 and above.

## Get Started
### Requirements:
* Any compiler that supports ``C++ 03`` standard and above.
* Note that it is recommended to use ``C++ 11`` standard and above

### Adding safe.hpp in your project
1. Get the ``safe.hpp`` in this repository either by cloning this or downloading the zip/tar.gz file.
2. Include ``safe.hpp`` to your project
3. That's it!!!

### How to use
1. By default, ``safe.hpp`` is non-verbose, and is in Release mode.  
2. Define by adding ``#define DEBUG_`` before the ``#include "safe.hpp"``.  
3. This will throw errors and more verbose on console output. Done!

## What's Inside
This includes memory leak detector and invalid array access detector.  
Features:  
* Memory leak detector also acts as a garbage collector, so that it can release any memory leaks found.  
* Invalid array access detector detects invalid array index/indices, preventing invalid memory access.  

### Memory leak detector
A simple memory leak looks like this:
```
int main(){
  int * a = new int[10];
  return 0;
}
```  
This example will leak 40 bytes of memory, that is ``10*sizeof(int)==40`` if ``sizeof(int)==4``.  
How to detect that in this simple example?  
It looks like this:  
```
#define DEBUG_
#include "safe.hpp"
mem_heap_debug<int> gb;
int main(){
  int * a = new_<int>(10, gb);
  return 0;
}
```  
Let me explain this code example. First we define the macro ``DEBUG_``, this will allow us to catch  
the memory leak. Then we include the header file, ofcourse. Then we create a memory heap.  This  
is not a real memory heap. This only acts as a memory manager. The operating system is still  
the one who allocates memory for us. Then we replace ``new int[10]`` with ``new_<int>(10, gb)``. That's it!  
Now compile the code. This should work and you will be able to see its output like this:  
```
-- Memory allocation: 0x1d9260 ~ size 10 at line 0
-- Found memory leak at 0x1d9260, releasing...
```  
We can see the output that it log. The first line is the allocation. This is very helpful if you want  
to know where is the allocation happened.  The last line tells us that there is a leak. Simple, right?  
But there is something wrong with the first line, ``it says size 10 at line 0``, even it is line 5.  
What happened? Well that is because we did not specify where the allocation happen. We need to manually
place where the allocation happed by adding ``__LINE__`` as an argument to ``new_<int>(10, gb)``. 
Simply change:  
```
#define DEBUG_
#include "safe.hpp"
mem_heap_debug<int> gb;
int main(){
  int * a = new_<int>(10, gb, __LINE__);
  return 0;
}
```  
Compile again and done! New output will look like:  
```
-- Memory allocation: 0x1d9260 ~ size 10 at line 5
-- Found memory leak at 0x1d9260, releasing...
```  
How do I ``delete[]`` the memory manually? You can do that by ``del_<int>(a, gb)``. 
Now change your code to this:  
```
#define DEBUG_
#include "safe.hpp"
mem_heap_debug<int> gb;
int main(){
  int * a = new_<int>(10, gb, __LINE__);
  del_<int>(a, gb);
  return 0;
}
```  
Compile and run!  
```
-- Memory allocation: 0x189260 ~ size 10 at line 5
-- Memory released: 0x189260 ~ at line 0
```  
As you can see, there is no ``memory leaks`` in the output. But there is still a problem.  
The second line says ``~ at line 0`` which is wrong, but this is the default behaviour.
Releasing memory is not tracked by default, so you may want to add the macro ``__LINE__`` again.  
```
#define DEBUG_
#include "safe.hpp"
mem_heap_debug<int> gb;
int main(){
  int * a = new_<int>(10, gb, __LINE__);
  del_<int>(a, gb, __LINE__);
  return 0;
}
```  
Compile and run!!!  
```
-- Memory allocation: 0x1289260 ~ size 10 at line 5
-- Memory released: 0x1289260 ~ at line 6
```  
As you can see the problem was fixed, problem solved!
Also, here are some pointers to remember.  
1. Never mixed ``new_`` with ``new`` and ``del_`` with ``delete``.
2. Create a new ``mem_heap_debug`` for every class. 
  Let us say ``int`` and ``float``. Never mix them together in one ``mem_heap_debug``. An error is waiting for you!
3. Try to reduce the number of memory allocation as much as possible.
4. If you do not want to debug anymore, just comment the ``DEBUG_`` macro, it will be fast!
5. Remember that the project is still not perfect, it may crash if you have a lot of memory used!
  Try to use ``gb.free_address()``. This will free the memory used only if ``DEBUG_`` is defined!  
6. Never destroy the ``mem_heap_debug`` that is still used. Let the destructor do it.  
  Only use ``gb.destroy()`` on some cases where memory is critical. This will destroy and free everything it holds.  
7. Just like ``new`` and ``delete``, ``new_`` and ``del_`` throws an exception!  
  This are ``bad_memory_alloc_error_debug``  and ``bad_memory_release_error_debug``. Note that the implemetation of  
  bad_memory_release_error_debug is not correct. There is no way to tell if an address is freed or not. But it  
  detects ``NULL`` and ``nullptr``, so be careful using the raw pointers!  

### Invalid array access detector
A simple example of an invalid array access looks as follows:  
```
int main(){
  int a[10];
  a[10] = 12345; //Invalid index
  return 0;
}
```
Now to remedy this:
```
#define DEBUG_
#include "safe.hpp"
int main(){
  arr_<int> a(10, __LINE__);
  a[10] = 12345; //Invalid index
  return 0;
}
```
Compile and run, and see the output:  
```
New array at line 5
Array index out of bounds : index 10
terminate called after throwing an instance of 'bad_memory_access_error_debug'
  what():  Array index is out of bounds
```

What we did is just replaced ``int a[10]`` with ``arr_<int> a(10, __LINE__)``. Nice and easy!  

#### An example:
##### Note: Check the folder named 'test' for how to use it.

```
#define DEBUG_
#include "safe.hpp"
int main(){
  arr_<arr_<int>> a(10, __LINE__);
  a.for_each([](arr_<int> &x, size_t i){
    //allocate an array  since this is 2d array of 10 by 10
    //the elements are not yet contain any allocation
    x.alloc(10, __LINE__);
  });
  
  a.for_each([](arr_<int>& x, size_t i){
    x.for_each([&i](int &y, size_t j){
      y = int(j);
    });
  });
  
  a.for_each([](arr_<int>& x, size_t i){
    x.print();
  });
  
  return 0;
}
```


# LICENSE
MIT License

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
