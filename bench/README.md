## Benchmarking Kyber PKE & KEM

For benchmarking this Kyber PKE & KEM implementation on CPU systems, you need to issue

```bash
make benchmark
```

> **Note**

> Benchmarking expects presence of `google-benchmark` library in global namespace ( so that it can be found by the compiler ).

> **Warning**

> When benchmarking ensure that you've disabled CPU frequency scaling by following [this](https://github.com/google/benchmark/blob/3b19d722/docs/user_guide.md#disabling-cpu-frequency-scaling) guide.

### On Intel(R) Core(TM) i5-8279U CPU @ 2.40GHz ( compiled with Clang )

```bash
2023-02-26T22:58:42+04:00
Running ./bench/a.out
Run on (8 X 2400 MHz CPU s)
CPU Caches:
  L1 Data 32 KiB
  L1 Instruction 32 KiB
  L2 Unified 256 KiB (x4)
  L3 Unified 6144 KiB
Load Average: 2.06, 2.03, 2.05
------------------------------------------------------------------------------------------
Benchmark                                                Time             CPU   Iterations
------------------------------------------------------------------------------------------
bench_kyber::pke_keygen<2, 3>_mean                    18.2 us         18.2 us            8
bench_kyber::pke_keygen<2, 3>_median                  18.2 us         18.1 us            8
bench_kyber::pke_keygen<2, 3>_stddev                 0.151 us        0.151 us            8
bench_kyber::pke_keygen<2, 3>_cv                      0.83 %          0.83 %             8
bench_kyber::pke_keygen<2, 3>_min                     18.0 us         18.0 us            8
bench_kyber::pke_keygen<2, 3>_max                     18.5 us         18.5 us            8
bench_kyber::encrypt<2, 3, 2, 10, 4>_mean             20.4 us         20.1 us            8
bench_kyber::encrypt<2, 3, 2, 10, 4>_median           20.0 us         20.0 us            8
bench_kyber::encrypt<2, 3, 2, 10, 4>_stddev          0.919 us        0.371 us            8
bench_kyber::encrypt<2, 3, 2, 10, 4>_cv               4.51 %          1.85 %             8
bench_kyber::encrypt<2, 3, 2, 10, 4>_min              19.7 us         19.7 us            8
bench_kyber::encrypt<2, 3, 2, 10, 4>_max              22.3 us         20.7 us            8
bench_kyber::decrypt<2, 3, 2, 10, 4>_mean             5.60 us         5.59 us            8
bench_kyber::decrypt<2, 3, 2, 10, 4>_median           5.59 us         5.59 us            8
bench_kyber::decrypt<2, 3, 2, 10, 4>_stddev          0.029 us        0.029 us            8
bench_kyber::decrypt<2, 3, 2, 10, 4>_cv               0.52 %          0.51 %             8
bench_kyber::decrypt<2, 3, 2, 10, 4>_min              5.56 us         5.56 us            8
bench_kyber::decrypt<2, 3, 2, 10, 4>_max              5.66 us         5.65 us            8
bench_kyber::pke_keygen<3, 2>_mean                    30.8 us         30.7 us            8
bench_kyber::pke_keygen<3, 2>_median                  30.7 us         30.7 us            8
bench_kyber::pke_keygen<3, 2>_stddev                 0.275 us        0.213 us            8
bench_kyber::pke_keygen<3, 2>_cv                      0.89 %          0.69 %             8
bench_kyber::pke_keygen<3, 2>_min                     30.5 us         30.4 us            8
bench_kyber::pke_keygen<3, 2>_max                     31.3 us         31.1 us            8
bench_kyber::encrypt<3, 2, 2, 10, 4>_mean             33.4 us         33.3 us            8
bench_kyber::encrypt<3, 2, 2, 10, 4>_median           33.3 us         33.3 us            8
bench_kyber::encrypt<3, 2, 2, 10, 4>_stddev          0.221 us        0.214 us            8
bench_kyber::encrypt<3, 2, 2, 10, 4>_cv               0.66 %          0.64 %             8
bench_kyber::encrypt<3, 2, 2, 10, 4>_min              33.2 us         33.2 us            8
bench_kyber::encrypt<3, 2, 2, 10, 4>_max              33.9 us         33.8 us            8
bench_kyber::decrypt<3, 2, 2, 10, 4>_mean             7.67 us         7.66 us            8
bench_kyber::decrypt<3, 2, 2, 10, 4>_median           7.66 us         7.65 us            8
bench_kyber::decrypt<3, 2, 2, 10, 4>_stddev          0.054 us        0.037 us            8
bench_kyber::decrypt<3, 2, 2, 10, 4>_cv               0.70 %          0.48 %             8
bench_kyber::decrypt<3, 2, 2, 10, 4>_min              7.64 us         7.63 us            8
bench_kyber::decrypt<3, 2, 2, 10, 4>_max              7.80 us         7.75 us            8
bench_kyber::pke_keygen<4, 2>_mean                    49.0 us         48.9 us            8
bench_kyber::pke_keygen<4, 2>_median                  48.9 us         48.8 us            8
bench_kyber::pke_keygen<4, 2>_stddev                 0.224 us        0.222 us            8
bench_kyber::pke_keygen<4, 2>_cv                      0.46 %          0.45 %             8
bench_kyber::pke_keygen<4, 2>_min                     48.7 us         48.7 us            8
bench_kyber::pke_keygen<4, 2>_max                     49.3 us         49.2 us            8
bench_kyber::encrypt<4, 2, 2, 11, 5>_mean             51.3 us         51.2 us            8
bench_kyber::encrypt<4, 2, 2, 11, 5>_median           51.2 us         51.2 us            8
bench_kyber::encrypt<4, 2, 2, 11, 5>_stddev          0.486 us        0.349 us            8
bench_kyber::encrypt<4, 2, 2, 11, 5>_cv               0.95 %          0.68 %             8
bench_kyber::encrypt<4, 2, 2, 11, 5>_min              50.9 us         50.8 us            8
bench_kyber::encrypt<4, 2, 2, 11, 5>_max              52.4 us         51.9 us            8
bench_kyber::decrypt<4, 2, 2, 11, 5>_mean             10.0 us         10.0 us            8
bench_kyber::decrypt<4, 2, 2, 11, 5>_median           10.0 us         10.0 us            8
bench_kyber::decrypt<4, 2, 2, 11, 5>_stddev          0.030 us        0.029 us            8
bench_kyber::decrypt<4, 2, 2, 11, 5>_cv               0.30 %          0.29 %             8
bench_kyber::decrypt<4, 2, 2, 11, 5>_min             10.00 us         9.99 us            8
bench_kyber::decrypt<4, 2, 2, 11, 5>_max              10.1 us         10.1 us            8
bench_kyber::kem_keygen<2, 3>_mean                    20.3 us         20.3 us            8
bench_kyber::kem_keygen<2, 3>_median                  20.2 us         20.2 us            8
bench_kyber::kem_keygen<2, 3>_stddev                 0.162 us        0.143 us            8
bench_kyber::kem_keygen<2, 3>_cv                      0.80 %          0.71 %             8
bench_kyber::kem_keygen<2, 3>_min                     20.1 us         20.1 us            8
bench_kyber::kem_keygen<2, 3>_max                     20.6 us         20.5 us            8
bench_kyber::encapsulate<2, 3, 2, 10, 4>_mean         24.9 us         24.9 us            8
bench_kyber::encapsulate<2, 3, 2, 10, 4>_median       24.9 us         24.9 us            8
bench_kyber::encapsulate<2, 3, 2, 10, 4>_stddev      0.072 us        0.068 us            8
bench_kyber::encapsulate<2, 3, 2, 10, 4>_cv           0.29 %          0.27 %             8
bench_kyber::encapsulate<2, 3, 2, 10, 4>_min          24.8 us         24.8 us            8
bench_kyber::encapsulate<2, 3, 2, 10, 4>_max          25.0 us         25.0 us            8
bench_kyber::decapsulate<2, 3, 2, 10, 4>_mean         28.3 us         28.2 us            8
bench_kyber::decapsulate<2, 3, 2, 10, 4>_median       28.2 us         28.2 us            8
bench_kyber::decapsulate<2, 3, 2, 10, 4>_stddev      0.217 us        0.150 us            8
bench_kyber::decapsulate<2, 3, 2, 10, 4>_cv           0.77 %          0.53 %             8
bench_kyber::decapsulate<2, 3, 2, 10, 4>_min          28.1 us         28.1 us            8
bench_kyber::decapsulate<2, 3, 2, 10, 4>_max          28.7 us         28.5 us            8
bench_kyber::kem_keygen<3, 2>_mean                    33.9 us         33.7 us            8
bench_kyber::kem_keygen<3, 2>_median                  33.6 us         33.6 us            8
bench_kyber::kem_keygen<3, 2>_stddev                 0.799 us        0.127 us            8
bench_kyber::kem_keygen<3, 2>_cv                      2.35 %          0.38 %             8
bench_kyber::kem_keygen<3, 2>_min                     33.6 us         33.5 us            8
bench_kyber::kem_keygen<3, 2>_max                     35.9 us         33.9 us            8
bench_kyber::encapsulate<3, 2, 2, 10, 4>_mean         40.5 us         40.5 us            8
bench_kyber::encapsulate<3, 2, 2, 10, 4>_median       40.5 us         40.5 us            8
bench_kyber::encapsulate<3, 2, 2, 10, 4>_stddev      0.151 us        0.155 us            8
bench_kyber::encapsulate<3, 2, 2, 10, 4>_cv           0.37 %          0.38 %             8
bench_kyber::encapsulate<3, 2, 2, 10, 4>_min          40.4 us         40.3 us            8
bench_kyber::encapsulate<3, 2, 2, 10, 4>_max          40.9 us         40.8 us            8
bench_kyber::decapsulate<3, 2, 2, 10, 4>_mean         44.9 us         44.8 us            8
bench_kyber::decapsulate<3, 2, 2, 10, 4>_median       44.8 us         44.8 us            8
bench_kyber::decapsulate<3, 2, 2, 10, 4>_stddev      0.168 us        0.164 us            8
bench_kyber::decapsulate<3, 2, 2, 10, 4>_cv           0.37 %          0.37 %             8
bench_kyber::decapsulate<3, 2, 2, 10, 4>_min          44.7 us         44.6 us            8
bench_kyber::decapsulate<3, 2, 2, 10, 4>_max          45.2 us         45.1 us            8
bench_kyber::kem_keygen<4, 2>_mean                    53.0 us         52.9 us            8
bench_kyber::kem_keygen<4, 2>_median                  52.9 us         52.8 us            8
bench_kyber::kem_keygen<4, 2>_stddev                 0.355 us        0.348 us            8
bench_kyber::kem_keygen<4, 2>_cv                      0.67 %          0.66 %             8
bench_kyber::kem_keygen<4, 2>_min                     52.7 us         52.7 us            8
bench_kyber::kem_keygen<4, 2>_max                     53.8 us         53.8 us            8
bench_kyber::encapsulate<4, 2, 2, 11, 5>_mean         60.7 us         60.6 us            8
bench_kyber::encapsulate<4, 2, 2, 11, 5>_median       60.6 us         60.5 us            8
bench_kyber::encapsulate<4, 2, 2, 11, 5>_stddev      0.589 us        0.433 us            8
bench_kyber::encapsulate<4, 2, 2, 11, 5>_cv           0.97 %          0.72 %             8
bench_kyber::encapsulate<4, 2, 2, 11, 5>_min          60.2 us         60.1 us            8
bench_kyber::encapsulate<4, 2, 2, 11, 5>_max          62.1 us         61.6 us            8
bench_kyber::decapsulate<4, 2, 2, 11, 5>_mean         66.5 us         66.4 us            8
bench_kyber::decapsulate<4, 2, 2, 11, 5>_median       66.4 us         66.3 us            8
bench_kyber::decapsulate<4, 2, 2, 11, 5>_stddev      0.418 us        0.270 us            8
bench_kyber::decapsulate<4, 2, 2, 11, 5>_cv           0.63 %          0.41 %             8
bench_kyber::decapsulate<4, 2, 2, 11, 5>_min          66.1 us         66.1 us            8
bench_kyber::decapsulate<4, 2, 2, 11, 5>_max          67.4 us         66.9 us            8
```

### On Intel(R) Xeon(R) CPU E5-2686 v4 @ 2.30GHz ( compiled with GCC )

```bash
2023-02-26T19:02:58+00:00
Running ./bench/a.out
Run on (4 X 2300.08 MHz CPU s)
CPU Caches:
  L1 Data 32 KiB (x2)
  L1 Instruction 32 KiB (x2)
  L2 Unified 256 KiB (x2)
  L3 Unified 46080 KiB (x1)
Load Average: 0.08, 0.02, 0.01
------------------------------------------------------------------------------------------
Benchmark                                                Time             CPU   Iterations
------------------------------------------------------------------------------------------
bench_kyber::pke_keygen<2, 3>_mean                    34.9 us         34.9 us            8
bench_kyber::pke_keygen<2, 3>_median                  34.9 us         34.9 us            8
bench_kyber::pke_keygen<2, 3>_stddev                 0.091 us        0.091 us            8
bench_kyber::pke_keygen<2, 3>_cv                      0.26 %          0.26 %             8
bench_kyber::pke_keygen<2, 3>_min                     34.7 us         34.7 us            8
bench_kyber::pke_keygen<2, 3>_max                     35.0 us         35.0 us            8
bench_kyber::encrypt<2, 3, 2, 10, 4>_mean             44.5 us         44.5 us            8
bench_kyber::encrypt<2, 3, 2, 10, 4>_median           44.5 us         44.5 us            8
bench_kyber::encrypt<2, 3, 2, 10, 4>_stddev          0.053 us        0.052 us            8
bench_kyber::encrypt<2, 3, 2, 10, 4>_cv               0.12 %          0.12 %             8
bench_kyber::encrypt<2, 3, 2, 10, 4>_min              44.5 us         44.5 us            8
bench_kyber::encrypt<2, 3, 2, 10, 4>_max              44.6 us         44.6 us            8
bench_kyber::decrypt<2, 3, 2, 10, 4>_mean             13.8 us         13.8 us            8
bench_kyber::decrypt<2, 3, 2, 10, 4>_median           13.8 us         13.8 us            8
bench_kyber::decrypt<2, 3, 2, 10, 4>_stddev          0.003 us        0.003 us            8
bench_kyber::decrypt<2, 3, 2, 10, 4>_cv               0.02 %          0.02 %             8
bench_kyber::decrypt<2, 3, 2, 10, 4>_min              13.8 us         13.8 us            8
bench_kyber::decrypt<2, 3, 2, 10, 4>_max              13.8 us         13.8 us            8
bench_kyber::pke_keygen<3, 2>_mean                    58.1 us         58.1 us            8
bench_kyber::pke_keygen<3, 2>_median                  58.1 us         58.1 us            8
bench_kyber::pke_keygen<3, 2>_stddev                 0.063 us        0.065 us            8
bench_kyber::pke_keygen<3, 2>_cv                      0.11 %          0.11 %             8
bench_kyber::pke_keygen<3, 2>_min                     58.0 us         58.0 us            8
bench_kyber::pke_keygen<3, 2>_max                     58.2 us         58.2 us            8
bench_kyber::encrypt<3, 2, 2, 10, 4>_mean             72.4 us         72.4 us            8
bench_kyber::encrypt<3, 2, 2, 10, 4>_median           72.4 us         72.4 us            8
bench_kyber::encrypt<3, 2, 2, 10, 4>_stddev          0.061 us        0.061 us            8
bench_kyber::encrypt<3, 2, 2, 10, 4>_cv               0.08 %          0.08 %             8
bench_kyber::encrypt<3, 2, 2, 10, 4>_min              72.2 us         72.2 us            8
bench_kyber::encrypt<3, 2, 2, 10, 4>_max              72.4 us         72.4 us            8
bench_kyber::decrypt<3, 2, 2, 10, 4>_mean             18.2 us         18.2 us            8
bench_kyber::decrypt<3, 2, 2, 10, 4>_median           18.2 us         18.2 us            8
bench_kyber::decrypt<3, 2, 2, 10, 4>_stddev          0.005 us        0.004 us            8
bench_kyber::decrypt<3, 2, 2, 10, 4>_cv               0.03 %          0.02 %             8
bench_kyber::decrypt<3, 2, 2, 10, 4>_min              18.2 us         18.2 us            8
bench_kyber::decrypt<3, 2, 2, 10, 4>_max              18.2 us         18.2 us            8
bench_kyber::pke_keygen<4, 2>_mean                    92.1 us         92.1 us            8
bench_kyber::pke_keygen<4, 2>_median                  92.0 us         92.0 us            8
bench_kyber::pke_keygen<4, 2>_stddev                 0.238 us        0.239 us            8
bench_kyber::pke_keygen<4, 2>_cv                      0.26 %          0.26 %             8
bench_kyber::pke_keygen<4, 2>_min                     91.9 us         91.9 us            8
bench_kyber::pke_keygen<4, 2>_max                     92.7 us         92.7 us            8
bench_kyber::encrypt<4, 2, 2, 11, 5>_mean              109 us          109 us            8
bench_kyber::encrypt<4, 2, 2, 11, 5>_median            109 us          109 us            8
bench_kyber::encrypt<4, 2, 2, 11, 5>_stddev          0.082 us        0.082 us            8
bench_kyber::encrypt<4, 2, 2, 11, 5>_cv               0.08 %          0.07 %             8
bench_kyber::encrypt<4, 2, 2, 11, 5>_min               109 us          109 us            8
bench_kyber::encrypt<4, 2, 2, 11, 5>_max               109 us          109 us            8
bench_kyber::decrypt<4, 2, 2, 11, 5>_mean             22.8 us         22.8 us            8
bench_kyber::decrypt<4, 2, 2, 11, 5>_median           22.8 us         22.8 us            8
bench_kyber::decrypt<4, 2, 2, 11, 5>_stddev          0.024 us        0.024 us            8
bench_kyber::decrypt<4, 2, 2, 11, 5>_cv               0.11 %          0.11 %             8
bench_kyber::decrypt<4, 2, 2, 11, 5>_min              22.7 us         22.7 us            8
bench_kyber::decrypt<4, 2, 2, 11, 5>_max              22.8 us         22.8 us            8
bench_kyber::kem_keygen<2, 3>_mean                    38.6 us         38.6 us            8
bench_kyber::kem_keygen<2, 3>_median                  38.6 us         38.6 us            8
bench_kyber::kem_keygen<2, 3>_stddev                 0.226 us        0.226 us            8
bench_kyber::kem_keygen<2, 3>_cv                      0.58 %          0.59 %             8
bench_kyber::kem_keygen<2, 3>_min                     38.5 us         38.5 us            8
bench_kyber::kem_keygen<2, 3>_max                     39.2 us         39.2 us            8
bench_kyber::encapsulate<2, 3, 2, 10, 4>_mean         54.7 us         54.7 us            8
bench_kyber::encapsulate<2, 3, 2, 10, 4>_median       54.7 us         54.7 us            8
bench_kyber::encapsulate<2, 3, 2, 10, 4>_stddev      0.194 us        0.195 us            8
bench_kyber::encapsulate<2, 3, 2, 10, 4>_cv           0.35 %          0.36 %             8
bench_kyber::encapsulate<2, 3, 2, 10, 4>_min          54.6 us         54.6 us            8
bench_kyber::encapsulate<2, 3, 2, 10, 4>_max          55.2 us         55.2 us            8
bench_kyber::decapsulate<2, 3, 2, 10, 4>_mean         65.2 us         65.2 us            8
bench_kyber::decapsulate<2, 3, 2, 10, 4>_median       65.2 us         65.2 us            8
bench_kyber::decapsulate<2, 3, 2, 10, 4>_stddev      0.032 us        0.033 us            8
bench_kyber::decapsulate<2, 3, 2, 10, 4>_cv           0.05 %          0.05 %             8
bench_kyber::decapsulate<2, 3, 2, 10, 4>_min          65.2 us         65.1 us            8
bench_kyber::decapsulate<2, 3, 2, 10, 4>_max          65.2 us         65.2 us            8
bench_kyber::kem_keygen<3, 2>_mean                    63.8 us         63.8 us            8
bench_kyber::kem_keygen<3, 2>_median                  63.8 us         63.8 us            8
bench_kyber::kem_keygen<3, 2>_stddev                 0.191 us        0.191 us            8
bench_kyber::kem_keygen<3, 2>_cv                      0.30 %          0.30 %             8
bench_kyber::kem_keygen<3, 2>_min                     63.7 us         63.7 us            8
bench_kyber::kem_keygen<3, 2>_max                     64.3 us         64.3 us            8
bench_kyber::encapsulate<3, 2, 2, 10, 4>_mean         86.1 us         86.1 us            8
bench_kyber::encapsulate<3, 2, 2, 10, 4>_median       86.1 us         86.1 us            8
bench_kyber::encapsulate<3, 2, 2, 10, 4>_stddev      0.030 us        0.029 us            8
bench_kyber::encapsulate<3, 2, 2, 10, 4>_cv           0.04 %          0.03 %             8
bench_kyber::encapsulate<3, 2, 2, 10, 4>_min          86.1 us         86.1 us            8
bench_kyber::encapsulate<3, 2, 2, 10, 4>_max          86.2 us         86.2 us            8
bench_kyber::decapsulate<3, 2, 2, 10, 4>_mean         99.1 us         99.1 us            8
bench_kyber::decapsulate<3, 2, 2, 10, 4>_median       99.1 us         99.1 us            8
bench_kyber::decapsulate<3, 2, 2, 10, 4>_stddev      0.087 us        0.087 us            8
bench_kyber::decapsulate<3, 2, 2, 10, 4>_cv           0.09 %          0.09 %             8
bench_kyber::decapsulate<3, 2, 2, 10, 4>_min          99.0 us         99.0 us            8
bench_kyber::decapsulate<3, 2, 2, 10, 4>_max          99.3 us         99.3 us            8
bench_kyber::kem_keygen<4, 2>_mean                    99.6 us         99.6 us            8
bench_kyber::kem_keygen<4, 2>_median                  99.6 us         99.6 us            8
bench_kyber::kem_keygen<4, 2>_stddev                 0.102 us        0.103 us            8
bench_kyber::kem_keygen<4, 2>_cv                      0.10 %          0.10 %             8
bench_kyber::kem_keygen<4, 2>_min                     99.4 us         99.4 us            8
bench_kyber::kem_keygen<4, 2>_max                     99.7 us         99.7 us            8
bench_kyber::encapsulate<4, 2, 2, 11, 5>_mean          127 us          127 us            8
bench_kyber::encapsulate<4, 2, 2, 11, 5>_median        127 us          127 us            8
bench_kyber::encapsulate<4, 2, 2, 11, 5>_stddev      0.146 us        0.148 us            8
bench_kyber::encapsulate<4, 2, 2, 11, 5>_cv           0.12 %          0.12 %             8
bench_kyber::encapsulate<4, 2, 2, 11, 5>_min           126 us          126 us            8
bench_kyber::encapsulate<4, 2, 2, 11, 5>_max           127 us          127 us            8
bench_kyber::decapsulate<4, 2, 2, 11, 5>_mean          144 us          144 us            8
bench_kyber::decapsulate<4, 2, 2, 11, 5>_median        144 us          144 us            8
bench_kyber::decapsulate<4, 2, 2, 11, 5>_stddev      0.364 us        0.366 us            8
bench_kyber::decapsulate<4, 2, 2, 11, 5>_cv           0.25 %          0.25 %             8
bench_kyber::decapsulate<4, 2, 2, 11, 5>_min           143 us          143 us            8
bench_kyber::decapsulate<4, 2, 2, 11, 5>_max           144 us          144 us            8
```

### On Intel(R) Xeon(R) CPU E5-2686 v4 @ 2.30GHz ( compiled with Clang )

```bash
2023-02-26T19:05:52+00:00
Running ./bench/a.out
Run on (4 X 2300.08 MHz CPU s)
CPU Caches:
  L1 Data 32 KiB (x2)
  L1 Instruction 32 KiB (x2)
  L2 Unified 256 KiB (x2)
  L3 Unified 46080 KiB (x1)
Load Average: 0.48, 0.29, 0.12
------------------------------------------------------------------------------------------
Benchmark                                                Time             CPU   Iterations
------------------------------------------------------------------------------------------
bench_kyber::pke_keygen<2, 3>_mean                    29.3 us         29.3 us            8
bench_kyber::pke_keygen<2, 3>_median                  29.3 us         29.3 us            8
bench_kyber::pke_keygen<2, 3>_stddev                 0.054 us        0.054 us            8
bench_kyber::pke_keygen<2, 3>_cv                      0.18 %          0.18 %             8
bench_kyber::pke_keygen<2, 3>_min                     29.2 us         29.2 us            8
bench_kyber::pke_keygen<2, 3>_max                     29.4 us         29.4 us            8
bench_kyber::encrypt<2, 3, 2, 10, 4>_mean             31.7 us         31.7 us            8
bench_kyber::encrypt<2, 3, 2, 10, 4>_median           31.6 us         31.6 us            8
bench_kyber::encrypt<2, 3, 2, 10, 4>_stddev          0.186 us        0.186 us            8
bench_kyber::encrypt<2, 3, 2, 10, 4>_cv               0.59 %          0.59 %             8
bench_kyber::encrypt<2, 3, 2, 10, 4>_min              31.6 us         31.6 us            8
bench_kyber::encrypt<2, 3, 2, 10, 4>_max              32.1 us         32.1 us            8
bench_kyber::decrypt<2, 3, 2, 10, 4>_mean             9.79 us         9.79 us            8
bench_kyber::decrypt<2, 3, 2, 10, 4>_median           9.79 us         9.79 us            8
bench_kyber::decrypt<2, 3, 2, 10, 4>_stddev          0.002 us        0.002 us            8
bench_kyber::decrypt<2, 3, 2, 10, 4>_cv               0.03 %          0.02 %             8
bench_kyber::decrypt<2, 3, 2, 10, 4>_min              9.78 us         9.78 us            8
bench_kyber::decrypt<2, 3, 2, 10, 4>_max              9.79 us         9.79 us            8
bench_kyber::pke_keygen<3, 2>_mean                    48.9 us         48.9 us            8
bench_kyber::pke_keygen<3, 2>_median                  48.9 us         48.9 us            8
bench_kyber::pke_keygen<3, 2>_stddev                 0.194 us        0.194 us            8
bench_kyber::pke_keygen<3, 2>_cv                      0.40 %          0.40 %             8
bench_kyber::pke_keygen<3, 2>_min                     48.8 us         48.8 us            8
bench_kyber::pke_keygen<3, 2>_max                     49.4 us         49.4 us            8
bench_kyber::encrypt<3, 2, 2, 10, 4>_mean             52.7 us         52.7 us            8
bench_kyber::encrypt<3, 2, 2, 10, 4>_median           52.7 us         52.6 us            8
bench_kyber::encrypt<3, 2, 2, 10, 4>_stddev          0.053 us        0.053 us            8
bench_kyber::encrypt<3, 2, 2, 10, 4>_cv               0.10 %          0.10 %             8
bench_kyber::encrypt<3, 2, 2, 10, 4>_min              52.6 us         52.5 us            8
bench_kyber::encrypt<3, 2, 2, 10, 4>_max              52.7 us         52.7 us            8
bench_kyber::decrypt<3, 2, 2, 10, 4>_mean             13.2 us         13.2 us            8
bench_kyber::decrypt<3, 2, 2, 10, 4>_median           13.2 us         13.2 us            8
bench_kyber::decrypt<3, 2, 2, 10, 4>_stddev          0.007 us        0.007 us            8
bench_kyber::decrypt<3, 2, 2, 10, 4>_cv               0.05 %          0.05 %             8
bench_kyber::decrypt<3, 2, 2, 10, 4>_min              13.2 us         13.2 us            8
bench_kyber::decrypt<3, 2, 2, 10, 4>_max              13.2 us         13.2 us            8
bench_kyber::pke_keygen<4, 2>_mean                    76.9 us         76.9 us            8
bench_kyber::pke_keygen<4, 2>_median                  76.9 us         76.9 us            8
bench_kyber::pke_keygen<4, 2>_stddev                 0.088 us        0.089 us            8
bench_kyber::pke_keygen<4, 2>_cv                      0.12 %          0.12 %             8
bench_kyber::pke_keygen<4, 2>_min                     76.8 us         76.8 us            8
bench_kyber::pke_keygen<4, 2>_max                     77.0 us         77.0 us            8
bench_kyber::encrypt<4, 2, 2, 11, 5>_mean             80.9 us         80.9 us            8
bench_kyber::encrypt<4, 2, 2, 11, 5>_median           80.9 us         80.9 us            8
bench_kyber::encrypt<4, 2, 2, 11, 5>_stddev          0.063 us        0.064 us            8
bench_kyber::encrypt<4, 2, 2, 11, 5>_cv               0.08 %          0.08 %             8
bench_kyber::encrypt<4, 2, 2, 11, 5>_min              80.8 us         80.8 us            8
bench_kyber::encrypt<4, 2, 2, 11, 5>_max              81.0 us         81.0 us            8
bench_kyber::decrypt<4, 2, 2, 11, 5>_mean             16.8 us         16.8 us            8
bench_kyber::decrypt<4, 2, 2, 11, 5>_median           16.8 us         16.8 us            8
bench_kyber::decrypt<4, 2, 2, 11, 5>_stddev          0.003 us        0.003 us            8
bench_kyber::decrypt<4, 2, 2, 11, 5>_cv               0.02 %          0.02 %             8
bench_kyber::decrypt<4, 2, 2, 11, 5>_min              16.8 us         16.8 us            8
bench_kyber::decrypt<4, 2, 2, 11, 5>_max              16.8 us         16.8 us            8
bench_kyber::kem_keygen<2, 3>_mean                    32.4 us         32.4 us            8
bench_kyber::kem_keygen<2, 3>_median                  32.4 us         32.4 us            8
bench_kyber::kem_keygen<2, 3>_stddev                 0.018 us        0.018 us            8
bench_kyber::kem_keygen<2, 3>_cv                      0.06 %          0.06 %             8
bench_kyber::kem_keygen<2, 3>_min                     32.3 us         32.3 us            8
bench_kyber::kem_keygen<2, 3>_max                     32.4 us         32.4 us            8
bench_kyber::encapsulate<2, 3, 2, 10, 4>_mean         39.4 us         39.4 us            8
bench_kyber::encapsulate<2, 3, 2, 10, 4>_median       39.4 us         39.4 us            8
bench_kyber::encapsulate<2, 3, 2, 10, 4>_stddev      0.017 us        0.016 us            8
bench_kyber::encapsulate<2, 3, 2, 10, 4>_cv           0.04 %          0.04 %             8
bench_kyber::encapsulate<2, 3, 2, 10, 4>_min          39.4 us         39.4 us            8
bench_kyber::encapsulate<2, 3, 2, 10, 4>_max          39.4 us         39.4 us            8
bench_kyber::decapsulate<2, 3, 2, 10, 4>_mean         45.7 us         45.7 us            8
bench_kyber::decapsulate<2, 3, 2, 10, 4>_median       45.7 us         45.7 us            8
bench_kyber::decapsulate<2, 3, 2, 10, 4>_stddev      0.180 us        0.180 us            8
bench_kyber::decapsulate<2, 3, 2, 10, 4>_cv           0.39 %          0.39 %             8
bench_kyber::decapsulate<2, 3, 2, 10, 4>_min          45.6 us         45.6 us            8
bench_kyber::decapsulate<2, 3, 2, 10, 4>_max          46.2 us         46.2 us            8
bench_kyber::kem_keygen<3, 2>_mean                    53.6 us         53.6 us            8
bench_kyber::kem_keygen<3, 2>_median                  53.6 us         53.6 us            8
bench_kyber::kem_keygen<3, 2>_stddev                 0.047 us        0.047 us            8
bench_kyber::kem_keygen<3, 2>_cv                      0.09 %          0.09 %             8
bench_kyber::kem_keygen<3, 2>_min                     53.6 us         53.6 us            8
bench_kyber::kem_keygen<3, 2>_max                     53.7 us         53.7 us            8
bench_kyber::encapsulate<3, 2, 2, 10, 4>_mean         63.8 us         63.8 us            8
bench_kyber::encapsulate<3, 2, 2, 10, 4>_median       63.7 us         63.7 us            8
bench_kyber::encapsulate<3, 2, 2, 10, 4>_stddev      0.198 us        0.198 us            8
bench_kyber::encapsulate<3, 2, 2, 10, 4>_cv           0.31 %          0.31 %             8
bench_kyber::encapsulate<3, 2, 2, 10, 4>_min          63.6 us         63.6 us            8
bench_kyber::encapsulate<3, 2, 2, 10, 4>_max          64.2 us         64.2 us            8
bench_kyber::decapsulate<3, 2, 2, 10, 4>_mean         72.0 us         72.0 us            8
bench_kyber::decapsulate<3, 2, 2, 10, 4>_median       72.0 us         72.0 us            8
bench_kyber::decapsulate<3, 2, 2, 10, 4>_stddev      0.034 us        0.034 us            8
bench_kyber::decapsulate<3, 2, 2, 10, 4>_cv           0.05 %          0.05 %             8
bench_kyber::decapsulate<3, 2, 2, 10, 4>_min          71.9 us         71.9 us            8
bench_kyber::decapsulate<3, 2, 2, 10, 4>_max          72.0 us         72.0 us            8
bench_kyber::kem_keygen<4, 2>_mean                    83.3 us         83.3 us            8
bench_kyber::kem_keygen<4, 2>_median                  83.2 us         83.2 us            8
bench_kyber::kem_keygen<4, 2>_stddev                 0.257 us        0.254 us            8
bench_kyber::kem_keygen<4, 2>_cv                      0.31 %          0.31 %             8
bench_kyber::kem_keygen<4, 2>_min                     83.0 us         83.0 us            8
bench_kyber::kem_keygen<4, 2>_max                     83.7 us         83.7 us            8
bench_kyber::encapsulate<4, 2, 2, 11, 5>_mean         95.3 us         95.3 us            8
bench_kyber::encapsulate<4, 2, 2, 11, 5>_median       95.2 us         95.2 us            8
bench_kyber::encapsulate<4, 2, 2, 11, 5>_stddev      0.279 us        0.279 us            8
bench_kyber::encapsulate<4, 2, 2, 11, 5>_cv           0.29 %          0.29 %             8
bench_kyber::encapsulate<4, 2, 2, 11, 5>_min          95.1 us         95.1 us            8
bench_kyber::encapsulate<4, 2, 2, 11, 5>_max          95.8 us         95.8 us            8
bench_kyber::decapsulate<4, 2, 2, 11, 5>_mean          106 us          106 us            8
bench_kyber::decapsulate<4, 2, 2, 11, 5>_median        105 us          105 us            8
bench_kyber::decapsulate<4, 2, 2, 11, 5>_stddev      0.269 us        0.268 us            8
bench_kyber::decapsulate<4, 2, 2, 11, 5>_cv           0.26 %          0.25 %             8
bench_kyber::decapsulate<4, 2, 2, 11, 5>_min           105 us          105 us            8
bench_kyber::decapsulate<4, 2, 2, 11, 5>_max           106 us          106 us            8
```
