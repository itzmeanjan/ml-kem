#pragma once
#include <algorithm>
#include <benchmark/benchmark.h>
#include <chrono>
#include <vector>

// Benchmark Kyber PQC suite implementation on CPU, using google-benchmark
namespace bench_kyber {

// Alias for casting duration to second level granularity, used for manual
// timing, during benchmarking, using chrono high resolution clock
using seconds_t = std::chrono::duration<double>;
// Alias for casting duration to nanosecond level granularity, used for timing
// minimum, median and maximum execution times, when benchmarking
using nano_t = std::chrono::nanoseconds;

}
