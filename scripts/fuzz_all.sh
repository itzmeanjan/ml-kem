#!/usr/bin/env bash
# =============================================================================
# fuzz_all.sh — Full-lifecycle fuzzing script for ML-KEM
#
# Configures, builds, generates seed corpus, runs all 14 fuzzers in parallel,
# and produces a summary report.
#
# Usage:
#   ./scripts/fuzz_all.sh                      # 1-hour run (default)
#   FUZZ_TIME=120 ./scripts/fuzz_all.sh        # 2-minute quick run
#   FUZZ_JOBS=4 ./scripts/fuzz_all.sh          # Limit to 4 parallel fuzzers
#   FUZZ_FORK=2 ./scripts/fuzz_all.sh          # 2 fork workers per fuzzer
# =============================================================================
set -euo pipefail

# ---------------------------------------------------------------------------
# Configuration
# ---------------------------------------------------------------------------
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
PROJECT_ROOT="$(cd "$SCRIPT_DIR/.." && pwd)"
BUILD_DIR="${BUILD_DIR:-$PROJECT_ROOT/build}"
CORPUS_DIR="${CORPUS_DIR:-$PROJECT_ROOT/fuzz_corpus}"
REPORT_DIR="${REPORT_DIR:-$PROJECT_ROOT/fuzz_report}"
NPROC=$(nproc)
FUZZ_TIME="${FUZZ_TIME:-3600}"
FUZZ_JOBS="${FUZZ_JOBS:-4}"
DEFAULT_FORK=$(( (NPROC / FUZZ_JOBS) - 1 ))
FUZZ_FORK="${FUZZ_FORK:-$(( DEFAULT_FORK > 0 ? DEFAULT_FORK : 1 ))}"
CXX="${CXX:-clang++}"

# Color output helpers
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
CYAN='\033[0;36m'
BOLD='\033[1m'
NC='\033[0m'

info()  { echo -e "${CYAN}[INFO]${NC}  $*"; }
ok()    { echo -e "${GREEN}[  OK]${NC}  $*"; }
warn()  { echo -e "${YELLOW}[WARN]${NC}  $*"; }
fail()  { echo -e "${RED}[FAIL]${NC}  $*"; }
header(){ echo -e "\n${BOLD}━━━ $* ━━━${NC}"; }

# ---------------------------------------------------------------------------
# ML-KEM Sizes (from FIPS 203, Table 2)
#
# pkey(k)   = k * 384 + 32
# skey(k)   = k * 384 + pkey(k) + 64
# ct(k,du,dv) = 32 * (k * du + dv)
#
# All seeds (d, z, m) = 32 bytes each
# ---------------------------------------------------------------------------
declare -A FUZZER_MAX_LEN
declare -A FUZZER_BINARY

# --- Component fuzzers (parameter-set independent) ---
FUZZER_MAX_LEN[field_arithmetic]=4        # 2 x uint16
FUZZER_MAX_LEN[poly_ntt]=512              # 256 x uint16
FUZZER_MAX_LEN[poly_serialize]=513        # 1B selector + 512
FUZZER_MAX_LEN[poly_compression]=3        # 1B selector + 2
FUZZER_MAX_LEN[poly_sampling]=193         # 1B selector + max(128, 192)

# --- ML-KEM-512 (k=2, du=10, dv=4) ---
# pkey=800, skey=1632, ct=768
FUZZER_MAX_LEN[ml_kem_512_keygen]=64      # seed_d(32) + seed_z(32)
FUZZER_MAX_LEN[ml_kem_512_encaps]=832     # Mode A: seed_m(32) + pkey(800)
FUZZER_MAX_LEN[ml_kem_512_decaps]=2400    # Mode A: skey(1632) + ct(768)

# --- ML-KEM-768 (k=3, du=10, dv=4) ---
# pkey=1184, skey=2400, ct=1088
FUZZER_MAX_LEN[ml_kem_768_keygen]=64
FUZZER_MAX_LEN[ml_kem_768_encaps]=1216    # Mode A: seed_m(32) + pkey(1184)
FUZZER_MAX_LEN[ml_kem_768_decaps]=3488    # Mode A: skey(2400) + ct(1088)

# --- ML-KEM-1024 (k=4, du=11, dv=5) ---
# pkey=1568, skey=3168, ct=1568
FUZZER_MAX_LEN[ml_kem_1024_keygen]=64
FUZZER_MAX_LEN[ml_kem_1024_encaps]=1600   # Mode A: seed_m(32) + pkey(1568)
FUZZER_MAX_LEN[ml_kem_1024_decaps]=4736   # Mode A: skey(3168) + ct(1568)

# All fuzzer names (in execution order)
ALL_FUZZERS=(
  # Component fuzzers first (fast, small inputs)
  field_arithmetic
  poly_ntt
  poly_serialize
  poly_compression
  poly_sampling
  # KEM fuzzers (by parameter set)
  ml_kem_512_keygen ml_kem_512_encaps ml_kem_512_decaps
  ml_kem_768_keygen ml_kem_768_encaps ml_kem_768_decaps
  ml_kem_1024_keygen ml_kem_1024_encaps ml_kem_1024_decaps
)

# ---------------------------------------------------------------------------
# Phase 1: Configure
# ---------------------------------------------------------------------------
phase_configure() {
  header "Phase 1/5: Configure"
  info "Configuring CMake with clang++ and fuzzer sanitizers..."
  cmake -B "$BUILD_DIR" \
    -DCMAKE_CXX_COMPILER="$CXX" \
    -DCMAKE_BUILD_TYPE=Release \
    -DML_KEM_BUILD_FUZZERS=ON \
    "$PROJECT_ROOT"
  ok "CMake configured."
}

# ---------------------------------------------------------------------------
# Phase 2: Build
# ---------------------------------------------------------------------------
phase_build() {
  header "Phase 2/5: Build"
  info "Building all 14 fuzzer binaries..."
  cmake --build "$BUILD_DIR" -j"$(nproc)"
  ok "All fuzzers built."

  # Verify all binaries exist
  local missing=0
  for name in "${ALL_FUZZERS[@]}"; do
    local bin="$BUILD_DIR/${name}_fuzzer"
    if [[ ! -x "$bin" ]]; then
      fail "Missing binary: $bin"
      missing=$((missing + 1))
    fi
  done
  if [[ $missing -gt 0 ]]; then
    fail "$missing fuzzer binaries are missing. Aborting."
    exit 1
  fi
  ok "All 14 fuzzer binaries verified."
}

# ---------------------------------------------------------------------------
# Phase 3: Generate Seed Corpus
# ---------------------------------------------------------------------------
generate_seeds_for_fuzzer() {
  local name="$1"
  local size="${FUZZER_MAX_LEN[$name]}"
  local corpus_path="$CORPUS_DIR/$name"

  mkdir -p "$corpus_path"

  # Generate 4 diverse seed files per fuzzer
  # Seed 1 & 2: random bytes (covers Mode A — odd first byte)
  for i in 1 2; do
    local seed_file="$corpus_path/seed_modeA_${i}"
    head -c "$size" /dev/urandom > "$seed_file"
    # Ensure first byte is odd (Mode A discriminator for encaps/decaps fuzzers)
    printf '\x01' | dd of="$seed_file" bs=1 count=1 conv=notrunc 2>/dev/null
  done

  # Seed 3 & 4: random bytes (covers Mode B — even first byte)
  for i in 1 2; do
    local seed_file="$corpus_path/seed_modeB_${i}"
    head -c "$size" /dev/urandom > "$seed_file"
    # Ensure first byte is even (Mode B discriminator)
    printf '\x00' | dd of="$seed_file" bs=1 count=1 conv=notrunc 2>/dev/null
  done

  # Seed 5: all zeros (edge case)
  dd if=/dev/zero of="$corpus_path/seed_zeros" bs=1 count="$size" 2>/dev/null

  # Seed 6: all 0xFF (edge case)
  dd if=/dev/zero bs=1 count="$size" 2>/dev/null | tr '\0' '\377' > "$corpus_path/seed_ones"
}

phase_seed_corpus() {
  header "Phase 3/5: Generate Seed Corpus"
  info "Creating per-fuzzer seed corpus in $CORPUS_DIR ..."

  for name in "${ALL_FUZZERS[@]}"; do
    generate_seeds_for_fuzzer "$name"
  done

  # Summary
  local total_seeds=0
  for name in "${ALL_FUZZERS[@]}"; do
    local count
    count=$(find "$CORPUS_DIR/$name" -type f | wc -l)
    total_seeds=$((total_seeds + count))
  done
  ok "Generated $total_seeds seed files across ${#ALL_FUZZERS[@]} corpus directories."

  # Print table
  printf "  %-30s %8s %6s\n" "Fuzzer" "MaxLen" "Seeds"
  printf "  %-30s %8s %6s\n" "------------------------------" "--------" "------"
  for name in "${ALL_FUZZERS[@]}"; do
    local count
    count=$(find "$CORPUS_DIR/$name" -type f | wc -l)
    printf "  %-30s %8d %6d\n" "$name" "${FUZZER_MAX_LEN[$name]}" "$count"
  done
}

# ---------------------------------------------------------------------------
# Phase 4: Run Fuzzers
# ---------------------------------------------------------------------------
phase_fuzz() {
  header "Phase 4/5: Fuzz (${FUZZ_TIME}s per fuzzer, ${FUZZ_JOBS} parallel, ${FUZZ_FORK} fork workers)"

  mkdir -p "$REPORT_DIR"
  local pids=()
  local running=0

  local i=0
  for name in "${ALL_FUZZERS[@]}"; do
    local bin="$BUILD_DIR/${name}_fuzzer"
    local corpus_path="$CORPUS_DIR/$name"
    local max_len="${FUZZER_MAX_LEN[$name]}"
    local log_file="$REPORT_DIR/${name}.log"

    info "Launching: $name (max_len=$max_len)"

    "$bin" \
      "$corpus_path" \
      -max_total_time="$FUZZ_TIME" \
      -max_len="$max_len" \
      -fork="$FUZZ_FORK" \
      -print_final_stats=1 \
      -print_corpus_stats=1 \
      > "$log_file" 2>&1 &

    pids+=($!)
    i=$((i + 1))

    # Batch Throttle: wait for the entire current batch to finish
    if [[ $(( i % FUZZ_JOBS )) -eq 0 ]]; then
      info "Waiting for current batch ($FUZZ_JOBS fuzzers) to finish..."
      for pid in "${pids[@]}"; do
        if ! wait "$pid"; then
          fail "A fuzzer in the current batch failed (potential bug found!)."
          fail "Review logs in $REPORT_DIR and artifacts in $CORPUS_DIR"
          exit 1
        fi
      done
      pids=() # Clear PIDs after successful batch wait
      ok "Batch completed."
    fi
  done

  # Wait for any remaining fuzzers (last partial batch)
  if [[ ${#pids[@]} -gt 0 ]]; then
    info "Waiting for remaining ${#pids[@]} fuzzer(s) to finish..."
    for pid in "${pids[@]}"; do
      if ! wait "$pid"; then
        fail "Fuzzer process failed. Aborting."
        exit 1
      fi
    done
  fi
  ok "All fuzzers completed."
}

# ---------------------------------------------------------------------------
# Phase 5: Report
# ---------------------------------------------------------------------------
phase_report() {
  header "Phase 5/5: Fuzzing Report"

  local report_file="$REPORT_DIR/summary.txt"
  local timestamp
  timestamp=$(date -u '+%Y-%m-%d %H:%M:%S UTC')

  {
    echo "============================================================"
    echo "  ML-KEM Fuzzing Report"
    echo "  Generated: $timestamp"
    echo "  Duration:  ${FUZZ_TIME}s per fuzzer"
    echo "  Workers:   ${FUZZ_FORK} fork workers per fuzzer"
    echo "  Parallel:  ${FUZZ_JOBS} concurrent fuzzers"
    echo "============================================================"
    echo ""
    printf "%-30s %8s %8s %10s %8s %s\n" "Fuzzer" "MaxLen" "Corpus" "Execs" "Cov" "Status"
    printf "%-30s %8s %8s %10s %8s %s\n" "------------------------------" "--------" "--------" "----------" "--------" "------"

    for name in "${ALL_FUZZERS[@]}"; do
      local corpus_path="$CORPUS_DIR/$name"
      local log_file="$REPORT_DIR/${name}.log"
      local max_len="${FUZZER_MAX_LEN[$name]}"

      # Count corpus files
      local corpus_count=0
      if [[ -d "$corpus_path" ]]; then
        corpus_count=$(find "$corpus_path" -type f | wc -l)
      fi

      # Extract total executions and coverage from log
      local execs="N/A"
      local cov="N/A"
      if [[ -f "$log_file" ]]; then
        # Try fork mode summary first (BINGO lines)
        local fork_execs
        fork_execs=$(grep -a 'BINGO TOTAL' "$log_file" | grep -oP 'RUNS:\s*\K[0-9]+' | tail -1) || true
        if [[ -n "$fork_execs" ]]; then
          execs="$fork_execs"
        else
          # Fallback 1: look for #[number]: status lines from coordinator
          execs=$(grep -a '^#[0-9]' "$log_file" | tail -1 | grep -oP '^#\K[0-9]+') || true
          if [[ -z "$execs" ]]; then
            # Fallback 2: look for stat::number_of_executed_units
            execs=$(grep -a 'stat::number_of_executed_units:' "$log_file" | awk '{print $NF}' | tail -1) || true
            [[ -z "$execs" ]] && execs="N/A"
          fi
        fi

        # Extract Coverage
        # Fork mode coordinator might log: BINGO TOTAL ... COV: 1234
        local fork_cov
        fork_cov=$(grep -a 'BINGO TOTAL' "$log_file" | grep -oP 'COV:\s*\K[0-9]+' | tail -1) || true
        if [[ -n "$fork_cov" ]]; then
          cov="$fork_cov"
        else
          # Try TOTAL_COV: line
          fork_cov=$(grep -a 'TOTAL_COV:' "$log_file" | awk '{print $NF}' | tail -1) || true
          if [[ -n "$fork_cov" ]]; then
            cov="$fork_cov"
          else
            # Fallback: look for latest cov: value in worker output
            cov=$(grep -a 'cov:' "$log_file" | grep -oP 'cov:\s*\K[0-9]+' | tail -1) || true
            [[ -z "$cov" ]] && cov="N/A"
          fi
        fi
      fi

      # Determine status
      local status="OK"
      if [[ -f "$log_file" ]]; then
        if grep -qi "SUMMARY:.*ERROR\|SUMMARY:.*bug\|==.*==ERROR" "$log_file" 2>/dev/null; then
          status="FINDING"
        elif grep -qi "BINGO\|crash-\|leak-" "$log_file" 2>/dev/null; then
          status="CRASH"
        fi
      else
        status="NO LOG"
      fi

      printf "%-30s %8d %8d %10s %8s %s\n" "$name" "$max_len" "$corpus_count" "$execs" "$cov" "$status"
    done

    echo ""
    echo "Corpus directory : $CORPUS_DIR"
    echo "Log files        : $REPORT_DIR/*.log"
    echo "============================================================"
  } | tee "$report_file"

  echo ""
  ok "Report saved to: $report_file"

  # Check for any crashes
  local crash_count=0
  for name in "${ALL_FUZZERS[@]}"; do
    local log_file="$REPORT_DIR/${name}.log"
    if [[ -f "$log_file" ]] && grep -qi "crash-\|leak-\|SUMMARY:.*ERROR" "$log_file" 2>/dev/null; then
      crash_count=$((crash_count + 1))
      warn "Potential finding in $name — see $log_file"
    fi
  done

  if [[ $crash_count -eq 0 ]]; then
    ok "No crashes or sanitizer errors found across all ${#ALL_FUZZERS[@]} fuzzers."
  else
    warn "$crash_count fuzzer(s) reported potential findings. Review the log files."
  fi
}

# ---------------------------------------------------------------------------
# Main
# ---------------------------------------------------------------------------
main() {
  header "ML-KEM Fuzzing Suite"
  info "Project root : $PROJECT_ROOT"
  info "Build dir    : $BUILD_DIR"
  info "Corpus dir   : $CORPUS_DIR"
  info "Report dir   : $REPORT_DIR"
  info "Fuzz time    : ${FUZZ_TIME}s per fuzzer"
  info "Fork workers : $FUZZ_FORK per fuzzer"
  info "Max parallel : $FUZZ_JOBS fuzzers"
  info "Compiler     : $CXX"
  echo ""

  local start_time
  start_time=$(date +%s)

  phase_configure
  phase_build
  phase_seed_corpus
  phase_fuzz
  phase_report

  local end_time elapsed_min
  end_time=$(date +%s)
  elapsed_min=$(( (end_time - start_time) / 60 ))
  echo ""
  ok "Total wall-clock time: ${elapsed_min} minutes."
}

main "$@"
