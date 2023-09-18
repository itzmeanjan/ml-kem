#include "kem.hpp"
#include "utils.hpp"
#include <algorithm>
#include <benchmark/benchmark.h>
#include <vector>

// Benchmarking IND-CCA2-secure Kyber KEM key generation algorithm
template<size_t k, size_t eta1>
void
bench_keygen(benchmark::State& state)
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
template<size_t k, size_t eta1, size_t eta2, size_t du, size_t dv>
void
bench_encapsulate(benchmark::State& state)
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
template<size_t k, size_t eta1, size_t eta2, size_t du, size_t dv>
void
bench_decapsulate(benchmark::State& state)
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

// Register for benchmarking IND-CCA2-secure Kyber Key Encapsulation Mechanism

// Kyber512
BENCHMARK(bench_keygen<2, 3>)->Name("kyber512/keygen");
BENCHMARK(bench_encapsulate<2, 3, 2, 10, 4>)->Name("kyber512/encap");
BENCHMARK(bench_decapsulate<2, 3, 2, 10, 4>)->Name("kyber512/decap");

// Kyber768
BENCHMARK(bench_keygen<3, 2>)->Name("kyber768/keygen");
BENCHMARK(bench_encapsulate<3, 2, 2, 10, 4>)->Name("kyber768/encap");
BENCHMARK(bench_decapsulate<3, 2, 2, 10, 4>)->Name("kyber768/decap");

// Kyber1024
BENCHMARK(bench_keygen<4, 2>)->Name("kyber1024/keygen");
BENCHMARK(bench_encapsulate<4, 2, 2, 11, 5>)->Name("kyber1024/encap");
BENCHMARK(bench_decapsulate<4, 2, 2, 11, 5>)->Name("kyber1024/decap");
