#pragma once
#include "kyber_kem.hpp"
#include "utils.hpp"
#include <benchmark/benchmark.h>

// Benchmark Kyber PQC suite implementation on CPU, using google-benchmark
namespace bench_kyber {

// Benchmarking IND-CCA2-secure Kyber KEM key generation algorithm
template<const size_t k, const size_t eta1>
void
kem_keygen(benchmark::State& state)
{
  constexpr size_t slen = 32;
  constexpr size_t pklen = kyber_utils::get_ccakem_public_key_len<k>();
  constexpr size_t sklen = kyber_utils::get_ccakem_secret_key_len<k>();

  uint8_t* d = static_cast<uint8_t*>(std::malloc(slen));
  uint8_t* z = static_cast<uint8_t*>(std::malloc(slen));
  uint8_t* pkey = static_cast<uint8_t*>(std::malloc(pklen));
  uint8_t* skey = static_cast<uint8_t*>(std::malloc(sklen));

  prng::prng_t prng;
  prng.read(d, slen);
  prng.read(z, slen);

  for (auto _ : state) {
    ccakem::keygen<k, eta1>(d, z, pkey, skey);

    benchmark::DoNotOptimize(d);
    benchmark::DoNotOptimize(z);
    benchmark::DoNotOptimize(pkey);
    benchmark::DoNotOptimize(skey);
    benchmark::ClobberMemory();
  }

  state.SetItemsProcessed(state.iterations());

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
  constexpr size_t pklen = kyber_utils::get_ccakem_public_key_len<k>();
  constexpr size_t sklen = kyber_utils::get_ccakem_secret_key_len<k>();
  constexpr size_t ctlen = kyber_utils::get_ccakem_cipher_len<k, du, dv>();
  constexpr size_t klen = 32;

  uint8_t* d = static_cast<uint8_t*>(std::malloc(slen));
  uint8_t* z = static_cast<uint8_t*>(std::malloc(slen));
  uint8_t* m = static_cast<uint8_t*>(std::malloc(slen));
  uint8_t* pkey = static_cast<uint8_t*>(std::malloc(pklen));
  uint8_t* skey = static_cast<uint8_t*>(std::malloc(sklen));
  uint8_t* cipher = static_cast<uint8_t*>(std::malloc(ctlen));
  uint8_t* sender_key = static_cast<uint8_t*>(std::malloc(klen));

  prng::prng_t prng;
  prng.read(d, slen);
  prng.read(z, slen);

  ccakem::keygen<k, eta1>(d, z, pkey, skey);

  prng.read(m, slen);

  for (auto _ : state) {
    auto skdf = ccakem::encapsulate<k, eta1, eta2, du, dv>(m, pkey, cipher);
    benchmark::DoNotOptimize(skdf);
    skdf.read(sender_key, klen);

    benchmark::DoNotOptimize(m);
    benchmark::DoNotOptimize(pkey);
    benchmark::DoNotOptimize(cipher);
    benchmark::DoNotOptimize(sender_key);
    benchmark::ClobberMemory();
  }

  state.SetItemsProcessed(state.iterations());

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
  constexpr size_t pklen = kyber_utils::get_ccakem_public_key_len<k>();
  constexpr size_t sklen = kyber_utils::get_ccakem_secret_key_len<k>();
  constexpr size_t ctlen = kyber_utils::get_ccakem_cipher_len<k, du, dv>();
  constexpr size_t klen = 32;

  uint8_t* d = static_cast<uint8_t*>(std::malloc(slen));
  uint8_t* z = static_cast<uint8_t*>(std::malloc(slen));
  uint8_t* m = static_cast<uint8_t*>(std::malloc(slen));
  uint8_t* pkey = static_cast<uint8_t*>(std::malloc(pklen));
  uint8_t* skey = static_cast<uint8_t*>(std::malloc(sklen));
  uint8_t* cipher = static_cast<uint8_t*>(std::malloc(ctlen));
  uint8_t* sender_key = static_cast<uint8_t*>(std::malloc(klen));
  uint8_t* receiver_key = static_cast<uint8_t*>(std::malloc(klen));

  prng::prng_t prng;
  prng.read(d, slen);
  prng.read(z, slen);

  ccakem::keygen<k, eta1>(d, z, pkey, skey);

  prng.read(m, slen);

  auto skdf = ccakem::encapsulate<k, eta1, eta2, du, dv>(m, pkey, cipher);
  skdf.read(sender_key, klen);

  for (auto _ : state) {
    auto rkdf = ccakem::decapsulate<k, eta1, eta2, du, dv>(skey, cipher);
    benchmark::DoNotOptimize(rkdf);
    rkdf.read(receiver_key, klen);

    benchmark::DoNotOptimize(skey);
    benchmark::DoNotOptimize(cipher);
    benchmark::DoNotOptimize(receiver_key);
    benchmark::ClobberMemory();
  }

  state.SetItemsProcessed(state.iterations());

  for (size_t i = 0; i < klen; i++) {
    assert(sender_key[i] == receiver_key[i]);
  }

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
