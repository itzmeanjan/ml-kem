CXX = g++
CXX_FLAGS = -std=c++20
WARN_FLAGS = -Wall -Wextra -pedantic
OPT_FLAGS = -O3 -march=native -mtune=native
IFLAGS = -I ./include
DEP_IFLAGS = -I ./sha3/include -I ./subtle/include

all: test

tests/a.out: tests/main.cpp include/*.hpp include/tests/*.hpp sha3/include/*.hpp subtle/include/*.hpp
	$(CXX) $(CXX_FLAGS) $(WARN_FLAGS) $(OPT_FLAGS) $(IFLAGS) $(DEP_IFLAGS) $< -o $@

test: tests/a.out
	./$<

benchmarks/bench.out: benchmarks/main.cpp include/*.hpp sha3/include/*.hpp subtle/include/*.hpp
	# In case you haven't built google-benchmark with libPFM support.
	# More @ https://gist.github.com/itzmeanjan/05dc3e946f635d00c5e0b21aae6203a7
	$(CXX) $(CXX_FLAGS) $(WARN_FLAGS) $(OPT_FLAGS) $(IFLAGS) $(DEP_IFLAGS) $< -lbenchmark -lpthread -o $@

benchmark: benchmarks/bench.out
	./$< --benchmark_time_unit=us --benchmark_counters_tabular=true

benchmarks/perf.out: benchmarks/main.cpp include/*.hpp sha3/include/*.hpp subtle/include/*.hpp
	# In case you've built google-benchmark with libPFM support.
	# More @ https://gist.github.com/itzmeanjan/05dc3e946f635d00c5e0b21aae6203a7
	$(CXX) $(CXX_FLAGS) $(WARN_FLAGS) $(OPT_FLAGS) $(IFLAGS) $(DEP_IFLAGS) $< -lbenchmark -lpthread -lpfm -o $@

perf: benchmarks/perf.out
	./$< --benchmark_time_unit=us --benchmark_counters_tabular=true --benchmark_perf_counters=CYCLES

clean:
	find . -name '*.out' -o -name '*.o' -o -name '*.so' -o -name '*.gch' | xargs rm -rf

format:
	find . -name '*.hpp' -o -name '*.cpp' -o -name '*.hpp' | xargs clang-format -i --style=Mozilla
