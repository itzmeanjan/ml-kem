ASAN_FLAGS := -fno-omit-frame-pointer -fno-optimize-sibling-calls -fsanitize=address # From https://clang.llvm.org/docs/AddressSanitizer.html
DEBUG_ASAN_FLAGS := $(DEBUG_FLAGS) $(ASAN_FLAGS)
RELEASE_ASAN_FLAGS := -g $(RELEASE_FLAGS) $(ASAN_FLAGS)
UBSAN_FLAGS := -fno-omit-frame-pointer -fno-optimize-sibling-calls -fsanitize=undefined # From https://clang.llvm.org/docs/UndefinedBehaviorSanitizer.html
DEBUG_UBSAN_FLAGS := $(DEBUG_FLAGS) $(UBSAN_FLAGS)
RELEASE_UBSAN_FLAGS := -g $(RELEASE_FLAGS) $(UBSAN_FLAGS)

TEST_BUILD_DIR := $(BUILD_DIR)/test
BENCHMARK_BUILD_DIR := $(BUILD_DIR)/benchmark
ASAN_BUILD_DIR := $(BUILD_DIR)/asan
DEBUG_ASAN_BUILD_DIR := $(ASAN_BUILD_DIR)/debug
RELEASE_ASAN_BUILD_DIR := $(ASAN_BUILD_DIR)/release
UBSAN_BUILD_DIR := $(BUILD_DIR)/ubsan
DEBUG_UBSAN_BUILD_DIR := $(UBSAN_BUILD_DIR)/debug
RELEASE_UBSAN_BUILD_DIR := $(UBSAN_BUILD_DIR)/release

TEST_DIR := tests
TEST_SOURCES := $(wildcard $(TEST_DIR)/*.cpp)
TEST_HEADERS := $(wildcard $(TEST_DIR)/*.hpp)
TEST_OBJECTS := $(addprefix $(TEST_BUILD_DIR)/, $(notdir $(patsubst %.cpp,%.o,$(TEST_SOURCES))))
TEST_BINARY := $(TEST_BUILD_DIR)/test.out
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

$(DEBUG_ASAN_TEST_BINARY): $(DEBUG_ASAN_TEST_OBJECTS)
	$(CXX) $(DEBUG_ASAN_FLAGS) $^ $(TEST_LINK_FLAGS) -o $@

$(RELEASE_ASAN_TEST_BINARY): $(RELEASE_ASAN_TEST_OBJECTS)
	$(CXX) $(RELEASE_ASAN_FLAGS) $^ $(TEST_LINK_FLAGS) -o $@

$(DEBUG_UBSAN_TEST_BINARY): $(DEBUG_UBSAN_TEST_OBJECTS)
	$(CXX) $(DEBUG_UBSAN_FLAGS) $^ $(TEST_LINK_FLAGS) -o $@

$(RELEASE_UBSAN_TEST_BINARY): $(RELEASE_UBSAN_TEST_OBJECTS)
	$(CXX) $(RELEASE_UBSAN_FLAGS) $^ $(TEST_LINK_FLAGS) -o $@

test: $(TEST_BINARY) $(GTEST_PARALLEL) ## Build and run all tests in RELEASE mode
	$(GTEST_PARALLEL) $< --print_test_times

debug_asan_test: $(DEBUG_ASAN_TEST_BINARY) $(GTEST_PARALLEL) ## Build and run all tests in DEBUG mode, with Address Sanitizer
	$(GTEST_PARALLEL) $< --print_test_times

release_asan_test: $(RELEASE_ASAN_TEST_BINARY) $(GTEST_PARALLEL) ## Build and run all tests in RELEASE mode, with Address Sanitizer
	$(GTEST_PARALLEL) $< --print_test_times

debug_ubsan_test: $(DEBUG_UBSAN_TEST_BINARY) $(GTEST_PARALLEL) ## Build and run all tests in DEBUG mode, with Undefined Behavior Sanitizer
	$(GTEST_PARALLEL) $< --print_test_times

release_ubsan_test: $(RELEASE_UBSAN_TEST_BINARY) $(GTEST_PARALLEL) ## Build and run all tests in RELEASE mode, with Undefined Behavior Sanitizer
	$(GTEST_PARALLEL) $< --print_test_times
