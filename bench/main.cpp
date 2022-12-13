#include "bench/bench_kyber.hpp"

// Register for benchmarking IND-CPA-secure Kyber Public Key Encryption

// Kyber512
BENCHMARK(bench_kyber::pke_keygen<2, 3>)->UseManualTime();
BENCHMARK(bench_kyber::encrypt<2, 3, 2, 10, 4>)->UseManualTime();
BENCHMARK(bench_kyber::decrypt<2, 3, 2, 10, 4>)->UseManualTime();

// Kyber768
BENCHMARK(bench_kyber::pke_keygen<3, 2>)->UseManualTime();
BENCHMARK(bench_kyber::encrypt<3, 2, 2, 10, 4>)->UseManualTime();
BENCHMARK(bench_kyber::decrypt<3, 2, 2, 10, 4>)->UseManualTime();

// Kyber1024
BENCHMARK(bench_kyber::pke_keygen<4, 2>)->UseManualTime();
BENCHMARK(bench_kyber::encrypt<4, 2, 2, 11, 5>)->UseManualTime();
BENCHMARK(bench_kyber::decrypt<4, 2, 2, 11, 5>)->UseManualTime();

// Register for benchmarking IND-CCA2-secure Kyber Key Encapsulation Mechanism

// Kyber512
BENCHMARK(bench_kyber::kem_keygen<2, 3>)->UseManualTime();
BENCHMARK(bench_kyber::encapsulate<2, 3, 2, 10, 4>)->UseManualTime();
BENCHMARK(bench_kyber::decapsulate<2, 3, 2, 10, 4>)->UseManualTime();

// Kyber768
BENCHMARK(bench_kyber::kem_keygen<3, 2>)->UseManualTime();
BENCHMARK(bench_kyber::encapsulate<3, 2, 2, 10, 4>)->UseManualTime();
BENCHMARK(bench_kyber::decapsulate<3, 2, 2, 10, 4>)->UseManualTime();

// Kyber1024
BENCHMARK(bench_kyber::kem_keygen<4, 2>)->UseManualTime();
BENCHMARK(bench_kyber::encapsulate<4, 2, 2, 11, 5>)->UseManualTime();
BENCHMARK(bench_kyber::decapsulate<4, 2, 2, 11, 5>)->UseManualTime();

// benchmark runner main routine
BENCHMARK_MAIN();
