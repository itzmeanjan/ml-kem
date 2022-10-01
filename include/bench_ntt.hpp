#pragma once
#include "ntt.hpp"
#include <benchmark/benchmark.h>

// Benchmark Kyber PQC suite implementation on CPU, using google-benchmark
namespace bench_kyber {

// Benchmark Number Theoretic Transform of degree-255 polynomial over
// Z_q | q = 3329
void
ntt(benchmark::State& state)
{
  ff::ff_t poly_a[ntt::N]{};
  ff::ff_t poly_b[ntt::N]{};

  for (size_t i = 0; i < ntt::N; i++) {
    poly_a[i] = ff::ff_t::random();
  }

  std::memcpy(poly_b, poly_a, sizeof(ff::ff_t) * ntt::N);

  for (auto _ : state) {
    ntt::ntt(poly_b);

    benchmark::DoNotOptimize(poly_b);
    benchmark::ClobberMemory();
  }

  const size_t itr = state.iterations();

  for (size_t i = 0; i < itr; i++) {
    ntt::ntt(poly_a);
  }

  for (size_t i = 0; i < ntt::N; i++) {
    assert(poly_a[i] == poly_b[i]);
  }
}

// Benchmark inverse Number Theoretic Transform of degree-255 polynomial over
// Z_q | q = 3329
void
intt(benchmark::State& state)
{
  ff::ff_t poly_a[ntt::N]{};
  ff::ff_t poly_b[ntt::N]{};

  for (size_t i = 0; i < ntt::N; i++) {
    poly_a[i] = ff::ff_t::random();
  }

  std::memcpy(poly_b, poly_a, sizeof(ff::ff_t) * ntt::N);

  for (auto _ : state) {
    ntt::intt(poly_b);

    benchmark::DoNotOptimize(poly_b);
    benchmark::ClobberMemory();
  }

  const size_t itr = state.iterations();

  for (size_t i = 0; i < itr; i++) {
    ntt::intt(poly_a);
  }

  for (size_t i = 0; i < ntt::N; i++) {
    assert(poly_a[i] == poly_b[i]);
  }
}

// Benchmark multiplication of two degree-255 polynomials in NTT domain
void
polymul(benchmark::State& state)
{
  ff::ff_t poly_a[ntt::N]{};
  ff::ff_t poly_b[ntt::N]{};
  ff::ff_t poly_c[ntt::N]{};

  for (size_t i = 0; i < ntt::N; i++) {
    poly_a[i] = ff::ff_t::random();
    poly_b[i] = ff::ff_t::random();
  }

  for (auto _ : state) {
    ntt::polymul(poly_a, poly_b, poly_c);

    benchmark::DoNotOptimize(poly_a);
    benchmark::DoNotOptimize(poly_b);
    benchmark::DoNotOptimize(poly_c);
    benchmark::ClobberMemory();
  }
}

}
