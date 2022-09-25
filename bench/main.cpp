#include "bench_kyber.hpp"

// register for benchmarking arithmetic over prime field
BENCHMARK(bench_kyber::ff_add);

// benchmark runner main routine
BENCHMARK_MAIN();
