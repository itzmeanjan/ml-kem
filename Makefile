CXX ?= clang++
CXX_FLAGS := -std=c++20
WARN_FLAGS := -Wall -Wextra -Wpedantic
DEBUG_FLAGS := -O1 -g
RELEASE_FLAGS := -O3 -march=native
LINK_OPT_FLAGS := -flto
ASAN_FLAGS := -fno-omit-frame-pointer -fno-optimize-sibling-calls -fsanitize=address # From https://clang.llvm.org/docs/AddressSanitizer.html
DEBUG_ASAN_FLAGS := $(DEBUG_FLAGS) $(ASAN_FLAGS)
RELEASE_ASAN_FLAGS := -g $(RELEASE_FLAGS) $(ASAN_FLAGS)
UBSAN_FLAGS := -fno-omit-frame-pointer -fno-optimize-sibling-calls -fsanitize=undefined # From https://clang.llvm.org/docs/UndefinedBehaviorSanitizer.html
DEBUG_UBSAN_FLAGS := $(DEBUG_FLAGS) $(UBSAN_FLAGS)
RELEASE_UBSAN_FLAGS := -g $(RELEASE_FLAGS) $(UBSAN_FLAGS)

I_FLAGS := -I ./include
SHA3_INC_DIR = ./sha3/include
SUBTLE_INC_DIR = ./subtle/include
DUDECT_INC_DIR = ./dudect/src
DEP_IFLAGS = -I $(SHA3_INC_DIR) -I $(SUBTLE_INC_DIR)
DUDECT_DEP_IFLAGS = $(DEP_IFLAGS) -I $(DUDECT_INC_DIR)

SRC_DIR := include
ML_KEM_SOURCES := $(shell find $(SRC_DIR) -name '*.hpp')
BUILD_DIR := build
DUDECT_BUILD_DIR := $(BUILD_DIR)/dudect
TEST_BUILD_DIR := $(BUILD_DIR)/test
BENCHMARK_BUILD_DIR := $(BUILD_DIR)/benchmark
ASAN_BUILD_DIR := $(BUILD_DIR)/asan
DEBUG_ASAN_BUILD_DIR := $(ASAN_BUILD_DIR)/debug
RELEASE_ASAN_BUILD_DIR := $(ASAN_BUILD_DIR)/release
UBSAN_BUILD_DIR := $(BUILD_DIR)/ubsan
DEBUG_UBSAN_BUILD_DIR := $(UBSAN_BUILD_DIR)/debug
RELEASE_UBSAN_BUILD_DIR := $(UBSAN_BUILD_DIR)/release

TEST_DIR := tests
DUDECT_TEST_DIR = $(TEST_DIR)/dudect
TEST_SOURCES := $(wildcard $(TEST_DIR)/*.cpp)
TEST_HEADERS := $(wildcard $(TEST_DIR)/*.hpp)
DUDECT_TEST_SOURCES := $(wildcard $(DUDECT_TEST_DIR)/*.cpp)
TEST_OBJECTS := $(addprefix $(TEST_BUILD_DIR)/, $(notdir $(patsubst %.cpp,%.o,$(TEST_SOURCES))))
TEST_BINARY := $(TEST_BUILD_DIR)/test.out
DUDECT_TEST_BINARIES := $(addprefix $(DUDECT_BUILD_DIR)/, $(notdir $(patsubst %.cpp,%.out,$(DUDECT_TEST_SOURCES))))
TEST_LINK_FLAGS := -lgtest -lgtest_main
GTEST_PARALLEL := ./gtest-parallel/gtest-parallel
DEBUG_ASAN_TEST_OBJECTS := $(addprefix $(DEBUG_ASAN_BUILD_DIR)/, $(notdir $(patsubst %.cpp,%.o,$(TEST_SOURCES))))
RELEASE_ASAN_TEST_OBJECTS := $(addprefix $(RELEASE_ASAN_BUILD_DIR)/, $(notdir $(patsubst %.cpp,%.o,$(TEST_SOURCES))))
DEBUG_ASAN_TEST_BINARY := $(DEBUG_ASAN_BUILD_DIR)/test.out
RELEASE_ASAN_TEST_BINARY := $(RELEASE_ASAN_BUILD_DIR)/test.out
DEBUG_UBSAN_TEST_OBJECTS := $(addprefix $(DEBUG_UBSAN_BUILD_DIR)/, $(notdir $(patsubst %.cpp,%.o,$(TEST_SOURCES))))
RELEASE_UBSAN_TEST_OBJECTS := $(addprefix $(RELEASE_UBSAN_BUILD_DIR)/, $(notdir $(patsubst %.cpp,%.o,$(TEST_SOURCES))))
DEBUG_UBSAN_TEST_BINARY := $(DEBUG_UBSAN_BUILD_DIR)/test.out
RELEASE_UBSAN_TEST_BINARY := $(RELEASE_UBSAN_BUILD_DIR)/test.out

BENCHMARK_DIR := benchmarks
BENCHMARK_SOURCES := $(wildcard $(BENCHMARK_DIR)/*.cpp)
BENCHMARK_HEADERS := $(wildcard $(BENCHMARK_DIR)/*.hpp)
BENCHMARK_OBJECTS := $(addprefix $(BENCHMARK_BUILD_DIR)/, $(notdir $(patsubst %.cpp,%.o,$(BENCHMARK_SOURCES))))
BENCHMARK_LINK_FLAGS := -lbenchmark -lbenchmark_main -lpthread
BENCHMARK_BINARY := $(BENCHMARK_BUILD_DIR)/bench.out
PERF_LINK_FLAGS := -lbenchmark -lbenchmark_main -lpfm -lpthread
PERF_BINARY := $(BENCHMARK_BUILD_DIR)/perf.out
BENCHMARK_OUT_FILE := bench_result_on_$(shell uname -s)_$(shell uname -r)_$(shell uname -m)_with_$(CXX)_$(shell $(CXX) -dumpversion).json

all: test

$(DUDECT_BUILD_DIR):
	mkdir -p $@

$(DEBUG_ASAN_BUILD_DIR):
	mkdir -p $@

$(RELEASE_ASAN_BUILD_DIR):
	mkdir -p $@

$(DEBUG_UBSAN_BUILD_DIR):
	mkdir -p $@

$(RELEASE_UBSAN_BUILD_DIR):
	mkdir -p $@

$(TEST_BUILD_DIR):
	mkdir -p $@

$(BENCHMARK_BUILD_DIR):
	mkdir -p $@

$(DUDECT_INC_DIR):
	git submodule update --init dudect

$(SUBTLE_INC_DIR): $(DUDECT_INC_DIR)
	git submodule update --init subtle

$(SHA3_INC_DIR): $(SUBTLE_INC_DIR)
	git submodule update --init sha3

$(GTEST_PARALLEL): $(SHA3_INC_DIR)
	git submodule update --init gtest-parallel

$(TEST_BUILD_DIR)/%.o: $(TEST_DIR)/%.cpp $(TEST_BUILD_DIR) $(SHA3_INC_DIR) $(ASCON_INC_DIR) $(SUBTLE_INC_DIR)
	$(CXX) $(CXX_FLAGS) $(WARN_FLAGS) $(RELEASE_FLAGS) $(I_FLAGS) $(DEP_IFLAGS) -c $< -o $@

$(DEBUG_ASAN_BUILD_DIR)/%.o: $(TEST_DIR)/%.cpp $(DEBUG_ASAN_BUILD_DIR) $(SHA3_INC_DIR) $(ASCON_INC_DIR) $(SUBTLE_INC_DIR)
	$(CXX) $(CXX_FLAGS) $(WARN_FLAGS) $(DEBUG_ASAN_FLAGS) $(I_FLAGS) $(DEP_IFLAGS) -c $< -o $@

$(RELEASE_ASAN_BUILD_DIR)/%.o: $(TEST_DIR)/%.cpp $(RELEASE_ASAN_BUILD_DIR) $(SHA3_INC_DIR) $(ASCON_INC_DIR) $(SUBTLE_INC_DIR)
	$(CXX) $(CXX_FLAGS) $(WARN_FLAGS) $(RELEASE_ASAN_FLAGS) $(I_FLAGS) $(DEP_IFLAGS) -c $< -o $@

$(DEBUG_UBSAN_BUILD_DIR)/%.o: $(TEST_DIR)/%.cpp $(DEBUG_UBSAN_BUILD_DIR) $(SHA3_INC_DIR) $(ASCON_INC_DIR) $(SUBTLE_INC_DIR)
	$(CXX) $(CXX_FLAGS) $(WARN_FLAGS) $(DEBUG_UBSAN_FLAGS) $(I_FLAGS) $(DEP_IFLAGS) -c $< -o $@

$(RELEASE_UBSAN_BUILD_DIR)/%.o: $(TEST_DIR)/%.cpp $(RELEASE_UBSAN_BUILD_DIR) $(SHA3_INC_DIR) $(ASCON_INC_DIR) $(SUBTLE_INC_DIR)
	$(CXX) $(CXX_FLAGS) $(WARN_FLAGS) $(RELEASE_UBSAN_FLAGS) $(I_FLAGS) $(DEP_IFLAGS) -c $< -o $@

$(TEST_BINARY): $(TEST_OBJECTS)
	$(CXX) $(RELEASE_FLAGS) $(LINK_OPT_FLAGS) $^ $(TEST_LINK_FLAGS) -o $@

$(DUDECT_BUILD_DIR)/%.out: $(DUDECT_TEST_DIR)/%.cpp $(DUDECT_BUILD_DIR) $(SHA3_INC_DIR) $(SUBTLE_INC_DIR) $(DUDECT_INC_DIR)
	$(CXX) $(CXX_FLAGS) $(WARN_FLAGS) $(OPT_FLAGS) $(I_FLAGS) $(DUDECT_DEP_IFLAGS) -lm $(LINK_FLAGS) $< -o $@

$(DEBUG_ASAN_TEST_BINARY): $(DEBUG_ASAN_TEST_OBJECTS)
	$(CXX) $(DEBUG_ASAN_FLAGS) $^ $(TEST_LINK_FLAGS) -o $@

$(RELEASE_ASAN_TEST_BINARY): $(RELEASE_ASAN_TEST_OBJECTS)
	$(CXX) $(RELEASE_ASAN_FLAGS) $^ $(TEST_LINK_FLAGS) -o $@

$(DEBUG_UBSAN_TEST_BINARY): $(DEBUG_UBSAN_TEST_OBJECTS)
	$(CXX) $(DEBUG_UBSAN_FLAGS) $^ $(TEST_LINK_FLAGS) -o $@

$(RELEASE_UBSAN_TEST_BINARY): $(RELEASE_UBSAN_TEST_OBJECTS)
	$(CXX) $(RELEASE_UBSAN_FLAGS) $^ $(TEST_LINK_FLAGS) -o $@

test: $(TEST_BINARY) $(GTEST_PARALLEL)
	$(GTEST_PARALLEL) $< --print_test_times

dudect_test_build: $(DUDECT_TEST_BINARIES)

debug_asan_test: $(DEBUG_ASAN_TEST_BINARY) $(GTEST_PARALLEL)
	$(GTEST_PARALLEL) $< --print_test_times

release_asan_test: $(RELEASE_ASAN_TEST_BINARY) $(GTEST_PARALLEL)
	$(GTEST_PARALLEL) $< --print_test_times

debug_ubsan_test: $(DEBUG_UBSAN_TEST_BINARY) $(GTEST_PARALLEL)
	$(GTEST_PARALLEL) $< --print_test_times

release_ubsan_test: $(RELEASE_UBSAN_TEST_BINARY) $(GTEST_PARALLEL)
	$(GTEST_PARALLEL) $< --print_test_times

$(BENCHMARK_BUILD_DIR)/%.o: $(BENCHMARK_DIR)/%.cpp $(BENCHMARK_BUILD_DIR) $(SHA3_INC_DIR) $(ASCON_INC_DIR) $(SUBTLE_INC_DIR)
	$(CXX) $(CXX_FLAGS) $(WARN_FLAGS) $(RELEASE_FLAGS) $(I_FLAGS) $(DEP_IFLAGS) -c $< -o $@

$(BENCHMARK_BINARY): $(BENCHMARK_OBJECTS)
	$(CXX) $(RELEASE_FLAGS) $(LINK_OPT_FLAGS) $^ $(BENCHMARK_LINK_FLAGS) -o $@

benchmark: $(BENCHMARK_BINARY)
	# Must *not* build google-benchmark with libPFM
	./$< --benchmark_time_unit=us --benchmark_min_warmup_time=.5 --benchmark_enable_random_interleaving=true --benchmark_repetitions=10 --benchmark_min_time=0.1s --benchmark_display_aggregates_only=true --benchmark_counters_tabular=true

$(PERF_BINARY): $(BENCHMARK_OBJECTS)
	$(CXX) $(RELEASE_FLAGS) $(LINK_OPT_FLAGS) $^ $(PERF_LINK_FLAGS) -o $@

perf: $(PERF_BINARY)
	# Must build google-benchmark with libPFM, follow https://gist.github.com/itzmeanjan/05dc3e946f635d00c5e0b21aae6203a7
	./$< --benchmark_time_unit=us --benchmark_min_warmup_time=.5 --benchmark_enable_random_interleaving=true --benchmark_repetitions=10 --benchmark_min_time=0.1s --benchmark_display_aggregates_only=true --benchmark_counters_tabular=true --benchmark_perf_counters=CYCLES

.PHONY: format clean

clean:
	rm -rf $(BUILD_DIR)

format: $(ML_KEM_SOURCES) $(TEST_SOURCES) $(TEST_HEADERS) $(DUDECT_TEST_SOURCES) $(BENCHMARK_SOURCES) $(BENCHMARK_HEADERS)
	clang-format -i $^
