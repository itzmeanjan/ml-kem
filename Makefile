DEFAULT_GOAL := help

# Collects inspiration from https://github.com/itzmeanjan/RandomShake/blob/a9cd4085a4d38d7b99ee42caadc56fc2d64ec1dc/Makefile#L1-L9
.PHONY: help
help:
	@for file in $(MAKEFILE_LIST); do \
	   grep -E '^[a-zA-Z_-]+:.*?## .*$$' $${file} | sort | awk 'BEGIN {FS = ":.*?## "}; {printf "\033[36m%-30s\033[0m %s\n", $$1, $$2}';\
	done

CXX ?= clang++
CXX_FLAGS := -std=c++20
WARN_FLAGS := -Wall -Wextra -Wpedantic -Wshadow -Wconversion -Wformat=2 -Wno-pass-failed -Werror
DEBUG_FLAGS := -O1 -g
RELEASE_FLAGS := -O3 -march=native
LINK_OPT_FLAGS := -flto

I_FLAGS := -I ./include
SHA3_INC_DIR := ./sha3/include
SUBTLE_INC_DIR := ./subtle/include
RANDOMSHAKE_INC_DIR := ./RandomShake/include
DEP_IFLAGS := -I $(SHA3_INC_DIR) -I $(SUBTLE_INC_DIR) -I $(RANDOMSHAKE_INC_DIR)

SRC_DIR := include
ML_KEM_SOURCES := $(shell find $(SRC_DIR) -name '*.hpp')
BUILD_DIR := build

include tests/test.mk
include benchmarks/bench.mk
include examples/example.mk

.PHONY: tidy
tidy: ## Run clang-tidy on ML-KEM headers
	clang-tidy include/ml_kem/*.hpp --header-filter=^include/ml_kem/.* --quiet -- $(CXX_FLAGS) $(I_FLAGS) $(DEP_IFLAGS)

.PHONY: clean
clean: ## Remove build directory
	rm -rf $(BUILD_DIR)

.PHONY: format
format: $(ML_KEM_SOURCES) $(TEST_SOURCES) $(TEST_HEADERS) $(FUZZ_SOURCES) $(BENCHMARK_SOURCES) $(BENCHMARK_HEADERS) $(EXAMPLE_SOURCES) $(EXAMPLE_HEADERS) ## Format source code
	clang-format -i $^

.PHONY: sync_submodules
sync_submodules: ## Update and initialize all git submodules
	git submodule update --init --recursive

.PHONY: sync_acvp_kats
sync_acvp_kats: ## Downloads NIST ACVP KAT vectors and updates local KATs
	cd kats/scripts && ./sync_acvp_kats.sh && cd -
