#include "bench/bench_kem.hpp"

auto compute_min = [](const std::vector<double>& v) -> double {
  return *std::min_element(v.begin(), v.end());
};

auto compute_max = [](const std::vector<double>& v) -> double {
  return *std::max_element(v.begin(), v.end());
};

// Register for benchmarking IND-CCA2-secure Kyber Key Encapsulation Mechanism

// Kyber512
BENCHMARK(bench_kyber::keygen<2, 3>)
  ->Name("kyber512_keygen")
  ->ComputeStatistics("min", compute_min)
  ->ComputeStatistics("max", compute_max);
BENCHMARK(bench_kyber::encapsulate<2, 3, 2, 10, 4>)
  ->Name("kyber512_encap")
  ->ComputeStatistics("min", compute_min)
  ->ComputeStatistics("max", compute_max);
BENCHMARK(bench_kyber::decapsulate<2, 3, 2, 10, 4>)
  ->Name("kyber512_decap")
  ->ComputeStatistics("min", compute_min)
  ->ComputeStatistics("max", compute_max);

// Kyber768
BENCHMARK(bench_kyber::keygen<3, 2>)
  ->Name("kyber768_keygen")
  ->ComputeStatistics("min", compute_min)
  ->ComputeStatistics("max", compute_max);
BENCHMARK(bench_kyber::encapsulate<3, 2, 2, 10, 4>)
  ->Name("kyber768_encap")
  ->ComputeStatistics("min", compute_min)
  ->ComputeStatistics("max", compute_max);
BENCHMARK(bench_kyber::decapsulate<3, 2, 2, 10, 4>)
  ->Name("kyber768_decap")
  ->ComputeStatistics("min", compute_min)
  ->ComputeStatistics("max", compute_max);

// Kyber1024
BENCHMARK(bench_kyber::keygen<4, 2>)
  ->Name("kyber1024_keygen")
  ->ComputeStatistics("min", compute_min)
  ->ComputeStatistics("max", compute_max);
BENCHMARK(bench_kyber::encapsulate<4, 2, 2, 11, 5>)
  ->Name("kyber1024_encap")
  ->ComputeStatistics("min", compute_min)
  ->ComputeStatistics("max", compute_max);
BENCHMARK(bench_kyber::decapsulate<4, 2, 2, 11, 5>)
  ->Name("kyber1024_decap")
  ->ComputeStatistics("min", compute_min)
  ->ComputeStatistics("max", compute_max);

// benchmark runner main routine
BENCHMARK_MAIN();
