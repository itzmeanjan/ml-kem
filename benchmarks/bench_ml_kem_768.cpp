#include "bench_helper.hpp"
#include "ml_kem/ml_kem_768.hpp"
#include <benchmark/benchmark.h>
#include <cassert>

// Benchmarking ML-KEM-768 key generation algorithm.
void
bench_ml_kem_768_keygen(benchmark::State& state)
{
  std::array<uint8_t, ml_kem_768::SEED_D_BYTE_LEN> seed_d{};
  std::array<uint8_t, ml_kem_768::SEED_Z_BYTE_LEN> seed_z{};

  std::array<uint8_t, ml_kem_768::PKEY_BYTE_LEN> pubkey{};
  std::array<uint8_t, ml_kem_768::SKEY_BYTE_LEN> seckey{};

  randomshake::randomshake_t<192> csprng{};

  csprng.generate(seed_d);
  csprng.generate(seed_z);

  for (auto _ : state) {
    ml_kem_768::keygen(seed_d, seed_z, pubkey, seckey);

    benchmark::DoNotOptimize(seed_d);
    benchmark::DoNotOptimize(seed_z);
    benchmark::DoNotOptimize(pubkey);
    benchmark::DoNotOptimize(seckey);
    benchmark::ClobberMemory();
  }

  state.SetItemsProcessed(state.iterations());
}

// Benchmarking ML-KEM-768 encapsulation algorithm.
void
bench_ml_kem_768_encapsulate(benchmark::State& state)
{
  std::array<uint8_t, ml_kem_768::SEED_D_BYTE_LEN> seed_d{};
  std::array<uint8_t, ml_kem_768::SEED_Z_BYTE_LEN> seed_z{};
  std::array<uint8_t, ml_kem_768::SEED_M_BYTE_LEN> seed_m{};

  std::array<uint8_t, ml_kem_768::PKEY_BYTE_LEN> pubkey{};
  std::array<uint8_t, ml_kem_768::SKEY_BYTE_LEN> seckey{};

  std::array<uint8_t, ml_kem_768::CIPHER_TEXT_BYTE_LEN> cipher{};
  std::array<uint8_t, ml_kem_768::SHARED_SECRET_BYTE_LEN> shared_secret{};

  randomshake::randomshake_t<192> csprng{};

  csprng.generate(seed_d);
  csprng.generate(seed_z);
  csprng.generate(seed_m);

  ml_kem_768::keygen(seed_d, seed_z, pubkey, seckey);

  bool is_encapsulated = true;
  for (auto _ : state) {
    is_encapsulated &= ml_kem_768::encapsulate(seed_m, pubkey, cipher, shared_secret);

    benchmark::DoNotOptimize(is_encapsulated);
    benchmark::DoNotOptimize(seed_m);
    benchmark::DoNotOptimize(pubkey);
    benchmark::DoNotOptimize(cipher);
    benchmark::DoNotOptimize(shared_secret);
    benchmark::ClobberMemory();
  }

  assert(is_encapsulated);
  state.SetItemsProcessed(state.iterations());
}

// Benchmarking ML-KEM-768 decapsulation algorithm.
void
bench_ml_kem_768_decapsulate(benchmark::State& state)
{
  std::array<uint8_t, ml_kem_768::SEED_D_BYTE_LEN> seed_d{};
  std::array<uint8_t, ml_kem_768::SEED_Z_BYTE_LEN> seed_z{};
  std::array<uint8_t, ml_kem_768::SEED_M_BYTE_LEN> seed_m{};

  std::array<uint8_t, ml_kem_768::PKEY_BYTE_LEN> pubkey{};
  std::array<uint8_t, ml_kem_768::SKEY_BYTE_LEN> seckey{};

  std::array<uint8_t, ml_kem_768::CIPHER_TEXT_BYTE_LEN> cipher{};
  std::array<uint8_t, ml_kem_768::SHARED_SECRET_BYTE_LEN> shared_secret_sender{};
  std::array<uint8_t, ml_kem_768::SHARED_SECRET_BYTE_LEN> shared_secret_receiver{};

  randomshake::randomshake_t<192> csprng{};

  csprng.generate(seed_d);
  csprng.generate(seed_z);
  csprng.generate(seed_m);

  ml_kem_768::keygen(seed_d, seed_z, pubkey, seckey);
  (void)ml_kem_768::encapsulate(seed_m, pubkey, cipher, shared_secret_sender);

  for (auto _ : state) {
    ml_kem_768::decapsulate(seckey, cipher, shared_secret_receiver);

    benchmark::DoNotOptimize(seckey);
    benchmark::DoNotOptimize(cipher);
    benchmark::DoNotOptimize(shared_secret_receiver);
    benchmark::ClobberMemory();
  }

  state.SetItemsProcessed(state.iterations());
  assert(shared_secret_sender == shared_secret_receiver);
}

BENCHMARK(bench_ml_kem_768_keygen)->Name("ml_kem_768/keygen")->ComputeStatistics("min", compute_min)->ComputeStatistics("max", compute_max);
BENCHMARK(bench_ml_kem_768_encapsulate)->Name("ml_kem_768/encap")->ComputeStatistics("min", compute_min)->ComputeStatistics("max", compute_max);
BENCHMARK(bench_ml_kem_768_decapsulate)->Name("ml_kem_768/decap")->ComputeStatistics("min", compute_min)->ComputeStatistics("max", compute_max);
