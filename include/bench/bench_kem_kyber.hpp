#pragma once
#include "decapsulation.hpp"
#include "encapsulation.hpp"
#include "kem_keygen.hpp"
#include "utils.hpp"
#include "x86_64_cpu_cycles.hpp"
#include <benchmark/benchmark.h>

// Benchmark Kyber PQC suite implementation on CPU, using google-benchmark
namespace bench_kyber {

// Benchmarking IND-CCA2-secure Kyber KEM key generation algorithm
template<const size_t k, const size_t eta1>
void
kem_keygen(benchmark::State& state)
{
  constexpr size_t slen = 32;
  constexpr size_t pklen = k * 12 * 32 + 32;
  constexpr size_t sklen = k * 12 * 32 + pklen + 32 + 32;

  uint8_t* d = static_cast<uint8_t*>(std::malloc(slen));
  uint8_t* z = static_cast<uint8_t*>(std::malloc(slen));
  uint8_t* pkey = static_cast<uint8_t*>(std::malloc(pklen));
  uint8_t* skey = static_cast<uint8_t*>(std::malloc(sklen));

  std::memset(pkey, 0, pklen);
  std::memset(skey, 0, sklen);

  kyber_utils::random_data<uint8_t>(d, slen);
  kyber_utils::random_data<uint8_t>(z, slen);

#if defined __x86_64__
  uint64_t total_cycles = 0ul;
#endif

  for (auto _ : state) {
#if defined __x86_64__
    const uint64_t start = cpu_cycles();
#endif

    ccakem::keygen<k, eta1>(d, z, pkey, skey);

#if defined __x86_64__
    const uint64_t end = cpu_cycles();
    total_cycles += (end - start);
#endif

    benchmark::DoNotOptimize(d);
    benchmark::DoNotOptimize(z);
    benchmark::DoNotOptimize(pkey);
    benchmark::DoNotOptimize(skey);
    benchmark::ClobberMemory();
  }

  state.SetItemsProcessed(static_cast<int64_t>(state.iterations()));

#if defined __x86_64__
  total_cycles /= static_cast<uint64_t>(state.iterations());
  state.counters["average_cpu_cycles"] = static_cast<double>(total_cycles);
#endif

  std::free(d);
  std::free(z);
  std::free(pkey);
  std::free(skey);
}

// Benchmarking IND-CCA2-secure Kyber KEM encapsulation algorithm
template<const size_t k,
         const size_t eta1,
         const size_t eta2,
         const size_t du,
         const size_t dv>
void
encapsulate(benchmark::State& state)
{
  constexpr size_t slen = 32;
  constexpr size_t pklen = k * 12 * 32 + 32;
  constexpr size_t sklen = k * 12 * 32 + pklen + 32 + 32;
  constexpr size_t ctlen = k * du * 32 + dv * 32;
  constexpr size_t klen = 32;

  uint8_t* d = static_cast<uint8_t*>(std::malloc(slen));
  uint8_t* z = static_cast<uint8_t*>(std::malloc(slen));
  uint8_t* m = static_cast<uint8_t*>(std::malloc(slen));
  uint8_t* pkey = static_cast<uint8_t*>(std::malloc(pklen));
  uint8_t* skey = static_cast<uint8_t*>(std::malloc(sklen));
  uint8_t* cipher = static_cast<uint8_t*>(std::malloc(ctlen));
  uint8_t* sender_key = static_cast<uint8_t*>(std::malloc(klen));

  std::memset(pkey, 0, pklen);
  std::memset(skey, 0, sklen);
  std::memset(cipher, 0, ctlen);
  std::memset(sender_key, 0, klen);

  kyber_utils::random_data<uint8_t>(d, slen);
  kyber_utils::random_data<uint8_t>(z, slen);
  kyber_utils::random_data<uint8_t>(m, slen);

  ccakem::keygen<k, eta1>(d, z, pkey, skey);

#if defined __x86_64__
  uint64_t total_cycles = 0ul;
#endif

  for (auto _ : state) {
#if defined __x86_64__
    const uint64_t start = cpu_cycles();
#endif

    auto skdf = ccakem::encapsulate<k, eta1, eta2, du, dv>(m, pkey, cipher);
    skdf.read(sender_key, klen);

#if defined __x86_64__
    const uint64_t end = cpu_cycles();
    total_cycles += (end - start);
#endif

    benchmark::DoNotOptimize(m);
    benchmark::DoNotOptimize(pkey);
    benchmark::DoNotOptimize(cipher);
    benchmark::DoNotOptimize(sender_key);
    benchmark::ClobberMemory();
  }

  state.SetItemsProcessed(static_cast<int64_t>(state.iterations()));

#if defined __x86_64__
  total_cycles /= static_cast<uint64_t>(state.iterations());
  state.counters["average_cpu_cycles"] = static_cast<double>(total_cycles);
#endif

  std::free(d);
  std::free(z);
  std::free(m);
  std::free(pkey);
  std::free(skey);
  std::free(cipher);
  std::free(sender_key);
}

// Benchmarking IND-CCA2-secure Kyber KEM decapsulation algorithm
template<const size_t k,
         const size_t eta1,
         const size_t eta2,
         const size_t du,
         const size_t dv>
void
decapsulate(benchmark::State& state)
{
  constexpr size_t slen = 32;
  constexpr size_t pklen = k * 12 * 32 + 32;
  constexpr size_t sklen = k * 12 * 32 + pklen + 32 + 32;
  constexpr size_t ctlen = k * du * 32 + dv * 32;
  constexpr size_t klen = 32;

  uint8_t* d = static_cast<uint8_t*>(std::malloc(slen));
  uint8_t* z = static_cast<uint8_t*>(std::malloc(slen));
  uint8_t* m = static_cast<uint8_t*>(std::malloc(slen));
  uint8_t* pkey = static_cast<uint8_t*>(std::malloc(pklen));
  uint8_t* skey = static_cast<uint8_t*>(std::malloc(sklen));
  uint8_t* cipher = static_cast<uint8_t*>(std::malloc(ctlen));
  uint8_t* sender_key = static_cast<uint8_t*>(std::malloc(klen));
  uint8_t* receiver_key = static_cast<uint8_t*>(std::malloc(klen));

  std::memset(pkey, 0, pklen);
  std::memset(skey, 0, sklen);
  std::memset(cipher, 0, ctlen);
  std::memset(sender_key, 0, klen);
  std::memset(receiver_key, 0, klen);

  kyber_utils::random_data<uint8_t>(d, slen);
  kyber_utils::random_data<uint8_t>(z, slen);
  kyber_utils::random_data<uint8_t>(m, slen);

  ccakem::keygen<k, eta1>(d, z, pkey, skey);
  auto skdf = ccakem::encapsulate<k, eta1, eta2, du, dv>(m, pkey, cipher);
  skdf.read(sender_key, klen);

#if defined __x86_64__
  uint64_t total_cycles = 0ul;
#endif

  for (auto _ : state) {
#if defined __x86_64__
    const uint64_t start = cpu_cycles();
#endif

    auto rkdf = ccakem::decapsulate<k, eta1, eta2, du, dv>(skey, cipher);
    rkdf.read(receiver_key, klen);

#if defined __x86_64__
    const uint64_t end = cpu_cycles();
    total_cycles += (end - start);
#endif

    benchmark::DoNotOptimize(skey);
    benchmark::DoNotOptimize(cipher);
    benchmark::DoNotOptimize(receiver_key);
    benchmark::ClobberMemory();
  }

  for (size_t i = 0; i < klen; i++) {
    assert(!static_cast<bool>(sender_key[i] ^ receiver_key[i]));
  }

  state.SetItemsProcessed(static_cast<int64_t>(state.iterations()));

#if defined __x86_64__
  total_cycles /= static_cast<uint64_t>(state.iterations());
  state.counters["average_cpu_cycles"] = static_cast<double>(total_cycles);
#endif

  std::free(d);
  std::free(z);
  std::free(m);
  std::free(pkey);
  std::free(skey);
  std::free(cipher);
  std::free(sender_key);
  std::free(receiver_key);
}

}
