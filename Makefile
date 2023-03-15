CXX=g++
CXXFLAGS=--coverage -g -O0 -std=c++11
LDFLAGS=-lgcov

all: heap bounds demo test

heap: test/heap.cpp
	$(CXX) $(CXXFLAGS) -Iinclude test/heap.cpp -o heap $(LDFLAGS)

bounds: test/bounds.cpp
	$(CXX) $(CXXFLAGS) -Iinclude test/bounds.cpp -o bounds $(LDFLAGS)

demo: test/demo.cpp
	$(CXX) $(CXXFLAGS) -Iinclude test/demo.cpp -o demo $(LDFLAGS)

test1: test/test1.cpp
	$(CXX) $(CXXFLAGS) -Iinclude test/test1.cpp -o test1 $(LDFLAGS)
