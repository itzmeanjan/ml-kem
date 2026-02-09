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
TEST_SOURCES := $(shell find $(TEST_DIR)/kat $(TEST_DIR)/prop -name '*.cpp')
FUZZ_SOURCES := $(wildcard $(TEST_DIR)/fuzz/*.cpp)
TEST_HEADERS := $(wildcard $(TEST_DIR)/*.hpp)

RELEASE_TEST_OBJECTS := $(patsubst $(TEST_DIR)/%.cpp,$(RELEASE_TEST_BUILD_DIR)/%.o,$(TEST_SOURCES))
DEBUG_TEST_OBJECTS := $(patsubst $(TEST_DIR)/%.cpp,$(DEBUG_TEST_BUILD_DIR)/%.o,$(TEST_SOURCES))
DEBUG_ASAN_TEST_OBJECTS := $(patsubst $(TEST_DIR)/%.cpp,$(DEBUG_ASAN_BUILD_DIR)/%.o,$(TEST_SOURCES))
RELEASE_ASAN_TEST_OBJECTS := $(patsubst $(TEST_DIR)/%.cpp,$(RELEASE_ASAN_BUILD_DIR)/%.o,$(TEST_SOURCES))
DEBUG_UBSAN_TEST_OBJECTS := $(patsubst $(TEST_DIR)/%.cpp,$(DEBUG_UBSAN_BUILD_DIR)/%.o,$(TEST_SOURCES))
RELEASE_UBSAN_TEST_OBJECTS := $(patsubst $(TEST_DIR)/%.cpp,$(RELEASE_UBSAN_BUILD_DIR)/%.o,$(TEST_SOURCES))

RELEASE_TEST_BINARY := $(RELEASE_TEST_BUILD_DIR)/test.out
DEBUG_TEST_BINARY := $(DEBUG_TEST_BUILD_DIR)/test.out
DEBUG_ASAN_TEST_BINARY := $(DEBUG_ASAN_BUILD_DIR)/test.out
RELEASE_ASAN_TEST_BINARY := $(RELEASE_ASAN_BUILD_DIR)/test.out
DEBUG_UBSAN_TEST_BINARY := $(DEBUG_UBSAN_BUILD_DIR)/test.out
RELEASE_UBSAN_TEST_BINARY := $(RELEASE_UBSAN_BUILD_DIR)/test.out

FUZZ_512_BINARIES := $(FUZZ_BUILD_DIR)/ml_kem_512_keygen $(FUZZ_BUILD_DIR)/ml_kem_512_encaps $(FUZZ_BUILD_DIR)/ml_kem_512_decaps
FUZZ_768_BINARIES := $(FUZZ_BUILD_DIR)/ml_kem_768_keygen $(FUZZ_BUILD_DIR)/ml_kem_768_encaps $(FUZZ_BUILD_DIR)/ml_kem_768_decaps
FUZZ_1024_BINARIES := $(FUZZ_BUILD_DIR)/ml_kem_1024_keygen $(FUZZ_BUILD_DIR)/ml_kem_1024_encaps $(FUZZ_BUILD_DIR)/ml_kem_1024_decaps
FUZZ_BINARIES := $(FUZZ_512_BINARIES) $(FUZZ_768_BINARIES) $(FUZZ_1024_BINARIES)

TEST_LINK_FLAGS := -lgtest -lgtest_main
GTEST_PARALLEL := ./gtest-parallel/gtest-parallel

$(DEBUG_TEST_BUILD_DIR)/%.o: $(TEST_DIR)/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXX_FLAGS) $(WARN_FLAGS) $(DEBUG_FLAGS) $(I_FLAGS) $(DEP_IFLAGS) -I $(TEST_DIR) -c $< -o $@

$(RELEASE_TEST_BUILD_DIR)/%.o: $(TEST_DIR)/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXX_FLAGS) $(WARN_FLAGS) $(RELEASE_FLAGS) $(I_FLAGS) $(DEP_IFLAGS) -I $(TEST_DIR) -c $< -o $@

$(DEBUG_ASAN_BUILD_DIR)/%.o: $(TEST_DIR)/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXX_FLAGS) $(WARN_FLAGS) $(DEBUG_ASAN_FLAGS) $(I_FLAGS) $(DEP_IFLAGS) -I $(TEST_DIR) -c $< -o $@

$(RELEASE_ASAN_BUILD_DIR)/%.o: $(TEST_DIR)/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXX_FLAGS) $(WARN_FLAGS) $(RELEASE_ASAN_FLAGS) $(I_FLAGS) $(DEP_IFLAGS) -I $(TEST_DIR) -c $< -o $@

$(DEBUG_UBSAN_BUILD_DIR)/%.o: $(TEST_DIR)/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXX_FLAGS) $(WARN_FLAGS) $(DEBUG_UBSAN_FLAGS) $(I_FLAGS) $(DEP_IFLAGS) -I $(TEST_DIR) -c $< -o $@

$(RELEASE_UBSAN_BUILD_DIR)/%.o: $(TEST_DIR)/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXX_FLAGS) $(WARN_FLAGS) $(RELEASE_UBSAN_FLAGS) $(I_FLAGS) $(DEP_IFLAGS) -I $(TEST_DIR) -c $< -o $@

$(FUZZ_BUILD_DIR)/ml_kem_512_%: $(TEST_DIR)/fuzz/ml_kem_%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXX_FLAGS) $(WARN_FLAGS) $(FUZZ_FLAGS) -DML_KEM_512 $(I_FLAGS) $(DEP_IFLAGS) -I $(TEST_DIR) $< -o $@

$(FUZZ_BUILD_DIR)/ml_kem_768_%: $(TEST_DIR)/fuzz/ml_kem_%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXX_FLAGS) $(WARN_FLAGS) $(FUZZ_FLAGS) -DML_KEM_768 $(I_FLAGS) $(DEP_IFLAGS) -I $(TEST_DIR) $< -o $@

$(FUZZ_BUILD_DIR)/ml_kem_1024_%: $(TEST_DIR)/fuzz/ml_kem_%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXX_FLAGS) $(WARN_FLAGS) $(FUZZ_FLAGS) -DML_KEM_1024 $(I_FLAGS) $(DEP_IFLAGS) -I $(TEST_DIR) $< -o $@

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
test: ## Build and run full test matrix (Debug/Release x No-Sanitizer/ASan/UBSan)
	$(MAKE) debug_test
	$(MAKE) release_test
	$(MAKE) debug_asan_test
	$(MAKE) release_asan_test
	$(MAKE) debug_ubsan_test
	$(MAKE) release_ubsan_test

.PHONY: fuzz
fuzz: $(FUZZ_BINARIES) ## Build and run all 9 fuzzers sequentially (60s each)
	@for binary in $(FUZZ_BINARIES); do \
		case $$binary in \
			*512_keygen)  len=64   ;; \
			*768_keygen)  len=64   ;; \
			*1024_keygen) len=64   ;; \
			*512_encaps)  len=832  ;; \
			*768_encaps)  len=1216 ;; \
			*1024_encaps) len=1600 ;; \
			*512_decaps)  len=2400 ;; \
			*768_decaps)  len=3488 ;; \
			*1024_decaps) len=4736 ;; \
		esac; \
		echo "--------------------------------------------------------------------------------"; \
		echo "Running $$binary (input size: $$len bytes)..."; \
		echo "Logs and artifacts will be stored in: $$binary.artifacts/"; \
		echo "--------------------------------------------------------------------------------"; \
		mkdir -p $$binary.artifacts/; \
		if [ ! -f $$binary.artifacts/seed ]; then \
			head -c $$len /dev/urandom > $$binary.artifacts/seed; \
		fi; \
		(cd $$binary.artifacts/ && $(CURDIR)/$$binary . -max_total_time=60 -max_len=$$len); \
	done
