#include "bench_kem.hpp"

// Register for benchmarking IND-CCA2-secure Kyber Key Encapsulation Mechanism

// Kyber512
BENCHMARK(bench_kyber::keygen<2, 3>)->Name("kyber512/keygen");
BENCHMARK(bench_kyber::encapsulate<2, 3, 2, 10, 4>)->Name("kyber512/encap");
BENCHMARK(bench_kyber::decapsulate<2, 3, 2, 10, 4>)->Name("kyber512/decap");

// Kyber768
BENCHMARK(bench_kyber::keygen<3, 2>)->Name("kyber768/keygen");
BENCHMARK(bench_kyber::encapsulate<3, 2, 2, 10, 4>)->Name("kyber768/encap");
BENCHMARK(bench_kyber::decapsulate<3, 2, 2, 10, 4>)->Name("kyber768/decap");

// Kyber1024
BENCHMARK(bench_kyber::keygen<4, 2>)->Name("kyber1024/keygen");
BENCHMARK(bench_kyber::encapsulate<4, 2, 2, 11, 5>)->Name("kyber1024/encap");
BENCHMARK(bench_kyber::decapsulate<4, 2, 2, 11, 5>)->Name("kyber1024/decap");

// benchmark runner main routine
BENCHMARK_MAIN();
