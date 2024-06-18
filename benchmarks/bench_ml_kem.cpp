#include "bench_helper.hpp"
#include "ml_kem/internals/ml_kem.hpp"
#include "x86_64_cpu_ticks.hpp"
#include <benchmark/benchmark.h>

// Benchmarking IND-CCA2-secure Ml_kem KEM key generation algorithm
template<size_t k, size_t eta1, size_t bit_security_level>
void
bench_keygen(benchmark::State& state)
{
  constexpr size_t slen = 32;
  constexpr size_t pklen = ml_kem_utils::get_kem_public_key_len(k);
  constexpr size_t sklen = ml_kem_utils::get_kem_secret_key_len(k);

  std::vector<uint8_t> d(slen);
  std::vector<uint8_t> z(slen);
  std::vector<uint8_t> pkey(pklen);
  std::vector<uint8_t> skey(sklen);

  auto _d = std::span<uint8_t, slen>(d);
  auto _z = std::span<uint8_t, slen>(z);
  auto _pkey = std::span<uint8_t, pklen>(pkey);
  auto _skey = std::span<uint8_t, sklen>(skey);

  prng::prng_t<bit_security_level> prng{};
  prng.read(_d);
  prng.read(_z);

#ifdef __x86_64__
  uint64_t total_ticks = 0ul;
#endif

  for (auto _ : state) {
#ifdef __x86_64__
    const uint64_t start = cpu_ticks();
#endif

    ml_kem::keygen<k, eta1>(_d, _z, _pkey, _skey);

    benchmark::DoNotOptimize(_d);
    benchmark::DoNotOptimize(_z);
    benchmark::DoNotOptimize(_pkey);
    benchmark::DoNotOptimize(_skey);
    benchmark::ClobberMemory();

#ifdef __x86_64__
    const uint64_t end = cpu_ticks();
    total_ticks += (end - start);
#endif
  }

  state.SetItemsProcessed(state.iterations());

#ifdef __x86_64__
  total_ticks /= static_cast<uint64_t>(state.iterations());
  state.counters["rdtsc"] = static_cast<double>(total_ticks);
#endif
}

// Benchmarking IND-CCA2-secure Ml_kem KEM encapsulation algorithm
template<size_t k, size_t eta1, size_t eta2, size_t du, size_t dv, size_t bit_security_level>
void
bench_encapsulate(benchmark::State& state)
{
  constexpr size_t slen = 32;
  constexpr size_t pklen = ml_kem_utils::get_kem_public_key_len(k);
  constexpr size_t sklen = ml_kem_utils::get_kem_secret_key_len(k);
  constexpr size_t ctlen = ml_kem_utils::get_kem_cipher_text_len(k, du, dv);
  constexpr size_t klen = 32;

  std::vector<uint8_t> d(slen);
  std::vector<uint8_t> z(slen);
  std::vector<uint8_t> m(slen);
  std::vector<uint8_t> pkey(pklen);
  std::vector<uint8_t> skey(sklen);
  std::vector<uint8_t> cipher(ctlen);
  std::vector<uint8_t> sender_key(klen);

  auto _d = std::span<uint8_t, slen>(d);
  auto _z = std::span<uint8_t, slen>(z);
  auto _m = std::span<uint8_t, slen>(m);
  auto _pkey = std::span<uint8_t, pklen>(pkey);
  auto _skey = std::span<uint8_t, sklen>(skey);
  auto _cipher = std::span<uint8_t, ctlen>(cipher);
  auto _sender_key = std::span<uint8_t, klen>(sender_key);

  prng::prng_t<bit_security_level> prng{};
  prng.read(_d);
  prng.read(_z);

  ml_kem::keygen<k, eta1>(_d, _z, _pkey, _skey);

  prng.read(_m);

#ifdef __x86_64__
  uint64_t total_ticks = 0ul;
#endif

  for (auto _ : state) {
#ifdef __x86_64__
    const uint64_t start = cpu_ticks();
#endif

    (void)ml_kem::encapsulate<k, eta1, eta2, du, dv>(_m, _pkey, _cipher, _sender_key);

    benchmark::DoNotOptimize(_m);
    benchmark::DoNotOptimize(_pkey);
    benchmark::DoNotOptimize(_cipher);
    benchmark::DoNotOptimize(_sender_key);
    benchmark::ClobberMemory();

#ifdef __x86_64__
    const uint64_t end = cpu_ticks();
    total_ticks += (end - start);
#endif
  }

  state.SetItemsProcessed(state.iterations());

#ifdef __x86_64__
  total_ticks /= static_cast<uint64_t>(state.iterations());
  state.counters["rdtsc"] = static_cast<double>(total_ticks);
#endif
}

// Benchmarking IND-CCA2-secure Ml_kem KEM decapsulation algorithm
template<size_t k, size_t eta1, size_t eta2, size_t du, size_t dv, size_t bit_security_level>
void
bench_decapsulate(benchmark::State& state)
{
  constexpr size_t slen = 32;
  constexpr size_t pklen = ml_kem_utils::get_kem_public_key_len(k);
  constexpr size_t sklen = ml_kem_utils::get_kem_secret_key_len(k);
  constexpr size_t ctlen = ml_kem_utils::get_kem_cipher_text_len(k, du, dv);
  constexpr size_t klen = 32;

  std::vector<uint8_t> d(slen);
  std::vector<uint8_t> z(slen);
  std::vector<uint8_t> m(slen);
  std::vector<uint8_t> pkey(pklen);
  std::vector<uint8_t> skey(sklen);
  std::vector<uint8_t> cipher(ctlen);
  std::vector<uint8_t> sender_key(klen);
  std::vector<uint8_t> receiver_key(klen);

  auto _d = std::span<uint8_t, slen>(d);
  auto _z = std::span<uint8_t, slen>(z);
  auto _m = std::span<uint8_t, slen>(m);
  auto _pkey = std::span<uint8_t, pklen>(pkey);
  auto _skey = std::span<uint8_t, sklen>(skey);
  auto _cipher = std::span<uint8_t, ctlen>(cipher);
  auto _sender_key = std::span<uint8_t, klen>(sender_key);
  auto _receiver_key = std::span<uint8_t, klen>(receiver_key);

  prng::prng_t<bit_security_level> prng{};
  prng.read(_d);
  prng.read(_z);

  ml_kem::keygen<k, eta1>(_d, _z, _pkey, _skey);

  prng.read(_m);

  (void)ml_kem::encapsulate<k, eta1, eta2, du, dv>(_m, _pkey, _cipher, _sender_key);

#ifdef __x86_64__
  uint64_t total_ticks = 0ul;
#endif

  for (auto _ : state) {
#ifdef __x86_64__
    const uint64_t start = cpu_ticks();
#endif

    ml_kem::decapsulate<k, eta1, eta2, du, dv>(_skey, _cipher, _receiver_key);

    benchmark::DoNotOptimize(_skey);
    benchmark::DoNotOptimize(_cipher);
    benchmark::DoNotOptimize(_receiver_key);
    benchmark::ClobberMemory();

#ifdef __x86_64__
    const uint64_t end = cpu_ticks();
    total_ticks += (end - start);
#endif
  }

  state.SetItemsProcessed(state.iterations());
  assert(std::ranges::equal(_sender_key, _receiver_key));

#ifdef __x86_64__
  total_ticks /= static_cast<uint64_t>(state.iterations());
  state.counters["rdtsc"] = static_cast<double>(total_ticks);
#endif
}

// Ml_kem512
BENCHMARK(bench_keygen<2, 3, 128>)->Name("ml_kem512/keygen")->ComputeStatistics("min", compute_min)->ComputeStatistics("max", compute_max);
BENCHMARK(bench_encapsulate<2, 3, 2, 10, 4, 128>)->Name("ml_kem512/encap")->ComputeStatistics("min", compute_min)->ComputeStatistics("max", compute_max);
BENCHMARK(bench_decapsulate<2, 3, 2, 10, 4, 128>)->Name("ml_kem512/decap")->ComputeStatistics("min", compute_min)->ComputeStatistics("max", compute_max);

// Ml_kem768
BENCHMARK(bench_keygen<3, 2, 192>)->Name("ml_kem768/keygen")->ComputeStatistics("min", compute_min)->ComputeStatistics("max", compute_max);
BENCHMARK(bench_encapsulate<3, 2, 2, 10, 4, 192>)->Name("ml_kem768/encap")->ComputeStatistics("min", compute_min)->ComputeStatistics("max", compute_max);
BENCHMARK(bench_decapsulate<3, 2, 2, 10, 4, 192>)->Name("ml_kem768/decap")->ComputeStatistics("min", compute_min)->ComputeStatistics("max", compute_max);

// Ml_kem1024
BENCHMARK(bench_keygen<4, 2, 256>)->Name("ml_kem1024/keygen")->ComputeStatistics("min", compute_min)->ComputeStatistics("max", compute_max);
BENCHMARK(bench_encapsulate<4, 2, 2, 11, 5, 256>)->Name("ml_kem1024/encap")->ComputeStatistics("min", compute_min)->ComputeStatistics("max", compute_max);
BENCHMARK(bench_decapsulate<4, 2, 2, 11, 5, 256>)->Name("ml_kem1024/decap")->ComputeStatistics("min", compute_min)->ComputeStatistics("max", compute_max);
