#pragma once
#include "ntt.hpp"
#include "sampling.hpp"
#include "utils.hpp"
#include <benchmark/benchmark.h>

// Benchmark Kyber PQC suite implementation on CPU, using google-benchmark
namespace bench_kyber {

// Benchmark how long does it take to generate public matrix A ( of dimension
// k x k, where each matrix element is a degree-255 polynomial in NTT form ),
// from an XOF SHAKE128, seeded with 32 -bytes randomness and two nonces ( each
// of 1 -byte )
template<const size_t k, const bool transpose>
void
generate_matrix(benchmark::State& state)
{
  constexpr size_t coeff_cnt = k * k * ntt::N;
  constexpr size_t coeff_len = sizeof(ff::ff_t) * coeff_cnt;
  constexpr size_t xof_len = sizeof(uint8_t) * 34;

  ff::ff_t* mat = static_cast<ff::ff_t*>(std::malloc(coeff_len));
  uint8_t* xof = static_cast<uint8_t*>(std::malloc(xof_len));

  std::memset(mat, 0, coeff_len);
  kyber_utils::random_data<uint8_t>(xof, 32);

  for (auto _ : state) {
    kyber_utils::generate_matrix<k, transpose>(mat, xof);

    benchmark::DoNotOptimize(mat);
    benchmark::DoNotOptimize(xof);
    benchmark::ClobberMemory();
  }

  std::free(mat);
  std::free(xof);
}

}
