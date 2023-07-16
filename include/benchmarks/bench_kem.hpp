#pragma once
#include "kem.hpp"
#include "utils.hpp"
#include <algorithm>
#include <benchmark/benchmark.h>
#include <vector>

// Benchmark Kyber PQC suite implementation on CPU, using google-benchmark
namespace bench_kyber {

// Benchmarking IND-CCA2-secure Kyber KEM key generation algorithm
template<const size_t k, const size_t eta1>
void
keygen(benchmark::State& state)
{
  constexpr size_t slen = 32;
  constexpr size_t pklen = kyber_utils::get_kem_public_key_len<k>();
  constexpr size_t sklen = kyber_utils::get_kem_secret_key_len<k>();

  std::vector<uint8_t> d(slen);
  std::vector<uint8_t> z(slen);
  std::vector<uint8_t> pkey(pklen);
  std::vector<uint8_t> skey(sklen);

  prng::prng_t prng;
  prng.read(d.data(), d.size());
  prng.read(z.data(), z.size());

  for (auto _ : state) {
    kem::keygen<k, eta1>(d.data(), z.data(), pkey.data(), skey.data());

    benchmark::DoNotOptimize(d);
    benchmark::DoNotOptimize(z);
    benchmark::DoNotOptimize(pkey);
    benchmark::DoNotOptimize(skey);
    benchmark::ClobberMemory();
  }

  state.SetItemsProcessed(state.iterations());
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
  constexpr size_t pklen = kyber_utils::get_kem_public_key_len<k>();
  constexpr size_t sklen = kyber_utils::get_kem_secret_key_len<k>();
  constexpr size_t ctlen = kyber_utils::get_kem_cipher_len<k, du, dv>();
  constexpr size_t klen = 32;

  std::vector<uint8_t> d(slen);
  std::vector<uint8_t> z(slen);
  std::vector<uint8_t> m(slen);
  std::vector<uint8_t> pkey(pklen);
  std::vector<uint8_t> skey(sklen);
  std::vector<uint8_t> cipher(ctlen);
  std::vector<uint8_t> sender_key(klen);

  prng::prng_t prng;
  prng.read(d.data(), d.size());
  prng.read(z.data(), z.size());

  kem::keygen<k, eta1>(d.data(), z.data(), pkey.data(), skey.data());

  prng.read(m.data(), m.size());

  for (auto _ : state) {
    auto skdf = kem::encapsulate<k, eta1, eta2, du, dv>(
      m.data(), pkey.data(), cipher.data());
    benchmark::DoNotOptimize(skdf);
    skdf.squeeze(sender_key.data(), sender_key.size());

    benchmark::DoNotOptimize(m);
    benchmark::DoNotOptimize(pkey);
    benchmark::DoNotOptimize(cipher);
    benchmark::DoNotOptimize(sender_key);
    benchmark::ClobberMemory();
  }

  state.SetItemsProcessed(state.iterations());
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
  constexpr size_t pklen = kyber_utils::get_kem_public_key_len<k>();
  constexpr size_t sklen = kyber_utils::get_kem_secret_key_len<k>();
  constexpr size_t ctlen = kyber_utils::get_kem_cipher_len<k, du, dv>();
  constexpr size_t klen = 32;

  std::vector<uint8_t> d(slen);
  std::vector<uint8_t> z(slen);
  std::vector<uint8_t> m(slen);
  std::vector<uint8_t> pkey(pklen);
  std::vector<uint8_t> skey(sklen);
  std::vector<uint8_t> cipher(ctlen);
  std::vector<uint8_t> sender_key(klen);
  std::vector<uint8_t> receiver_key(klen);

  prng::prng_t prng;
  prng.read(d.data(), d.size());
  prng.read(z.data(), z.size());

  kem::keygen<k, eta1>(d.data(), z.data(), pkey.data(), skey.data());

  prng.read(m.data(), m.size());

  auto skdf = kem::encapsulate<k, eta1, eta2, du, dv>(
    m.data(), pkey.data(), cipher.data());
  skdf.squeeze(sender_key.data(), sender_key.size());

  for (auto _ : state) {
    auto rkdf =
      kem::decapsulate<k, eta1, eta2, du, dv>(skey.data(), cipher.data());
    benchmark::DoNotOptimize(rkdf);
    rkdf.squeeze(receiver_key.data(), receiver_key.size());

    benchmark::DoNotOptimize(skey);
    benchmark::DoNotOptimize(cipher);
    benchmark::DoNotOptimize(receiver_key);
    benchmark::ClobberMemory();
  }

  state.SetItemsProcessed(state.iterations());
  assert(std::ranges::equal(sender_key, receiver_key));
}

}
