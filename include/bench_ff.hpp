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
    benchmark::DoNotOptimize(a);
    benchmark::ClobberMemory();
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
    benchmark::DoNotOptimize(a);
    benchmark::ClobberMemory();
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
    benchmark::DoNotOptimize(a);
    benchmark::ClobberMemory();
  }

  const size_t itr = state.iterations();

  ff::ff_t c{};
  for (size_t i = 0; i < itr; i++) {
    c = c - a;
  }

  assert(c == b);
}

// Benchmark negation over prime field Z_q | q = 3329
void
ff_neg(benchmark::State& state)
{
  ff::ff_t a = ff::ff_t::random();
  ff::ff_t b{};

  for (auto _ : state) {
    b = -a;

    benchmark::DoNotOptimize(b);
    benchmark::DoNotOptimize(a);
    benchmark::ClobberMemory();
  }

  ff::ff_t c = -a;
  assert(c == b);
}

// Benchmark multiplication over prime field Z_q | q = 3329
void
ff_mul(benchmark::State& state)
{
  ff::ff_t a = ff::ff_t::random();
  ff::ff_t b{ 1 };

  for (auto _ : state) {
    b = b * a;

    benchmark::DoNotOptimize(b);
    benchmark::DoNotOptimize(a);
    benchmark::ClobberMemory();
  }

  const size_t itr = state.iterations();

  ff::ff_t c{ 1 };
  for (size_t i = 0; i < itr; i++) {
    c = c * a;
  }

  assert(c == b);
}

// Benchmark computation of multiplicative inverse over prime field
// Z_q | q = 3329
void
ff_inv(benchmark::State& state)
{
  ff::ff_t a = ff::ff_t::random();
  ff::ff_t b{};

  for (auto _ : state) {
    b = a.inv();

    benchmark::DoNotOptimize(b);
    benchmark::DoNotOptimize(a);
    benchmark::ClobberMemory();
  }

  ff::ff_t c = a.inv();
  assert(c == b);
}

// Benchmark division over prime field Z_q | q = 3329
void
ff_div(benchmark::State& state)
{
  ff::ff_t a = ff::ff_t::random();
  ff::ff_t b{ 1 };

  for (auto _ : state) {
    b = b / a;

    benchmark::DoNotOptimize(b);
    benchmark::DoNotOptimize(a);
    benchmark::ClobberMemory();
  }

  const size_t itr = state.iterations();

  ff::ff_t c{ 1 };
  for (size_t i = 0; i < itr; i++) {
    c = c / a;
  }

  assert(c == b);
}

}
