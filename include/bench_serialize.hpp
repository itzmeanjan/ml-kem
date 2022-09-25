#pragma once
#include "ntt.hpp"
#include "serialize.hpp"
#include <benchmark/benchmark.h>

// Benchmark Kyber PQC suite implementation on CPU, using google-benchmark
namespace bench_kyber {

// Benchmark routine which encodes degree-255 polynomial to a byte array
template<const size_t l>
void
encode(benchmark::State& state)
{
  constexpr size_t blen = 32 * l;
  constexpr uint16_t mask = (1u << l) - 1u;

  ff::ff_t src[ntt::N]{};
  uint8_t arr[blen]{};
  ff::ff_t dst[ntt::N]{};

  for (size_t i = 0; i < ntt::N; i++) {
    src[i] = ff::ff_t::random();
  }

  for (auto _ : state) {
    kyber_utils::encode<l>(src, arr);

    benchmark::DoNotOptimize(src);
    benchmark::DoNotOptimize(arr);
    benchmark::ClobberMemory();
  }

  kyber_utils::decode<l>(arr, dst);

  for (size_t i = 0; i < ntt::N; i++) {
    assert((src[i].v & mask) == (dst[i].v & mask));
  }

  const size_t tot_bytes = state.iterations() * sizeof(arr);
  state.SetBytesProcessed(static_cast<int64_t>(tot_bytes));
}

// Benchmark routine which decodes a byte array to degree-255 polynomial
template<const size_t l>
void
decode(benchmark::State& state)
{
  constexpr size_t blen = 32 * l;
  constexpr uint16_t mask = (1u << l) - 1u;

  ff::ff_t src[ntt::N]{};
  uint8_t arr[blen]{};
  ff::ff_t dst[ntt::N]{};

  for (size_t i = 0; i < ntt::N; i++) {
    src[i] = ff::ff_t::random();
  }

  kyber_utils::encode<l>(src, arr);

  for (auto _ : state) {
    kyber_utils::decode<l>(arr, dst);

    benchmark::DoNotOptimize(arr);
    benchmark::DoNotOptimize(dst);
    benchmark::ClobberMemory();
  }

  for (size_t i = 0; i < ntt::N; i++) {
    assert((src[i].v & mask) == (dst[i].v & mask));
  }

  const size_t tot_bytes = state.iterations() * sizeof(arr);
  state.SetBytesProcessed(static_cast<int64_t>(tot_bytes));
}

}
