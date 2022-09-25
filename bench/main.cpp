#include "bench_kyber.hpp"

// register for benchmarking arithmetic over prime field
BENCHMARK(bench_kyber::ff_add);
BENCHMARK(bench_kyber::ff_compound_add);
BENCHMARK(bench_kyber::ff_sub);
BENCHMARK(bench_kyber::ff_neg);
BENCHMARK(bench_kyber::ff_mul);
BENCHMARK(bench_kyber::ff_inv);
BENCHMARK(bench_kyber::ff_div);

// benchmark runner main routine
BENCHMARK_MAIN();
