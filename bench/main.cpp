#include "bench/bench_kyber.hpp"

// register for benchmarking IND-CPA-secure Kyber Public Key Encryption
BENCHMARK(bench_kyber::pke_keygen<2, 3>);        // kyber512
BENCHMARK(bench_kyber::encrypt<2, 3, 2, 10, 4>); // kyber512
BENCHMARK(bench_kyber::decrypt<2, 3, 2, 10, 4>); // kyber512

BENCHMARK(bench_kyber::pke_keygen<3, 2>);        // kyber768
BENCHMARK(bench_kyber::encrypt<3, 2, 2, 10, 4>); // kyber768
BENCHMARK(bench_kyber::decrypt<3, 2, 2, 10, 4>); // kyber768

BENCHMARK(bench_kyber::pke_keygen<4, 2>);        // kyber1024
BENCHMARK(bench_kyber::encrypt<4, 2, 2, 11, 5>); // kyber1024
BENCHMARK(bench_kyber::decrypt<4, 2, 2, 11, 5>); // kyber1024

// register for benchmarking IND-CCA2-secure Kyber Key Encapsulation Mechanism
BENCHMARK(bench_kyber::kem_keygen<2, 3>);            // kyber512
BENCHMARK(bench_kyber::encapsulate<2, 3, 2, 10, 4>); // kyber512
BENCHMARK(bench_kyber::decapsulate<2, 3, 2, 10, 4>); // kyber512

BENCHMARK(bench_kyber::kem_keygen<3, 2>);            // kyber768
BENCHMARK(bench_kyber::encapsulate<3, 2, 2, 10, 4>); // kyber768
BENCHMARK(bench_kyber::decapsulate<3, 2, 2, 10, 4>); // kyber768

BENCHMARK(bench_kyber::kem_keygen<4, 2>);            // kyber1024
BENCHMARK(bench_kyber::encapsulate<4, 2, 2, 11, 5>); // kyber1024
BENCHMARK(bench_kyber::decapsulate<4, 2, 2, 11, 5>); // kyber1024

// benchmark runner main routine
BENCHMARK_MAIN();
