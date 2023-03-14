CXX=g++
CXXFLAGS=--coverage -g -O0 -std=c++11
LDFLAGS=-lgcov

all: heap bounds demo test1

heap: test/heap.cpp
	$(CXX) $(CXXFLAGS) -o heap $(LDFLAGS)

bounds: test/bounds.cpp
	$(CXX) $(CXXFLAGS) -o bounds $(LDFLAGS)

demo: test/demo.cpp
	$(CXX) $(CXXFLAGS) -o demo $(LDFLAGS)

test1: test/test.cpp
	$(CXX) $(CXXFLAGS) -o test1 $(LDFLAGS)
