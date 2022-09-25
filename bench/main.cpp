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
BENCHMARK(bench_kyber::decode<1>);
BENCHMARK(bench_kyber::encode<4>);
BENCHMARK(bench_kyber::decode<4>);
BENCHMARK(bench_kyber::encode<5>);
BENCHMARK(bench_kyber::decode<5>);
BENCHMARK(bench_kyber::encode<10>);
BENCHMARK(bench_kyber::decode<10>);
BENCHMARK(bench_kyber::encode<11>);
BENCHMARK(bench_kyber::decode<11>);
BENCHMARK(bench_kyber::encode<12>);
BENCHMARK(bench_kyber::decode<12>);

// register for benchmarking of polynomial coefficient (de)compression
BENCHMARK(bench_kyber::compress<1>);
BENCHMARK(bench_kyber::decompress<1>);
BENCHMARK(bench_kyber::compress<4>);
BENCHMARK(bench_kyber::decompress<4>);
BENCHMARK(bench_kyber::compress<5>);
BENCHMARK(bench_kyber::decompress<5>);
BENCHMARK(bench_kyber::compress<10>);
BENCHMARK(bench_kyber::decompress<10>);
BENCHMARK(bench_kyber::compress<11>);
BENCHMARK(bench_kyber::decompress<11>);

// register for benchmarking (i)NTT of degree-255 polynomial over Z_q | q = 3329
BENCHMARK(bench_kyber::ntt);
BENCHMARK(bench_kyber::intt);

// benchmark runner main routine
BENCHMARK_MAIN();
