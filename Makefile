CXX = clang++
CXXFLAGS = -std=c++20 -Wall -Wextra -pedantic
OPTFLAGS = -O3 -march=native
IFLAGS = -I ./include
DEP_IFLAGS = -I ./sha3/include

all: testing

test/a.out: test/main.cpp include/*.hpp sha3/include/*.hpp
	$(CXX) $(CXXFLAGS) $(OPTFLAGS) $(IFLAGS) $(DEP_IFLAGS) $< -o $@

testing: test/a.out
	./$<

clean:
	find . -name '*.out' -o -name '*.o' -o -name '*.so' -o -name '*.gch' | xargs rm -rf
	rm -rf deps

format:
	find . -name '*.cpp' -o -name '*.hpp' | xargs clang-format -i --style=Mozilla

bench/a.out: bench/main.cpp include/*.hpp sha3/include/*.hpp
	# make sure you've google-benchmark globally installed;
	# see https://github.com/google/benchmark/tree/60b16f1#installation
	$(CXX) $(CXXFLAGS) $(OPTFLAGS) $(IFLAGS) $(DEP_IFLAGS) $< -lbenchmark -o $@

benchmark: bench/a.out
	./$<
