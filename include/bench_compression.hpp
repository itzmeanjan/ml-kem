#pragma once
#include "compression.hpp"
#include <benchmark/benchmark.h>

// Benchmark Kyber PQC suite implementation on CPU, using google-benchmark
namespace bench_kyber {

// Benchmark Z_q element compression routine
template<const size_t d>
void
compress(benchmark::State& state)
{
  const ff::ff_t a = ff::ff_t::random();
  ff::ff_t b = a;

  for (auto _ : state) {
    b = kyber_utils::compress<d>(b);

    benchmark::DoNotOptimize(b);
    benchmark::ClobberMemory();
  }

  const size_t itr = state.iterations();

  ff::ff_t c = a;
  for (size_t i = 0; i < itr; i++) {
    c = kyber_utils::compress<d>(c);
  }

  assert(c == b);
}

// Benchmark Z_q element decompression routine
template<const size_t d>
void
decompress(benchmark::State& state)
{
  const ff::ff_t a = ff::ff_t::random();
  ff::ff_t b = a;

  for (auto _ : state) {
    b = kyber_utils::decompress<d>(b);

    benchmark::DoNotOptimize(b);
    benchmark::ClobberMemory();
  }

  const size_t itr = state.iterations();

  ff::ff_t c = a;
  for (size_t i = 0; i < itr; i++) {
    c = kyber_utils::decompress<d>(c);
  }

  assert(c == b);
}

}
