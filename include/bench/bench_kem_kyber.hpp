#pragma once
#include "bench_common.hpp"
#include "decapsulation.hpp"
#include "encapsulation.hpp"
#include "kem_keygen.hpp"
#include "utils.hpp"

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

  std::vector<uint64_t> durations;

  for (auto _ : state) {
    kyber_utils::random_data<uint8_t>(d, slen);
    kyber_utils::random_data<uint8_t>(z, slen);

    const auto t0 = std::chrono::high_resolution_clock::now();

    ccakem::keygen<k, eta1>(d, z, pkey, skey);

    benchmark::DoNotOptimize(d);
    benchmark::DoNotOptimize(z);
    benchmark::DoNotOptimize(pkey);
    benchmark::DoNotOptimize(skey);
    benchmark::ClobberMemory();

    const auto t1 = std::chrono::high_resolution_clock::now();

    const auto sdur = std::chrono::duration_cast<seconds_t>(t1 - t0);
    const auto nsdur = std::chrono::duration_cast<nano_t>(t1 - t0);

    state.SetIterationTime(sdur.count());
    durations.push_back(nsdur.count());
  }

  state.SetItemsProcessed(static_cast<int64_t>(state.iterations()));
  assert(durations.size() == static_cast<size_t>(state.iterations()));

  const auto min_idx = std::min_element(durations.begin(), durations.end());
  const auto min = durations.at(std::distance(durations.begin(), min_idx));
  state.counters["min_exec_time (ns)"] = static_cast<double>(min);

  const auto max_idx = std::max_element(durations.begin(), durations.end());
  const auto max = durations.at(std::distance(durations.begin(), max_idx));
  state.counters["max_exec_time (ns)"] = static_cast<double>(max);

  const auto lenby2 = durations.size() / 2;
  const auto mid_idx = durations.begin() + lenby2;
  std::nth_element(durations.begin(), mid_idx, durations.end());
  const auto mid = durations[lenby2];
  state.counters["median_exec_time (ns)"] = static_cast<double>(mid);

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

  std::vector<uint64_t> durations;

  for (auto _ : state) {
    kyber_utils::random_data<uint8_t>(d, slen);
    kyber_utils::random_data<uint8_t>(z, slen);
    ccakem::keygen<k, eta1>(d, z, pkey, skey);

    kyber_utils::random_data<uint8_t>(m, slen);

    const auto t0 = std::chrono::high_resolution_clock::now();

    auto skdf = ccakem::encapsulate<k, eta1, eta2, du, dv>(m, pkey, cipher);
    skdf.read(sender_key, klen);

    benchmark::DoNotOptimize(m);
    benchmark::DoNotOptimize(pkey);
    benchmark::DoNotOptimize(cipher);
    benchmark::DoNotOptimize(sender_key);
    benchmark::ClobberMemory();

    const auto t1 = std::chrono::high_resolution_clock::now();

    const auto sdur = std::chrono::duration_cast<seconds_t>(t1 - t0);
    const auto nsdur = std::chrono::duration_cast<nano_t>(t1 - t0);

    state.SetIterationTime(sdur.count());
    durations.push_back(nsdur.count());
  }

  state.SetItemsProcessed(static_cast<int64_t>(state.iterations()));
  assert(durations.size() == static_cast<size_t>(state.iterations()));

  const auto min_idx = std::min_element(durations.begin(), durations.end());
  const auto min = durations.at(std::distance(durations.begin(), min_idx));
  state.counters["min_exec_time (ns)"] = static_cast<double>(min);

  const auto max_idx = std::max_element(durations.begin(), durations.end());
  const auto max = durations.at(std::distance(durations.begin(), max_idx));
  state.counters["max_exec_time (ns)"] = static_cast<double>(max);

  const auto lenby2 = durations.size() / 2;
  const auto mid_idx = durations.begin() + lenby2;
  std::nth_element(durations.begin(), mid_idx, durations.end());
  const auto mid = durations[lenby2];
  state.counters["median_exec_time (ns)"] = static_cast<double>(mid);

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

  std::vector<uint64_t> durations;

  for (auto _ : state) {
    kyber_utils::random_data<uint8_t>(d, slen);
    kyber_utils::random_data<uint8_t>(z, slen);
    ccakem::keygen<k, eta1>(d, z, pkey, skey);

    kyber_utils::random_data<uint8_t>(m, slen);
    auto skdf = ccakem::encapsulate<k, eta1, eta2, du, dv>(m, pkey, cipher);
    skdf.read(sender_key, klen);

    const auto t0 = std::chrono::high_resolution_clock::now();

    auto rkdf = ccakem::decapsulate<k, eta1, eta2, du, dv>(skey, cipher);
    rkdf.read(receiver_key, klen);

    benchmark::DoNotOptimize(skey);
    benchmark::DoNotOptimize(cipher);
    benchmark::DoNotOptimize(receiver_key);
    benchmark::ClobberMemory();

    const auto t1 = std::chrono::high_resolution_clock::now();

    for (size_t i = 0; i < klen; i++) {
      assert(!static_cast<bool>(sender_key[i] ^ receiver_key[i]));
    }

    const auto sdur = std::chrono::duration_cast<seconds_t>(t1 - t0);
    const auto nsdur = std::chrono::duration_cast<nano_t>(t1 - t0);

    state.SetIterationTime(sdur.count());
    durations.push_back(nsdur.count());
  }

  state.SetItemsProcessed(static_cast<int64_t>(state.iterations()));
  assert(durations.size() == static_cast<size_t>(state.iterations()));

  const auto min_idx = std::min_element(durations.begin(), durations.end());
  const auto min = durations.at(std::distance(durations.begin(), min_idx));
  state.counters["min_exec_time (ns)"] = static_cast<double>(min);

  const auto max_idx = std::max_element(durations.begin(), durations.end());
  const auto max = durations.at(std::distance(durations.begin(), max_idx));
  state.counters["max_exec_time (ns)"] = static_cast<double>(max);

  const auto lenby2 = durations.size() / 2;
  const auto mid_idx = durations.begin() + lenby2;
  std::nth_element(durations.begin(), mid_idx, durations.end());
  const auto mid = durations[lenby2];
  state.counters["median_exec_time (ns)"] = static_cast<double>(mid);

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
