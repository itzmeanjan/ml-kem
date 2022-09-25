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
  constexpr size_t plen = sizeof(ff::ff_t) * ntt::N;
  constexpr size_t blen = 32 * l;
  constexpr uint16_t mask = (1u << l) - 1u;

  ff::ff_t src[plen]{};
  uint8_t arr[blen]{};
  ff::ff_t dst[plen]{};

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
}

}
