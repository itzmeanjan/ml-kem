#include "bench_kyber.hpp"

// register for benchmarking arithmetic over prime field
BENCHMARK(bench_kyber::ff_add);
BENCHMARK(bench_kyber::ff_compound_add);
BENCHMARK(bench_kyber::ff_sub);
BENCHMARK(bench_kyber::ff_neg);
BENCHMARK(bench_kyber::ff_mul);
BENCHMARK(bench_kyber::ff_inv);
BENCHMARK(bench_kyber::ff_div);
BENCHMARK(bench_kyber::ff_exp);

// register for benchmarking polynomial encoding and byte array decoding
BENCHMARK(bench_kyber::encode<1>);
BENCHMARK(bench_kyber::encode<4>);
BENCHMARK(bench_kyber::encode<5>);
BENCHMARK(bench_kyber::encode<10>);
BENCHMARK(bench_kyber::encode<11>);
BENCHMARK(bench_kyber::encode<12>);

// benchmark runner main routine
BENCHMARK_MAIN();
