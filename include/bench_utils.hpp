#pragma once
#include "utils.hpp"
#include <benchmark/benchmark.h>

// Benchmark Kyber PQC suite implementation on CPU, using google-benchmark
namespace bench_kyber {

// Benchmark random byte array ( of length N ) generation using system
// randomness
template<const size_t len>
void
random_data(benchmark::State& state)
{
  uint8_t data[len]{};

  for (auto _ : state) {
    kyber_utils::random_data<uint8_t>(data, len);

    benchmark::DoNotOptimize(data);
    benchmark::ClobberMemory();
  }
}

}
