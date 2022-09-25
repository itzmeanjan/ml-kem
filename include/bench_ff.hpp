#pragma once
#include "ff.hpp"
#include <benchmark/benchmark.h>

// Benchmark Kyber PQC suite implementation on CPU, using google-benchmark
namespace bench_kyber {

// Benchmark addition over prime field Z_q | q = 3329
void
ff_add(benchmark::State& state)
{
  ff::ff_t a = ff::ff_t::random();
  ff::ff_t b{};

  for (auto _ : state) {
    b = b + a;
    benchmark::DoNotOptimize(b);
  }

  const size_t itr = state.iterations();

  ff::ff_t c{};
  for (size_t i = 0; i < itr; i++) {
    c = c + a;
  }

  assert(c == b);
}

// Benchmark compound addition over prime field Z_q | q = 3329
void
ff_compound_add(benchmark::State& state)
{
  ff::ff_t a = ff::ff_t::random();
  ff::ff_t b{};

  for (auto _ : state) {
    b += a;
    benchmark::DoNotOptimize(b);
  }

  const size_t itr = state.iterations();

  ff::ff_t c{};
  for (size_t i = 0; i < itr; i++) {
    c += a;
  }

  assert(c == b);
}

// Benchmark subtraction over prime field Z_q | q = 3329
void
ff_sub(benchmark::State& state)
{
  ff::ff_t a = ff::ff_t::random();
  ff::ff_t b{};

  for (auto _ : state) {
    b = b - a;
    benchmark::DoNotOptimize(b);
  }

  const size_t itr = state.iterations();

  ff::ff_t c{};
  for (size_t i = 0; i < itr; i++) {
    c = c - a;
  }

  assert(c == b);
}

}
