#pragma once
#include "pke_keygen.hpp"
#include <benchmark/benchmark.h>

// Benchmark Kyber PQC suite implementation on CPU, using google-benchmark
namespace bench_kyber {

// Benchmarking IND-CPA-secure Kyber Public Key Encryption  ( PKE ) key
// generation algorithm
template<const size_t k, const size_t eta1>
void
keygen(benchmark::State& state)
{
  constexpr size_t pklen = k * 12 * 32 + 32;
  constexpr size_t sklen = k * 12 * 32;

  uint8_t* pkey = static_cast<uint8_t*>(std::malloc(pklen));
  uint8_t* skey = static_cast<uint8_t*>(std::malloc(sklen));

  std::memset(pkey, 0, pklen);
  std::memset(skey, 0, sklen);

  for (auto _ : state) {
    cpapke::keygen<k, eta1>(pkey, skey);

    benchmark::DoNotOptimize(pkey);
    benchmark::DoNotOptimize(skey);
    benchmark::ClobberMemory();
  }

  state.SetItemsProcessed(static_cast<int64_t>(state.iterations()));

  std::free(pkey);
  std::free(skey);
}

}
