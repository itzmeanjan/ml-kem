#include "bench/bench_kyber.hpp"

auto compute_min = [](const std::vector<double>& v) -> double {
  return *std::min_element(v.begin(), v.end());
};

auto compute_max = [](const std::vector<double>& v) -> double {
  return *std::max_element(v.begin(), v.end());
};

// Register for benchmarking IND-CPA-secure Kyber Public Key Encryption

// Kyber512
BENCHMARK(bench_kyber::pke_keygen<2, 3>)
  ->ComputeStatistics("min", compute_min)
  ->ComputeStatistics("max", compute_max);
BENCHMARK(bench_kyber::encrypt<2, 3, 2, 10, 4>)
  ->ComputeStatistics("min", compute_min)
  ->ComputeStatistics("max", compute_max);
BENCHMARK(bench_kyber::decrypt<2, 3, 2, 10, 4>)
  ->ComputeStatistics("min", compute_min)
  ->ComputeStatistics("max", compute_max);

// Kyber768
BENCHMARK(bench_kyber::pke_keygen<3, 2>)
  ->ComputeStatistics("min", compute_min)
  ->ComputeStatistics("max", compute_max);
BENCHMARK(bench_kyber::encrypt<3, 2, 2, 10, 4>)
  ->ComputeStatistics("min", compute_min)
  ->ComputeStatistics("max", compute_max);
BENCHMARK(bench_kyber::decrypt<3, 2, 2, 10, 4>)
  ->ComputeStatistics("min", compute_min)
  ->ComputeStatistics("max", compute_max);

// Kyber1024
BENCHMARK(bench_kyber::pke_keygen<4, 2>)
  ->ComputeStatistics("min", compute_min)
  ->ComputeStatistics("max", compute_max);
BENCHMARK(bench_kyber::encrypt<4, 2, 2, 11, 5>)
  ->ComputeStatistics("min", compute_min)
  ->ComputeStatistics("max", compute_max);
BENCHMARK(bench_kyber::decrypt<4, 2, 2, 11, 5>)
  ->ComputeStatistics("min", compute_min)
  ->ComputeStatistics("max", compute_max);

// Register for benchmarking IND-CCA2-secure Kyber Key Encapsulation Mechanism

// Kyber512
BENCHMARK(bench_kyber::kem_keygen<2, 3>)
  ->ComputeStatistics("min", compute_min)
  ->ComputeStatistics("max", compute_max);
BENCHMARK(bench_kyber::encapsulate<2, 3, 2, 10, 4>)
  ->ComputeStatistics("min", compute_min)
  ->ComputeStatistics("max", compute_max);
BENCHMARK(bench_kyber::decapsulate<2, 3, 2, 10, 4>)
  ->ComputeStatistics("min", compute_min)
  ->ComputeStatistics("max", compute_max);

// Kyber768
BENCHMARK(bench_kyber::kem_keygen<3, 2>)
  ->ComputeStatistics("min", compute_min)
  ->ComputeStatistics("max", compute_max);
BENCHMARK(bench_kyber::encapsulate<3, 2, 2, 10, 4>)
  ->ComputeStatistics("min", compute_min)
  ->ComputeStatistics("max", compute_max);
BENCHMARK(bench_kyber::decapsulate<3, 2, 2, 10, 4>)
  ->ComputeStatistics("min", compute_min)
  ->ComputeStatistics("max", compute_max);

// Kyber1024
BENCHMARK(bench_kyber::kem_keygen<4, 2>)
  ->ComputeStatistics("min", compute_min)
  ->ComputeStatistics("max", compute_max);
BENCHMARK(bench_kyber::encapsulate<4, 2, 2, 11, 5>)
  ->ComputeStatistics("min", compute_min)
  ->ComputeStatistics("max", compute_max);
BENCHMARK(bench_kyber::decapsulate<4, 2, 2, 11, 5>)
  ->ComputeStatistics("min", compute_min)
  ->ComputeStatistics("max", compute_max);

// benchmark runner main routine
BENCHMARK_MAIN();
