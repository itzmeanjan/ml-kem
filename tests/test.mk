ASAN_FLAGS := -fsanitize=address -fno-omit-frame-pointer -fno-optimize-sibling-calls # From https://clang.llvm.org/docs/AddressSanitizer.html
DEBUG_ASAN_FLAGS := $(DEBUG_FLAGS) $(ASAN_FLAGS)
RELEASE_ASAN_FLAGS := -g $(RELEASE_FLAGS) $(ASAN_FLAGS)
UBSAN_FLAGS := -fsanitize=undefined -fno-omit-frame-pointer -fno-optimize-sibling-calls -fno-sanitize-recover=undefined # From https://clang.llvm.org/docs/UndefinedBehaviorSanitizer.html
DEBUG_UBSAN_FLAGS := $(DEBUG_FLAGS) $(UBSAN_FLAGS)
RELEASE_UBSAN_FLAGS := -g $(RELEASE_FLAGS) $(UBSAN_FLAGS)
FUZZ_FLAGS := $(RELEASE_FLAGS) -fsanitize=fuzzer,address,undefined -fno-sanitize-recover=undefined

TEST_ARTIFACT_DIR := $(BUILD_DIR)/test
STANDARD_TEST_DIR := $(TEST_ARTIFACT_DIR)/standard
ASAN_TEST_DIR := $(TEST_ARTIFACT_DIR)/asan
UBSAN_TEST_DIR := $(TEST_ARTIFACT_DIR)/ubsan
DEBUG_TEST_BUILD_DIR := $(STANDARD_TEST_DIR)/debug
RELEASE_TEST_BUILD_DIR := $(STANDARD_TEST_DIR)/release
DEBUG_ASAN_BUILD_DIR := $(ASAN_TEST_DIR)/debug
RELEASE_ASAN_BUILD_DIR := $(ASAN_TEST_DIR)/release
DEBUG_UBSAN_BUILD_DIR := $(UBSAN_TEST_DIR)/debug
RELEASE_UBSAN_BUILD_DIR := $(UBSAN_TEST_DIR)/release
FUZZ_BUILD_DIR := $(TEST_ARTIFACT_DIR)/fuzz

TEST_DIR := tests
vpath %.cpp $(TEST_DIR)/kat $(TEST_DIR)/prop $(TEST_DIR)/fuzz

TEST_SOURCES := $(shell find $(TEST_DIR)/kat $(TEST_DIR)/prop -name '*.cpp')
FUZZ_SOURCES := $(wildcard $(TEST_DIR)/fuzz/*.cpp)
TEST_HEADERS := $(wildcard $(TEST_DIR)/*.hpp)
RELEASE_TEST_OBJECTS := $(addprefix $(RELEASE_TEST_BUILD_DIR)/, $(notdir $(patsubst %.cpp,%.o,$(TEST_SOURCES))))
RELEASE_TEST_BINARY := $(RELEASE_TEST_BUILD_DIR)/test.out
DEBUG_TEST_OBJECTS := $(addprefix $(DEBUG_TEST_BUILD_DIR)/, $(notdir $(patsubst %.cpp,%.o,$(TEST_SOURCES))))
DEBUG_TEST_BINARY := $(DEBUG_TEST_BUILD_DIR)/test.out
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
FUZZ_BINARY := $(FUZZ_BUILD_DIR)/ml_kem_decaps

$(DEBUG_TEST_BUILD_DIR):
	mkdir -p $@

$(RELEASE_TEST_BUILD_DIR):
	mkdir -p $@

$(DEBUG_ASAN_BUILD_DIR):
	mkdir -p $@

$(RELEASE_ASAN_BUILD_DIR):
	mkdir -p $@

$(DEBUG_UBSAN_BUILD_DIR):
	mkdir -p $@

$(RELEASE_UBSAN_BUILD_DIR):
	mkdir -p $@

$(FUZZ_BUILD_DIR):
	mkdir -p $@

$(DEBUG_TEST_BUILD_DIR)/%.o: %.cpp $(DEBUG_TEST_BUILD_DIR) $(SHA3_INC_DIR) $(ASCON_INC_DIR) $(SUBTLE_INC_DIR)
	$(CXX) $(CXX_FLAGS) $(WARN_FLAGS) $(DEBUG_FLAGS) $(I_FLAGS) $(DEP_IFLAGS) -I $(TEST_DIR) -c $< -o $@

$(RELEASE_TEST_BUILD_DIR)/%.o: %.cpp $(RELEASE_TEST_BUILD_DIR) $(SHA3_INC_DIR) $(ASCON_INC_DIR) $(SUBTLE_INC_DIR)
	$(CXX) $(CXX_FLAGS) $(WARN_FLAGS) $(RELEASE_FLAGS) $(I_FLAGS) $(DEP_IFLAGS) -I $(TEST_DIR) -c $< -o $@

$(DEBUG_ASAN_BUILD_DIR)/%.o: %.cpp $(DEBUG_ASAN_BUILD_DIR) $(SHA3_INC_DIR) $(ASCON_INC_DIR) $(SUBTLE_INC_DIR)
	$(CXX) $(CXX_FLAGS) $(WARN_FLAGS) $(DEBUG_ASAN_FLAGS) $(I_FLAGS) $(DEP_IFLAGS) -I $(TEST_DIR) -c $< -o $@

$(RELEASE_ASAN_BUILD_DIR)/%.o: %.cpp $(RELEASE_ASAN_BUILD_DIR) $(SHA3_INC_DIR) $(ASCON_INC_DIR) $(SUBTLE_INC_DIR)
	$(CXX) $(CXX_FLAGS) $(WARN_FLAGS) $(RELEASE_ASAN_FLAGS) $(I_FLAGS) $(DEP_IFLAGS) -I $(TEST_DIR) -c $< -o $@

$(DEBUG_UBSAN_BUILD_DIR)/%.o: %.cpp $(DEBUG_UBSAN_BUILD_DIR) $(SHA3_INC_DIR) $(ASCON_INC_DIR) $(SUBTLE_INC_DIR)
	$(CXX) $(CXX_FLAGS) $(WARN_FLAGS) $(DEBUG_UBSAN_FLAGS) $(I_FLAGS) $(DEP_IFLAGS) -I $(TEST_DIR) -c $< -o $@

$(RELEASE_UBSAN_BUILD_DIR)/%.o: %.cpp $(RELEASE_UBSAN_BUILD_DIR) $(SHA3_INC_DIR) $(ASCON_INC_DIR) $(SUBTLE_INC_DIR)
	$(CXX) $(CXX_FLAGS) $(WARN_FLAGS) $(RELEASE_UBSAN_FLAGS) $(I_FLAGS) $(DEP_IFLAGS) -I $(TEST_DIR) -c $< -o $@

$(FUZZ_BINARY): ml_kem_decaps.cpp $(FUZZ_BUILD_DIR) $(SHA3_INC_DIR) $(ASCON_INC_DIR) $(SUBTLE_INC_DIR)
	$(CXX) $(CXX_FLAGS) $(WARN_FLAGS) $(FUZZ_FLAGS) $(I_FLAGS) $(DEP_IFLAGS) -I $(TEST_DIR) $< -o $@

$(DEBUG_TEST_BINARY): $(DEBUG_TEST_OBJECTS)
	$(CXX) $(DEBUG_FLAGS) $^ $(TEST_LINK_FLAGS) -o $@

$(RELEASE_TEST_BINARY): $(RELEASE_TEST_OBJECTS)
	$(CXX) $(RELEASE_FLAGS) $(LINK_OPT_FLAGS) $^ $(TEST_LINK_FLAGS) -o $@

$(DEBUG_ASAN_TEST_BINARY): $(DEBUG_ASAN_TEST_OBJECTS)
	$(CXX) $(DEBUG_ASAN_FLAGS) $^ $(TEST_LINK_FLAGS) -o $@

$(RELEASE_ASAN_TEST_BINARY): $(RELEASE_ASAN_TEST_OBJECTS)
	$(CXX) $(RELEASE_ASAN_FLAGS) $^ $(TEST_LINK_FLAGS) -o $@

$(DEBUG_UBSAN_TEST_BINARY): $(DEBUG_UBSAN_TEST_OBJECTS)
	$(CXX) $(DEBUG_UBSAN_FLAGS) $^ $(TEST_LINK_FLAGS) -o $@

$(RELEASE_UBSAN_TEST_BINARY): $(RELEASE_UBSAN_TEST_OBJECTS)
	$(CXX) $(RELEASE_UBSAN_FLAGS) $^ $(TEST_LINK_FLAGS) -o $@

debug_test: $(DEBUG_TEST_BINARY) $(GTEST_PARALLEL)
	$(GTEST_PARALLEL) $< --print_test_times

release_test: $(RELEASE_TEST_BINARY) $(GTEST_PARALLEL)
	$(GTEST_PARALLEL) $< --print_test_times

debug_asan_test: $(DEBUG_ASAN_TEST_BINARY) $(GTEST_PARALLEL)
	ASAN_OPTIONS=detect_leaks=1 $(GTEST_PARALLEL) $< --print_test_times

release_asan_test: $(RELEASE_ASAN_TEST_BINARY) $(GTEST_PARALLEL)
	ASAN_OPTIONS=detect_leaks=1 $(GTEST_PARALLEL) $< --print_test_times

debug_ubsan_test: $(DEBUG_UBSAN_TEST_BINARY) $(GTEST_PARALLEL)
	$(GTEST_PARALLEL) $< --print_test_times

release_ubsan_test: $(RELEASE_UBSAN_TEST_BINARY) $(GTEST_PARALLEL)
	$(GTEST_PARALLEL) $< --print_test_times

.PHONY: test
test: debug_test release_test debug_asan_test release_asan_test debug_ubsan_test release_ubsan_test ## Build and run full test matrix (Debug/Release x No-Sanitizer/ASan/UBSan)

.PHONY: fuzz
fuzz: $(FUZZ_BINARY) ## Build and run the unified decapsulation fuzzer (ML-KEM-512/768/1024)
	./$< -max_total_time=60
