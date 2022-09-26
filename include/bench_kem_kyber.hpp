#pragma once
#include "decapsulation.hpp"
#include "encapsulation.hpp"
#include "kem_keygen.hpp"
#include <benchmark/benchmark.h>

// Benchmark Kyber PQC suite implementation on CPU, using google-benchmark
namespace bench_kyber {

// Benchmarking IND-CCA2-secure Kyber KEM key generation algorithm
template<const size_t k, const size_t eta1>
void
kem_keygen(benchmark::State& state)
{
  constexpr size_t pklen = k * 12 * 32 + 32;
  constexpr size_t sklen = k * 12 * 32 + pklen + 32 + 32;

  uint8_t* pkey = static_cast<uint8_t*>(std::malloc(pklen));
  uint8_t* skey = static_cast<uint8_t*>(std::malloc(sklen));

  std::memset(pkey, 0, pklen);
  std::memset(skey, 0, sklen);

  for (auto _ : state) {
    ccakem::keygen<k, eta1>(pkey, skey);

    benchmark::DoNotOptimize(pkey);
    benchmark::DoNotOptimize(skey);
    benchmark::ClobberMemory();
  }

  state.SetItemsProcessed(static_cast<int64_t>(state.iterations()));

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
  constexpr size_t pklen = k * 12 * 32 + 32;
  constexpr size_t sklen = k * 12 * 32 + pklen + 32 + 32;
  constexpr size_t ctlen = k * du * 32 + dv * 32;
  constexpr size_t klen = 32;

  uint8_t* pkey = static_cast<uint8_t*>(std::malloc(pklen));
  uint8_t* skey = static_cast<uint8_t*>(std::malloc(sklen));
  uint8_t* cipher = static_cast<uint8_t*>(std::malloc(ctlen));
  uint8_t* sender_key = static_cast<uint8_t*>(std::malloc(klen));

  std::memset(pkey, 0, pklen);
  std::memset(skey, 0, sklen);
  std::memset(cipher, 0, ctlen);
  std::memset(sender_key, 0, klen);

  ccakem::keygen<k, eta1>(pkey, skey);

  for (auto _ : state) {
    auto skdf = ccakem::encapsulate<k, eta1, eta2, du, dv>(pkey, cipher);
    skdf.read(sender_key, klen);

    benchmark::DoNotOptimize(pkey);
    benchmark::DoNotOptimize(cipher);
    benchmark::DoNotOptimize(sender_key);
    benchmark::ClobberMemory();
  }

  state.SetItemsProcessed(static_cast<int64_t>(state.iterations()));

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
  constexpr size_t pklen = k * 12 * 32 + 32;
  constexpr size_t sklen = k * 12 * 32 + pklen + 32 + 32;
  constexpr size_t ctlen = k * du * 32 + dv * 32;
  constexpr size_t klen = 32;

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

  ccakem::keygen<k, eta1>(pkey, skey);
  auto skdf = ccakem::encapsulate<k, eta1, eta2, du, dv>(pkey, cipher);
  skdf.read(sender_key, klen);

  for (auto _ : state) {
    auto rkdf = ccakem::decapsulate<k, eta1, eta2, du, dv>(skey, cipher);
    rkdf.read(receiver_key, klen);

    benchmark::DoNotOptimize(skey);
    benchmark::DoNotOptimize(cipher);
    benchmark::DoNotOptimize(receiver_key);
    benchmark::ClobberMemory();
  }

  for (size_t i = 0; i < klen; i++) {
    assert(!static_cast<bool>(sender_key[i] ^ receiver_key[i]));
  }

  state.SetItemsProcessed(static_cast<int64_t>(state.iterations()));

  std::free(pkey);
  std::free(skey);
  std::free(cipher);
  std::free(sender_key);
  std::free(receiver_key);
}

}
