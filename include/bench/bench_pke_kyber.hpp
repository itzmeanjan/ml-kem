#pragma once
#include "kyber_pke.hpp"
#include "utils.hpp"
#include <benchmark/benchmark.h>

// Benchmark Kyber PQC suite implementation on CPU, using google-benchmark
namespace bench_kyber {

// Benchmarking IND-CPA-secure Kyber PKE key generation algorithm
template<const size_t k, const size_t eta1>
void
pke_keygen(benchmark::State& state)
{
  constexpr size_t slen = 32;
  constexpr size_t pklen = kyber_utils::get_cpapke_public_key_len<k>();
  constexpr size_t sklen = kyber_utils::get_cpapke_secret_key_len<k>();

  uint8_t* seed = static_cast<uint8_t*>(std::malloc(slen));
  uint8_t* pkey = static_cast<uint8_t*>(std::malloc(pklen));
  uint8_t* skey = static_cast<uint8_t*>(std::malloc(sklen));

  prng::prng_t prng;
  prng.read(seed, slen);

  for (auto _ : state) {
    cpapke::keygen<k, eta1>(seed, pkey, skey);

    benchmark::DoNotOptimize(seed);
    benchmark::DoNotOptimize(pkey);
    benchmark::DoNotOptimize(skey);
    benchmark::ClobberMemory();
  }

  state.SetItemsProcessed(state.iterations());

  std::free(seed);
  std::free(pkey);
  std::free(skey);
}

// Benchmarking IND-CPA-secure Kyber public key encryption algorithm
template<const size_t k,
         const size_t eta1,
         const size_t eta2,
         const size_t du,
         const size_t dv>
void
encrypt(benchmark::State& state)
{
  constexpr size_t slen = 32;
  constexpr size_t pklen = kyber_utils::get_cpapke_public_key_len<k>();
  constexpr size_t sklen = kyber_utils::get_cpapke_secret_key_len<k>();
  constexpr size_t mlen = 32;
  constexpr size_t enclen = kyber_utils::get_cpapke_cipher_len<k, du, dv>();

  uint8_t* seed = static_cast<uint8_t*>(std::malloc(slen));
  uint8_t* pkey = static_cast<uint8_t*>(std::malloc(pklen));
  uint8_t* skey = static_cast<uint8_t*>(std::malloc(sklen));
  uint8_t* rcoin = static_cast<uint8_t*>(std::malloc(mlen));
  uint8_t* txt = static_cast<uint8_t*>(std::malloc(mlen));
  uint8_t* enc = static_cast<uint8_t*>(std::malloc(enclen));

  prng::prng_t prng;
  prng.read(seed, slen);

  cpapke::keygen<k, eta1>(seed, pkey, skey);

  prng.read(txt, mlen);
  prng.read(rcoin, mlen);

  for (auto _ : state) {
    cpapke::encrypt<k, eta1, eta2, du, dv>(pkey, txt, rcoin, enc);

    benchmark::DoNotOptimize(pkey);
    benchmark::DoNotOptimize(txt);
    benchmark::DoNotOptimize(rcoin);
    benchmark::DoNotOptimize(enc);
    benchmark::ClobberMemory();
  }

  state.SetItemsProcessed(state.iterations());

  std::free(seed);
  std::free(pkey);
  std::free(skey);
  std::free(rcoin);
  std::free(txt);
  std::free(enc);
}

// Benchmarking IND-CPA-secure Kyber public key decryption algorithm
template<const size_t k,
         const size_t eta1,
         const size_t eta2,
         const size_t du,
         const size_t dv>
void
decrypt(benchmark::State& state)
{
  constexpr size_t slen = 32;
  constexpr size_t pklen = kyber_utils::get_cpapke_public_key_len<k>();
  constexpr size_t sklen = kyber_utils::get_cpapke_secret_key_len<k>();
  constexpr size_t mlen = 32;
  constexpr size_t enclen = kyber_utils::get_cpapke_cipher_len<k, du, dv>();

  uint8_t* seed = static_cast<uint8_t*>(std::malloc(slen));
  uint8_t* pkey = static_cast<uint8_t*>(std::malloc(pklen));
  uint8_t* skey = static_cast<uint8_t*>(std::malloc(sklen));
  uint8_t* rcoin = static_cast<uint8_t*>(std::malloc(mlen));
  uint8_t* txt = static_cast<uint8_t*>(std::malloc(mlen));
  uint8_t* enc = static_cast<uint8_t*>(std::malloc(enclen));
  uint8_t* dec = static_cast<uint8_t*>(std::malloc(mlen));

  prng::prng_t prng;
  prng.read(seed, slen);

  cpapke::keygen<k, eta1>(seed, pkey, skey);

  prng.read(txt, mlen);
  prng.read(rcoin, mlen);

  cpapke::encrypt<k, eta1, eta2, du, dv>(pkey, txt, rcoin, enc);

  for (auto _ : state) {
    cpapke::decrypt<k, du, dv>(skey, enc, dec);

    benchmark::DoNotOptimize(skey);
    benchmark::DoNotOptimize(enc);
    benchmark::DoNotOptimize(dec);
    benchmark::ClobberMemory();
  }

  state.SetItemsProcessed(state.iterations());

  std::free(seed);
  std::free(pkey);
  std::free(skey);
  std::free(rcoin);
  std::free(txt);
  std::free(enc);
  std::free(dec);
}

}
