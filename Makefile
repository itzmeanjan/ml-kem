CXX = g++
CXXFLAGS = -std=c++20 -Wall -Wextra -pedantic
OPTFLAGS = -O3 -march=native
IFLAGS = -I ./include
DEP_IFLAGS = -I ./deps/sha3/include

all: testing

deps/sha3:
	mkdir -p deps
	git clone https://github.com/itzmeanjan/sha3.git $@

dep: deps/sha3

test/a.out: test/main.cpp include/*.hpp dep
	$(CXX) $(CXXFLAGS) $(OPTFLAGS) $(IFLAGS) $(DEP_IFLAGS) $< -o $@

testing: test/a.out
	./$<

clean:
	find . -name '*.out' -o -name '*.o' -o -name '*.so' -o -name '*.gch' | xargs rm -rf
	rm -rf deps

format:
	find . -name '*.cpp' -o -name '*.hpp' | xargs clang-format -i --style=Mozilla
