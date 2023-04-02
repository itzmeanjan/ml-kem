## Benchmarking Kyber PKE & KEM

For benchmarking this Kyber PKE & KEM implementation on CPU systems, you need to issue

```bash
make benchmark
```

> **Note** Benchmarking expects presence of `google-benchmark` header and library in global namespace ( so that it can be found by the compiler ).

> **Warning** When benchmarking ensure that you've disabled CPU frequency scaling by following [this](https://github.com/google/benchmark/blob/3b19d722/docs/user_guide.md#disabling-cpu-frequency-scaling) guide.

### On Intel(R) Core(TM) i5-8279U CPU @ 2.40GHz ( compiled with Clang )

```bash
2023-04-02T19:34:26+04:00
Running ./bench/a.out
Run on (8 X 2400 MHz CPU s)
CPU Caches:
  L1 Data 32 KiB
  L1 Instruction 32 KiB
  L2 Unified 256 KiB (x4)
  L3 Unified 6144 KiB
Load Average: 2.43, 2.32, 1.94
---------------------------------------------------------------------------------
Benchmark                      Time             CPU   Iterations items_per_second
---------------------------------------------------------------------------------
kyber512_pke_keygen         25.4 us         25.3 us        27572       39.4813k/s
kyber512_pke_encrypt        28.2 us         28.2 us        24645       35.4379k/s
kyber512_pke_decrypt        10.4 us         10.4 us        65829       95.7532k/s
kyber768_pke_keygen         43.0 us         42.9 us        16310       23.2865k/s
kyber768_pke_encrypt        47.2 us         47.1 us        14864       21.2152k/s
kyber768_pke_decrypt        14.2 us         14.2 us        48850       70.2276k/s
kyber1024_pke_keygen        69.7 us         69.6 us         9674       14.3724k/s
kyber1024_pke_encrypt       73.5 us         73.5 us         9286       13.6111k/s
kyber1024_pke_decrypt       18.9 us         18.9 us        35965        52.805k/s
kyber512_kem_keygen         27.5 us         27.4 us        25382       36.4449k/s
kyber512_kem_encap          33.3 us         33.3 us        20795        30.034k/s
kyber512_kem_decap          41.5 us         41.4 us        16699        24.132k/s
kyber768_kem_keygen         45.8 us         45.8 us        15190       21.8362k/s
kyber768_kem_encap          54.5 us         54.5 us        12555       18.3619k/s
kyber768_kem_decap          65.1 us         65.0 us        10342       15.3807k/s
kyber1024_kem_keygen        73.1 us         73.1 us         9409        13.688k/s
kyber1024_kem_encap         82.5 us         82.5 us         8242       12.1227k/s
kyber1024_kem_decap         98.2 us         98.1 us         7060       10.1915k/s
```

### On Intel(R) Xeon(R) CPU E5-2686 v4 @ 2.30GHz ( compiled with GCC )

```bash
2023-04-02T15:41:02+00:00
Running ./bench/a.out
Run on (4 X 2300.08 MHz CPU s)
CPU Caches:
  L1 Data 32 KiB (x2)
  L1 Instruction 32 KiB (x2)
  L2 Unified 256 KiB (x2)
  L3 Unified 46080 KiB (x1)
Load Average: 0.11, 0.03, 0.01
--------------------------------------------------------------------------------
Benchmark                      Time             CPU   Iterations UserCounters...
--------------------------------------------------------------------------------
kyber512_pke_keygen         46.6 us         46.6 us        15043 items_per_second=21.4648k/s
kyber512_pke_encrypt        56.2 us         56.2 us        12478 items_per_second=17.8036k/s
kyber512_pke_decrypt        20.5 us         20.5 us        34207 items_per_second=48.8547k/s
kyber768_pke_keygen         79.9 us         79.9 us         8743 items_per_second=12.5099k/s
kyber768_pke_encrypt        93.8 us         93.8 us         7454 items_per_second=10.6615k/s
kyber768_pke_decrypt        27.9 us         27.9 us        25086 items_per_second=35.8342k/s
kyber1024_pke_keygen         127 us          127 us         5524 items_per_second=7.88339k/s
kyber1024_pke_encrypt        144 us          144 us         4828 items_per_second=6.96582k/s
kyber1024_pke_decrypt       37.1 us         37.1 us        18890 items_per_second=26.947k/s
kyber512_kem_keygen         50.6 us         50.6 us        13817 items_per_second=19.7582k/s
kyber512_kem_encap          66.3 us         66.3 us        10567 items_per_second=15.0841k/s
kyber512_kem_decap          82.1 us         82.1 us         8529 items_per_second=12.1857k/s
kyber768_kem_keygen         86.1 us         86.1 us         8139 items_per_second=11.621k/s
kyber768_kem_encap           107 us          107 us         6563 items_per_second=9.36933k/s
kyber768_kem_decap           128 us          128 us         5455 items_per_second=7.79808k/s
kyber1024_kem_keygen         135 us          135 us         5200 items_per_second=7.4184k/s
kyber1024_kem_encap          162 us          162 us         4323 items_per_second=6.17727k/s
kyber1024_kem_decap          190 us          190 us         3683 items_per_second=5.27069k/s
```

### On Intel(R) Xeon(R) CPU E5-2686 v4 @ 2.30GHz ( compiled with Clang )

```bash
2023-04-02T15:42:07+00:00
Running ./bench/a.out
Run on (4 X 2300.08 MHz CPU s)
CPU Caches:
  L1 Data 32 KiB (x2)
  L1 Instruction 32 KiB (x2)
  L2 Unified 256 KiB (x2)
  L3 Unified 46080 KiB (x1)
Load Average: 0.21, 0.08, 0.02
--------------------------------------------------------------------------------
Benchmark                      Time             CPU   Iterations UserCounters...
--------------------------------------------------------------------------------
kyber512_pke_keygen         40.8 us         40.8 us        17143 items_per_second=24.4998k/s
kyber512_pke_encrypt        46.3 us         46.3 us        15117 items_per_second=21.6126k/s
kyber512_pke_decrypt        17.5 us         17.5 us        39973 items_per_second=57.0944k/s
kyber768_pke_keygen         68.7 us         68.7 us        10176 items_per_second=14.5532k/s
kyber768_pke_encrypt        76.7 us         76.7 us         9132 items_per_second=13.0393k/s
kyber768_pke_decrypt        23.7 us         23.7 us        29529 items_per_second=42.1953k/s
kyber1024_pke_keygen         110 us          110 us         6363 items_per_second=9.079k/s
kyber1024_pke_encrypt        119 us          119 us         5864 items_per_second=8.41309k/s
kyber1024_pke_decrypt       31.3 us         31.3 us        22341 items_per_second=31.9104k/s
kyber512_kem_keygen         44.0 us         44.0 us        15925 items_per_second=22.7486k/s
kyber512_kem_encap          54.1 us         54.1 us        12969 items_per_second=18.4918k/s
kyber512_kem_decap          68.3 us         68.3 us        10255 items_per_second=14.6378k/s
kyber768_kem_keygen         73.4 us         73.4 us         9519 items_per_second=13.6276k/s
kyber768_kem_encap          87.6 us         87.6 us         7970 items_per_second=11.4148k/s
kyber768_kem_decap           107 us          107 us         6566 items_per_second=9.36897k/s
kyber1024_kem_keygen         116 us          116 us         6031 items_per_second=8.61909k/s
kyber1024_kem_encap          133 us          133 us         5266 items_per_second=7.53159k/s
kyber1024_kem_decap          157 us          157 us         4451 items_per_second=6.36268k/s
```
