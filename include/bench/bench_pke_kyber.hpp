#pragma once
#include "bench_common.hpp"
#include "decryption.hpp"
#include "encryption.hpp"
#include "pke_keygen.hpp"
#include "utils.hpp"

// Benchmark Kyber PQC suite implementation on CPU, using google-benchmark
namespace bench_kyber {

// Benchmarking IND-CPA-secure Kyber PKE key generation algorithm
template<const size_t k, const size_t eta1>
void
pke_keygen(benchmark::State& state)
{
  constexpr size_t slen = 32;
  constexpr size_t pklen = k * 12 * 32 + 32;
  constexpr size_t sklen = k * 12 * 32;

  uint8_t* seed = static_cast<uint8_t*>(std::malloc(slen));
  uint8_t* pkey = static_cast<uint8_t*>(std::malloc(pklen));
  uint8_t* skey = static_cast<uint8_t*>(std::malloc(sklen));

  std::vector<uint64_t> durations;

  for (auto _ : state) {
    kyber_utils::random_data<uint8_t>(seed, slen);

    const auto t0 = std::chrono::high_resolution_clock::now();
    cpapke::keygen<k, eta1>(seed, pkey, skey);
    const auto t1 = std::chrono::high_resolution_clock::now();

    benchmark::DoNotOptimize(seed);
    benchmark::DoNotOptimize(pkey);
    benchmark::DoNotOptimize(skey);
    benchmark::ClobberMemory();

    const auto sdur = std::chrono::duration_cast<seconds_t>(t1 - t0);
    const auto nsdur = std::chrono::duration_cast<nano_t>(t1 - t0);

    state.SetIterationTime(sdur.count());
    durations.push_back(nsdur.count());
  }

  state.SetItemsProcessed(static_cast<int64_t>(state.iterations()));

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
  constexpr size_t pklen = k * 12 * 32 + 32;
  constexpr size_t sklen = k * 12 * 32;
  constexpr size_t mlen = 32;
  constexpr size_t enclen = k * du * 32 + dv * 32;

  uint8_t* seed = static_cast<uint8_t*>(std::malloc(slen));
  uint8_t* pkey = static_cast<uint8_t*>(std::malloc(pklen));
  uint8_t* skey = static_cast<uint8_t*>(std::malloc(sklen));
  uint8_t* rcoin = static_cast<uint8_t*>(std::malloc(mlen));
  uint8_t* txt = static_cast<uint8_t*>(std::malloc(mlen));
  uint8_t* enc = static_cast<uint8_t*>(std::malloc(enclen));

  std::vector<uint64_t> durations;

  for (auto _ : state) {
    kyber_utils::random_data<uint8_t>(seed, slen);
    cpapke::keygen<k, eta1>(seed, pkey, skey);

    kyber_utils::random_data<uint8_t>(txt, mlen);
    kyber_utils::random_data<uint8_t>(rcoin, mlen);

    const auto t0 = std::chrono::high_resolution_clock::now();
    cpapke::encrypt<k, eta1, eta2, du, dv>(pkey, txt, rcoin, enc);
    const auto t1 = std::chrono::high_resolution_clock::now();

    benchmark::DoNotOptimize(pkey);
    benchmark::DoNotOptimize(txt);
    benchmark::DoNotOptimize(rcoin);
    benchmark::DoNotOptimize(enc);
    benchmark::ClobberMemory();

    const auto sdur = std::chrono::duration_cast<seconds_t>(t1 - t0);
    const auto nsdur = std::chrono::duration_cast<nano_t>(t1 - t0);

    state.SetIterationTime(sdur.count());
    durations.push_back(nsdur.count());
  }

  state.SetItemsProcessed(static_cast<int64_t>(state.iterations()));

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
  constexpr size_t pklen = k * 12 * 32 + 32;
  constexpr size_t sklen = k * 12 * 32;
  constexpr size_t mlen = 32;
  constexpr size_t enclen = k * du * 32 + dv * 32;

  uint8_t* seed = static_cast<uint8_t*>(std::malloc(slen));
  uint8_t* pkey = static_cast<uint8_t*>(std::malloc(pklen));
  uint8_t* skey = static_cast<uint8_t*>(std::malloc(sklen));
  uint8_t* rcoin = static_cast<uint8_t*>(std::malloc(mlen));
  uint8_t* txt = static_cast<uint8_t*>(std::malloc(mlen));
  uint8_t* enc = static_cast<uint8_t*>(std::malloc(enclen));
  uint8_t* dec = static_cast<uint8_t*>(std::malloc(mlen));

  std::vector<uint64_t> durations;

  for (auto _ : state) {
    kyber_utils::random_data<uint8_t>(seed, slen);
    cpapke::keygen<k, eta1>(seed, pkey, skey);

    kyber_utils::random_data<uint8_t>(txt, mlen);
    kyber_utils::random_data<uint8_t>(rcoin, mlen);
    cpapke::encrypt<k, eta1, eta2, du, dv>(pkey, txt, rcoin, enc);

    const auto t0 = std::chrono::high_resolution_clock::now();
    cpapke::decrypt<k, du, dv>(skey, enc, dec);
    const auto t1 = std::chrono::high_resolution_clock::now();

    benchmark::DoNotOptimize(skey);
    benchmark::DoNotOptimize(enc);
    benchmark::DoNotOptimize(dec);
    benchmark::ClobberMemory();

    for (size_t i = 0; i < mlen; i++) {
      assert(!static_cast<bool>(txt[i] ^ dec[i]));
    }

    const auto sdur = std::chrono::duration_cast<seconds_t>(t1 - t0);
    const auto nsdur = std::chrono::duration_cast<nano_t>(t1 - t0);

    state.SetIterationTime(sdur.count());
    durations.push_back(nsdur.count());
  }

  state.SetItemsProcessed(static_cast<int64_t>(state.iterations()));

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

  std::free(seed);
  std::free(pkey);
  std::free(skey);
  std::free(rcoin);
  std::free(txt);
  std::free(enc);
  std::free(dec);
}

}
